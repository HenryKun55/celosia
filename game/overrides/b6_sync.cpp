// Round B6 (SOTC_B6SYNC, A/B `b6sync`, kB6Sync; docs/decomp_b6.md): the message boxes whose every party is a carrier, and the EE
// semaphore calls of carried threads, as host primitives.
//
// The boxes. The ios layer's message boxes (iosCreateMsgQueue 0x1b12b8: {+0 slots, +4 capacity, +8 count, +0xc write index,
// +0x14 the receiver inside a receive, +0x18 the waiting senders linked through the ios thread records' +0x28}) used after
// GAMECORE with carriers at both ends are created with no buffer (slots 0, capacity 0): rendezvous points, not queues.
//   0x26e9d0  the GP box     t10 (waitVscheduler) and t9 send, t8 (the VBlank thread) polls it once a VBlank
//   0x26d9a8  the frame box  t9 sends at the end of its frame, t10 (the GP thread) polls it when the GS is done
//   0x1fc870  the idle queue t11 / t9 send (iosWaitIdleThread), t5 (the idle thread) polls it
// On such a box iosSendMsg (0x1b0fc0) with a2 = 1 takes the kernel message semaphore *0x1e0be8, appends its ios thread record
// to +0x18 (+0x28 = 0, linked at the tail), gives the semaphore back and sleeps (iosSleepThread: SleepThread); when it runs
// again it sees +0 still 0 and returns -1 (a2 != 1: -1 at once). iosRecvMsg (0x1b1138) takes the semaphore, takes the whole
// sender list (+0x18 = 0), and wakes it through the callback thread: iosCallBackThreadMgr(0x1b1340, head) takes the callback
// slot's semaphore *0x201dc4, writes the slot (0x201dc0 = 1, 0x201db8 = 0x1b1340, 0x201dbc = head) and wakes t4 (priority 2),
// which preempts the receiver, runs 0x1b1340 (iosWakeupThread of each record from the head through +0x28: WakeupThread of its
// +4), gives the slot's semaphore back and sleeps; the dispatcher then picks the highest ready thread. The receive gives the
// message semaphore back and returns 0 (*a1 is not written: there is no buffer).
//
// B6's host form (on a carrier, the switch on, GAMECORE entered): the box is a host wait list (HostBox::senders, the ios
// records' thread ids in order). A blocking send appends the caller and parks it in place (EeScheduler::carrierSleep:
// SleepThread's records); a receive takes the list and wakes it itself, in order, then makes the one switch the dispatcher
// would make after t4 parked (EeScheduler::carrierWakeList). Why that is exact (docs/decomp_b6.md section 3):
//   - the messages: none is stored (no buffer); the order that matters is the senders' wake order: the list's order, kept;
//   - the kernel message semaphore and the slot's semaphore: taken and given back with no thread between (a host operation
//     runs whole while its carrier holds the baton), so their counts end where the guest path leaves them; the host path is
//     taken only while both are free (count > 0: nobody inside a message call, t4 not busy), else the guest code runs and
//     blocks on them exactly as before;
//   - the hop through t4: t4 runs at priority 2, so none of its wakes preempts it; after it parks the dispatcher picks the
//     highest ready thread. All live threads have distinct priorities, so that pick is the receiver (still at the front of
//     its queue) unless a woken sender outranks it: the same decision carrierWakeList makes after waking the same threads in
//     the same order. What the hop adds is t4's own turn (its SleepThread / WakeupThread records end as they started), the
//     dispatches around it (EE time: the same order of change as host timing after GAMECORE) and the callback slot's words,
//     which B6 writes as the guest path does (nothing but t4 reads them, and only after a wake);
//   - the guest box memory stays what the guest path leaves (+0x18 list and the records' +0x28 links), so the readers that
//     still read it (the idle-turn hook sotc_decomp::idleTurn, the frame cap's release hook, the [b5dog] dump, the loader log)
//     see the same box. The wake list is taken from the guest list as t4 walks it; the host list is checked against it
//     (SOTC_B6_CHECK) and follows it when something else changed the box (the idle-turn hook's rescue).
//
// The semaphores. The EE kernel's semaphores are already host records (EeSemaphore: a count and a FIFO of waiting thread ids,
// EeScheduler); B3 made the stubs' WaitSema / SignalSema / PollSema in place on a carrier (b3_sync.h). B6 puts host calls in
// the stubs' slots (WaitSema 0x107020, SignalSema 0x107000, PollSema 0x107030): on a carried thread's own code they go to the
// record directly (EeScheduler::carrierWaitSema / carrierSignalSema / carrierPollSema: the count, FIFO waiters, the priority
// test that preempts), without the syscall layer (Sync.cpp's binding call, the SOTC_B3_SYNC gate); anything else (the boot,
// interrupt handlers' i* calls, a thread that is not carried) takes the stub as before. The record stays the one object every
// path uses (ReferSemaStatus, the isys fast paths, iSignalSema from handlers), so nothing has to be kept in step.
//
// SOTC_B6_CHECK=1: a shadow of the guest path's semantics for every host operation: per box the sender list the guest code
// would hold (compared with the host list and with the guest memory's list before each operation, the released list, the
// switch the dispatcher would make), per semaphore the record's count and waiters before / after each call against the
// syscall's rules (result, the waiter woken first-in first-out, a block / switch exactly when the syscall would). Bad ones are
// counted and the first few printed ([b6chk]). It also turns on the executor audit (g_ps2xB1Check).
// SOTC_B6_LOG=<frames> (default 600 when on or checking): `[b6sync]`, `[b6box]`, `[b6sema]` lines.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "sotc_decomp.h"

namespace b6sync
{
    namespace
    {
        constexpr uint32_t kSend = 0x001B0FC0u, kRecv = 0x001B1138u;
        constexpr uint32_t kSignalSema = 0x00107000u, kWaitSema = 0x00107020u, kPollSema = 0x00107030u;
        constexpr uint32_t kMsgSemaWord = 0x001E0BE8u;  // the kernel message semaphore's id (every box's lock)
        constexpr uint32_t kThreadRecs = 0x001FF9A0u;   // the ios thread records by kernel thread id (iosGetThread)
        constexpr uint32_t kCbType = 0x00201DC0u, kCbFn = 0x00201DB8u, kCbArg = 0x00201DBCu, kCbSemaWord = 0x00201DC4u; // t4's slot
        constexpr uint32_t kWakeList = 0x001B1340u;     // the callback that wakes a sender list (kernel_sub_001B1340)
        constexpr uint32_t kWakeOne = 0x001B1328u;      // the callback that wakes one thread (kernel_sub_001B1328: iosWakeupThread)
        // The kernel's results (Kernel/Syscalls/Helpers/State.h).
        constexpr int KE_UNKNOWN_SEMID = -408, KE_RELEASE_WAIT = -418, KE_SEMA_ZERO = -419, KE_SEMA_OVF = -420, KE_WAIT_DELETE = -425;

        uint32_t g_on = 0u;
        bool g_check = false;
        uint32_t g_log = 0u;
        PS2Runtime *g_rt = nullptr;
        PS2Runtime::RecompiledFunction g_prevSend = nullptr, g_prevRecv = nullptr, g_prevWait = nullptr, g_prevSignal = nullptr, g_prevPoll = nullptr;

        inline uint32_t rd32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFCu), 4);
            return v;
        }
        inline void wr32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + (a & 0x01FFFFFCu), &v, 4); }
        inline bool okAddr(uint32_t a) { return a != 0u && a < 0x02000000u && (a & 3u) == 0u; }

        uint64_t g_bad = 0;
        void bad(const char *what, uint32_t a, uint32_t b, uint32_t c)
        {
            if (++g_bad <= 8u)
                std::fprintf(stderr, "[b6chk] %s: %x %x %x\n", what, a, b, c);
        }

        // ---- the boxes ------------------------------------------------------------------------------------------------------------
        struct Waiter
        {
            int tid = 0;
            uint32_t rec = 0;
        };
        enum Fallback : int
        {
            kFbCarrier = 0, // not a carried thread's own code (the boot, a handler, t12 / t7)
            kFbShape,       // the box has a buffer or a count (not the rendezvous shape)
            kFbMsgSema,     // the kernel message semaphore is held (someone is inside a message call)
            kFbCbSema,      // the callback slot's semaphore is held (t4 busy)
            kFbRecvIn,      // +0x14 set (a receive in progress: the guest code's error path)
            kFbRecord,      // the caller's ios record does not name it
            kFbFull,        // a buffered box full or with waiting senders (the guest code's full-box path)
            kFbCount
        };
        struct HostBox
        {
            uint32_t addr = 0;
            const char *name = "";
            bool buffered = false;      // round B6: the memory-card box (a buffer of one message) besides the rendezvous boxes
            std::deque<Waiter> senders; // the host wait list (the guest memory's +0x18 list mirrors it)
            std::deque<uint32_t> msgs;  // a buffered box's messages in order (the guest buffer mirrors it)
            uint64_t recvTook = 0, recvBlock = 0, recvEmpty = 0, wokeRecv = 0, lockWaits = 0;
            // counts (host path): sends (blocking / not), receives (with senders), senders released, of them woken from their
            // sleep / with a wakeup counted, receives that switched, sends resumed, host list re-read from the guest list
            uint64_t sends = 0, sendsBlock = 0, recvs = 0, recvsRelease = 0, released = 0, wokeSleep = 0, wokeCounted = 0, switched = 0, resumed = 0,
                     resync = 0, maxList = 0;
            uint64_t fallback[kFbCount] = {};
            uint32_t senderMask = 0, receiverMask = 0;
            uint64_t seq = 1469598103934665603ull; // FNV-1a of the event sequence (op, thread) in the host path's order
            // the check's shadow: the sender list as the guest path holds it (thread ids)
            std::deque<int> shadow;
            std::deque<uint32_t> shadowMsgs;
            uint64_t shadowSeq = 1469598103934665603ull, checks = 0, badOps = 0;
        };
        HostBox g_boxes[4];
        HostBox *findBox(uint32_t box)
        {
            for (HostBox &b : g_boxes)
                if (b.addr == box)
                    return &b;
            return nullptr;
        }
        inline void mix(uint64_t &h, uint32_t v)
        {
            for (int i = 0; i < 4; ++i)
            {
                h ^= (v >> (8 * i)) & 0xFFu;
                h *= 1099511628211ull;
            }
        }

        // The guest list from +0x18 through the records' +0x28, as 0x1b1340 walks it (at most 64).
        int walkGuest(const uint8_t *rdram, uint32_t box, Waiter *out, int max)
        {
            int n = 0;
            for (uint32_t r = rd32(rdram, box + 0x18u); okAddr(r) && n < max; r = rd32(rdram, r + 0x28u))
                out[n++] = Waiter{static_cast<int>(rd32(rdram, r + 4u)), r};
            return n;
        }

        // Why the host path cannot take this call (-1: it can). A semaphore is free when its record's count is above 0 (then no
        // thread waits on it): WaitSema takes it at once, as the guest path's would. `wake`: the call will wake a thread through
        // the callback slot (its semaphore must be free too).
        int refusalLocks(EeScheduler &ee, const uint8_t *rdram, uint32_t box, bool recv, bool wake)
        {
            const EeSemaphore *msg = ee.semaphore(static_cast<int>(rd32(rdram, kMsgSemaWord)));
            if (!msg || msg->count <= 0)
                return kFbMsgSema;
            if (recv && rd32(rdram, box + 0x14u) != 0u)
                return kFbRecvIn;
            if (wake)
            {
                const EeSemaphore *cb = ee.semaphore(static_cast<int>(rd32(rdram, kCbSemaWord)));
                if (!cb || cb->count <= 0)
                    return kFbCbSema;
            }
            return -1;
        }
        int refusal(EeScheduler &ee, const uint8_t *rdram, const HostBox &b, bool recv)
        {
            if (!ee.carrierCallable())
                return kFbCarrier;
            const uint32_t box = b.addr;
            if (!b.buffered)
            {
                // the rendezvous shape: no buffer, no capacity, no count
                if (rd32(rdram, box) != 0u || rd32(rdram, box + 4u) != 0u || rd32(rdram, box + 8u) != 0u)
                    return kFbShape;
                return refusalLocks(ee, rdram, box, recv, recv && rd32(rdram, box + 0x18u) != 0u);
            }
            // a buffer of `capacity` 8-byte slots, its count within it, no sender waiting (the full-box path stays the guest code's)
            const uint32_t slots = rd32(rdram, box), cap = rd32(rdram, box + 4u), count = rd32(rdram, box + 8u), wr = rd32(rdram, box + 0xCu);
            if (!okAddr(slots) || cap == 0u || cap > 64u || count > cap || wr >= cap)
                return kFbShape;
            if (rd32(rdram, box + 0x18u) != 0u || (!recv && count >= cap))
                return kFbFull;
            return refusalLocks(ee, rdram, box, recv, !recv && rd32(rdram, box + 0x14u) != 0u);
        }

        // The host list against the guest memory's list (the guest list wins: it is what the guest code would walk). Also the
        // check's shadow against both.
        void syncList(HostBox &b, const uint8_t *rdram)
        {
            Waiter g[64];
            const int n = walkGuest(rdram, b.addr, g, 64);
            bool same = static_cast<int>(b.senders.size()) == n;
            for (int i = 0; same && i < n; ++i)
                same = b.senders[static_cast<size_t>(i)].rec == g[i].rec && b.senders[static_cast<size_t>(i)].tid == g[i].tid;
            if (!same)
            {
                ++b.resync; // changed outside the host path (the idle-turn hook's rescue, a guest-path call while refused)
                b.senders.assign(g, g + n);
                b.shadowSeq = b.seq; // (the sequences compare the host path's own events from here)
                if (g_check)
                {
                    b.shadow.clear();
                    for (int i = 0; i < n; ++i)
                        b.shadow.push_back(g[i].tid);
                }
            }
            if (g_check)
            {
                ++b.checks;
                bool sh = b.shadow.size() == b.senders.size();
                for (size_t i = 0; sh && i < b.shadow.size(); ++i)
                    sh = b.shadow[i] == b.senders[i].tid;
                if (!sh)
                {
                    ++b.badOps;
                    bad("box list != shadow", b.addr, static_cast<uint32_t>(b.shadow.size()), static_cast<uint32_t>(b.senders.size()));
                    b.shadow.clear();
                    for (const Waiter &w : b.senders)
                        b.shadow.push_back(w.tid);
                }
            }
        }

        // ---- the memory-card box 0x121af50 (a buffer of one 8-byte slot; t9 sends, t12 receives once t12 is carried) --------
        // The guest buffer's messages in order (from the oldest: write index - count, wrapping), as iosRecvMsg reads them.
        void syncMsgs(HostBox &b, const uint8_t *rdram)
        {
            const uint32_t slots = rd32(rdram, b.addr), cap = rd32(rdram, b.addr + 4u), count = rd32(rdram, b.addr + 8u), wr = rd32(rdram, b.addr + 0xCu);
            uint32_t g[64];
            int n = 0;
            for (uint32_t i = 0; i < count && n < 64; ++i)
            {
                int32_t idx = static_cast<int32_t>(wr) - static_cast<int32_t>(count) + static_cast<int32_t>(i);
                if (idx < 0)
                    idx += static_cast<int32_t>(cap);
                g[n++] = rd32(rdram, slots + static_cast<uint32_t>(idx) * 8u);
            }
            bool same = static_cast<int>(b.msgs.size()) == n;
            for (int i = 0; same && i < n; ++i)
                same = b.msgs[static_cast<size_t>(i)] == g[i];
            if (!same)
            {
                ++b.resync;
                b.msgs.assign(g, g + n);
                b.shadowSeq = b.seq;
                if (g_check)
                    b.shadowMsgs.assign(g, g + n);
            }
            if (g_check)
            {
                ++b.checks;
                if (b.shadowMsgs != b.msgs)
                {
                    ++b.badOps;
                    bad("box messages != shadow", b.addr, static_cast<uint32_t>(b.shadowMsgs.size()), static_cast<uint32_t>(b.msgs.size()));
                    b.shadowMsgs = b.msgs;
                }
            }
        }

        // iosSendMsg to a buffered box with room (0x1b101c..0x1b1070): the message in the slot at the write index, the index
        // on (wrapping), the count up; a receiver waiting in the box (+0x14) is woken through the callback slot (0x1b1328:
        // iosWakeupThread) after the box is updated; v0 0.
        void sendBuffered(HostBox &b, uint8_t *rdram, R5900Context *ctx, EeScheduler &ee)
        {
            syncMsgs(b, rdram);
            const int self = ee.currentThreadId();
            const uint32_t ra = GPR_U32(ctx, 31), msg = GPR_U32(ctx, 5);
            ctx->pc = ra;
            ++b.sends;
            b.senderMask |= 1u << (self & 31);
            const uint32_t slots = rd32(rdram, b.addr), cap = rd32(rdram, b.addr + 4u), count = rd32(rdram, b.addr + 8u), wr = rd32(rdram, b.addr + 0xCu);
            wr32(rdram, slots + wr * 8u, msg);
            const int32_t next = static_cast<int32_t>(wr) + 1;
            wr32(rdram, b.addr + 0xCu, next < static_cast<int32_t>(cap) ? static_cast<uint32_t>(next) : 0u);
            wr32(rdram, b.addr + 8u, count + 1u);
            b.msgs.push_back(msg);
            mix(b.seq, 0x40000u | static_cast<uint32_t>(self));
            mix(b.seq, msg);
            if (g_check)
            {
                b.shadowMsgs.push_back(msg);
                mix(b.shadowSeq, 0x40000u | static_cast<uint32_t>(self));
                mix(b.shadowSeq, msg);
            }
            const uint32_t waiter = rd32(rdram, b.addr + 0x14u);
            if (okAddr(waiter))
            {
                wr32(rdram, b.addr + 0x14u, 0u);
                wr32(rdram, kCbType, 1u);
                wr32(rdram, kCbFn, kWakeOne);
                wr32(rdram, kCbArg, waiter);
                int id = static_cast<int>(rd32(rdram, waiter + 4u)), result = 0;
                const GuestThread *t = ee.thread(id), *me = ee.currentThread();
                const bool sleeping = t && (t->status == EeThreadStatus::Waiting || t->status == EeThreadStatus::WaitingSuspended) && t->wait.reason == EeWaitReason::Sleep;
                const bool expectSwitch = sleeping && t->suspendCount == 0 && me && t->currentPriority < me->currentPriority;
                ++b.wokeRecv;
                bool switched = false;
                if (!ee.carrierWakeList(&id, 1, &result, switched))
                    bad("send: wake refused", b.addr, static_cast<uint32_t>(self), 0u);
                if (result < 0)
                    bad("send: the receiver's wake failed (the guest path's iosJumpRecoverPoint)", b.addr, static_cast<uint32_t>(id), static_cast<uint32_t>(result));
                if (switched)
                    ++b.switched;
                if (g_check && switched != expectSwitch)
                {
                    ++b.badOps;
                    bad("send: the switch differs from the dispatcher's pick after t4", b.addr, switched ? 1u : 0u, expectSwitch ? 1u : 0u);
                }
            }
            SET_GPR_S32(ctx, 2, 0);
            ctx->pc = ra;
        }

        // iosRecvMsg from a buffered box (a1: where the message goes, a2 1: wait for one). The oldest message out, the count
        // down, v0 0; none: -1 (a2 != 1), or the caller waits in the box's +0x14 (its ios record) until a send wakes it, and
        // takes it then (the guest code's loop from its WaitSema: taken in place if the kernel message semaphore is held at
        // that point, and given back at the end, as the guest code's).
        void recvBuffered(HostBox &b, uint8_t *rdram, R5900Context *ctx, EeScheduler &ee, uint32_t rec)
        {
            const int self = ee.currentThreadId();
            const uint32_t ra = GPR_U32(ctx, 31), out = GPR_U32(ctx, 5);
            const bool wait = GPR_U32(ctx, 6) == 1u;
            ctx->pc = ra;
            ++b.recvs;
            b.receiverMask |= 1u << (self & 31);
            const int msgSema = static_cast<int>(rd32(rdram, kMsgSemaWord));
            bool held = false;
            for (;;)
            {
                syncMsgs(b, rdram);
                if (rd32(rdram, b.addr + 0x18u) != 0u)
                    bad("recv: senders wait in a buffered box (the guest code's full-box path)", b.addr, rd32(rdram, b.addr + 0x18u), 0u);
                const uint32_t slots = rd32(rdram, b.addr), cap = rd32(rdram, b.addr + 4u), count = rd32(rdram, b.addr + 8u), wr = rd32(rdram, b.addr + 0xCu);
                if (count == 0u)
                {
                    if (!wait)
                    {
                        ++b.recvEmpty;
                        mix(b.seq, 0x50000u | static_cast<uint32_t>(self));
                        if (g_check)
                            mix(b.shadowSeq, 0x50000u | static_cast<uint32_t>(self));
                        SET_GPR_S32(ctx, 2, -1);
                        break;
                    }
                    ++b.recvBlock;
                    wr32(rdram, b.addr + 0x14u, rec);
                    mix(b.seq, 0x60000u | static_cast<uint32_t>(self));
                    if (g_check)
                        mix(b.shadowSeq, 0x60000u | static_cast<uint32_t>(self));
                    if (held)
                    {
                        int r = 0;
                        (void)ee.carrierSignalSema(msgSema, r);
                        held = false;
                    }
                    int r = 0;
                    if (!ee.carrierSleep(r))
                        bad("recv: sleep refused", b.addr, static_cast<uint32_t>(self), 0u);
                    const EeSemaphore *m = ee.semaphore(msgSema);
                    if (!m || m->count <= 0)
                    {
                        ++b.lockWaits;
                        int w = 0;
                        (void)ee.carrierWaitSema(msgSema, w);
                        held = true;
                    }
                    continue;
                }
                int32_t idx = static_cast<int32_t>(wr) - static_cast<int32_t>(count);
                if (idx < 0)
                    idx += static_cast<int32_t>(cap);
                const uint32_t msg = rd32(rdram, slots + static_cast<uint32_t>(idx) * 8u);
                if (out != 0u)
                    wr32(rdram, out, msg);
                wr32(rdram, b.addr + 8u, count - 1u);
                wr32(rdram, b.addr + 0x14u, 0u);
                ++b.recvTook;
                if (!b.msgs.empty())
                    b.msgs.pop_front();
                mix(b.seq, 0x70000u | static_cast<uint32_t>(self));
                mix(b.seq, msg);
                if (g_check)
                {
                    ++b.checks;
                    if (b.shadowMsgs.empty() || b.shadowMsgs.front() != msg)
                    {
                        ++b.badOps;
                        bad("recv: message != shadow's oldest", b.addr, msg, b.shadowMsgs.empty() ? 0u : b.shadowMsgs.front());
                    }
                    if (!b.shadowMsgs.empty())
                        b.shadowMsgs.pop_front();
                    mix(b.shadowSeq, 0x70000u | static_cast<uint32_t>(self));
                    mix(b.shadowSeq, msg);
                }
                SET_GPR_S32(ctx, 2, 0);
                break;
            }
            if (held)
            {
                int r = 0;
                (void)ee.carrierSignalSema(msgSema, r);
            }
            ctx->pc = ra;
        }

        // iosSendMsg on a host box (a0 box, a1 message, a2 1: wait for a receiver).
        void sendHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            HostBox *b = g_on ? findBox(GPR_U32(ctx, 4)) : nullptr;
            if (!b)
                return g_prevSend(rdram, ctx, rt);
            EeScheduler &ee = rt->eeScheduler();
            const int why = refusal(ee, rdram, *b, false);
            if (why >= 0)
            {
                ++b->fallback[why];
                return g_prevSend(rdram, ctx, rt);
            }
            const int self = ee.currentThreadId();
            const uint32_t rec = rd32(rdram, kThreadRecs + static_cast<uint32_t>(self) * 4u);
            if (!okAddr(rec) || static_cast<int>(rd32(rdram, rec + 4u)) != self)
            {
                ++b->fallback[kFbRecord];
                return g_prevSend(rdram, ctx, rt);
            }
            if (b->buffered)
                return sendBuffered(*b, rdram, ctx, ee);
            syncList(*b, rdram);
            const uint32_t ra = GPR_U32(ctx, 31);
            const bool wait = GPR_U32(ctx, 6) == 1u;
            ++b->sends;
            b->senderMask |= 1u << (self & 31);
            ctx->pc = ra;
            if (wait)
            {
                ++b->sendsBlock;
                // The guest memory as 0x1b1090..0x1b10c8 leaves it: the record's +0x28 = 0, linked at the list's tail.
                wr32(rdram, rec + 0x28u, 0u);
                uint32_t last = rd32(rdram, b->addr + 0x18u);
                if (!okAddr(last))
                    wr32(rdram, b->addr + 0x18u, rec);
                else
                {
                    for (int guard = 0; guard < 64; ++guard)
                    {
                        const uint32_t next = rd32(rdram, last + 0x28u);
                        if (!okAddr(next))
                            break;
                        last = next;
                    }
                    wr32(rdram, last + 0x28u, rec);
                }
                b->senders.push_back(Waiter{self, rec});
                b->maxList = std::max<uint64_t>(b->maxList, b->senders.size());
                mix(b->seq, 0x10000u | static_cast<uint32_t>(self));
                if (g_check)
                {
                    b->shadow.push_back(self);
                    mix(b->shadowSeq, 0x10000u | static_cast<uint32_t>(self));
                }
                // iosSleepThread: SleepThread in place (a wakeup already counted returns at once).
                int r = 0;
                if (!ee.carrierSleep(r))
                    bad("send: sleep refused", b->addr, static_cast<uint32_t>(self), 0u);
                ++b->resumed;
                // 0x1b10dc: the box's +0 read again: still 0 for these boxes (else the guest code would loop to the top)
                if (rd32(rdram, b->addr) != 0u)
                    bad("send: the box got a buffer", b->addr, rd32(rdram, b->addr), 0u);
            }
            SET_GPR_S32(ctx, 2, -1);
            ctx->pc = ra;
        }

        // iosRecvMsg on a host box (a0 box, a1 out (not written: no buffer), a2 mode (no effect without a buffer)).
        void recvHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            HostBox *b = g_on ? findBox(GPR_U32(ctx, 4)) : nullptr;
            if (!b)
                return g_prevRecv(rdram, ctx, rt);
            EeScheduler &ee = rt->eeScheduler();
            const int why = refusal(ee, rdram, *b, true);
            if (why >= 0)
            {
                ++b->fallback[why];
                return g_prevRecv(rdram, ctx, rt);
            }
            if (b->buffered)
            {
                const int me = ee.currentThreadId();
                const uint32_t rec = rd32(rdram, kThreadRecs + static_cast<uint32_t>(me) * 4u);
                if (!okAddr(rec) || static_cast<int>(rd32(rdram, rec + 4u)) != me)
                {
                    ++b->fallback[kFbRecord];
                    return g_prevRecv(rdram, ctx, rt);
                }
                return recvBuffered(*b, rdram, ctx, ee, rec);
            }
            syncList(*b, rdram);
            const int self = ee.currentThreadId();
            const uint32_t ra = GPR_U32(ctx, 31);
            ++b->recvs;
            b->receiverMask |= 1u << (self & 31);
            ctx->pc = ra;
            const int n = static_cast<int>(std::min<size_t>(b->senders.size(), 64u));
            if (n > 0)
            {
                ++b->recvsRelease;
                int ids[64], results[64];
                bool expectSwitch = false;
                const GuestThread *me = ee.currentThread();
                for (int i = 0; i < n; ++i)
                {
                    ids[i] = b->senders[static_cast<size_t>(i)].tid;
                    const GuestThread *t = ee.thread(ids[i]);
                    const bool sleeping = t && (t->status == EeThreadStatus::Waiting || t->status == EeThreadStatus::WaitingSuspended) &&
                                          t->wait.reason == EeWaitReason::Sleep;
                    ++(sleeping ? b->wokeSleep : b->wokeCounted);
                    if (sleeping && t->suspendCount == 0 && me && t->currentPriority < me->currentPriority)
                        expectSwitch = true;
                    mix(b->seq, 0x20000u | static_cast<uint32_t>(ids[i]));
                }
                if (g_check)
                {
                    // the guest path releases exactly the shadow list, in its order
                    bool same = static_cast<int>(b->shadow.size()) == n;
                    for (int i = 0; same && i < n; ++i)
                        same = b->shadow[static_cast<size_t>(i)] == ids[i];
                    if (!same)
                    {
                        ++b->badOps;
                        bad("recv: released != shadow", b->addr, static_cast<uint32_t>(n), static_cast<uint32_t>(b->shadow.size()));
                    }
                    for (int i = 0; i < n; ++i)
                        mix(b->shadowSeq, 0x20000u | static_cast<uint32_t>(b->shadow.size() > static_cast<size_t>(i) ? b->shadow[static_cast<size_t>(i)] : -1));
                    b->shadow.clear();
                }
                // The guest memory as the guest path leaves it: the list taken (+0x18 = 0; the records keep their links), the
                // callback slot as iosCallBackThreadMgr writes it.
                const uint32_t head = b->senders.front().rec;
                wr32(rdram, b->addr + 0x18u, 0u);
                wr32(rdram, kCbType, 1u);
                wr32(rdram, kCbFn, kWakeList);
                wr32(rdram, kCbArg, head);
                b->senders.clear();
                b->released += static_cast<uint64_t>(n);
                bool switched = false;
                if (!ee.carrierWakeList(ids, n, results, switched))
                    bad("recv: wake refused", b->addr, static_cast<uint32_t>(self), 0u);
                for (int i = 0; i < n; ++i)
                    if (results[i] < 0)
                        bad("recv: a wake failed (the guest path's iosJumpRecoverPoint)", b->addr, static_cast<uint32_t>(ids[i]), static_cast<uint32_t>(results[i]));
                if (switched)
                    ++b->switched;
                if (g_check)
                {
                    ++b->checks;
                    if (switched != expectSwitch)
                    {
                        ++b->badOps;
                        bad("recv: the switch differs from the dispatcher's pick after t4", b->addr, switched ? 1u : 0u, expectSwitch ? 1u : 0u);
                    }
                }
            }
            else
            {
                mix(b->seq, 0x30000u | static_cast<uint32_t>(self));
                if (g_check)
                    mix(b->shadowSeq, 0x30000u | static_cast<uint32_t>(self));
            }
            SET_GPR_S32(ctx, 2, 0);
            ctx->pc = ra;
        }

        // ---- the semaphores ---------------------------------------------------------------------------------------------------
        struct Sema
        {
            int id = 0;
            uint64_t op[3] = {}, hit[3] = {}, fb[3] = {}; // wait (blocked), signal (woke a waiter), poll (took); off the host path
            uint32_t hostMask = 0, otherMask = 0;      // threads on the host path / the stub's path (after GAMECORE)
            uint64_t checks = 0, badOps = 0;
        };
        Sema g_semas[64];
        uint64_t g_semaOther = 0;
        Sema *findSema(int id)
        {
            for (Sema &s : g_semas)
                if (s.id == id || s.id == 0)
                {
                    s.id = id;
                    return &s;
                }
            ++g_semaOther;
            return nullptr;
        }
        // What the syscall's rules say for a call, from the record before it (the check's shadow).
        struct Before
        {
            bool exists = false;
            int count = 0, maxCount = 0, front = 0, frontPrio = 0, selfPrio = 0;
            size_t waiters = 0;
            bool frontSuspended = false;
        };
        Before before(EeScheduler &ee, int id)
        {
            Before r;
            const EeSemaphore *s = ee.semaphore(id);
            if (!s)
                return r;
            r.exists = true;
            r.count = s->count;
            r.maxCount = s->maxCount;
            r.waiters = s->waiters.size();
            if (!s->waiters.empty())
            {
                r.front = s->waiters.front();
                if (const GuestThread *t = ee.thread(r.front))
                    r.frontPrio = t->currentPriority, r.frontSuspended = t->suspendCount != 0;
            }
            if (const GuestThread *me = ee.currentThread())
                r.selfPrio = me->currentPriority;
            return r;
        }

        inline bool semaHostOk(EeScheduler &ee) { return g_on && ee.carrierCallable(); }
        void noteOther(EeScheduler &ee, int id, int op)
        {
            if (!g_on)
                return;
            if (Sema *s = findSema(id))
            {
                ++s->fb[op];
                s->otherMask |= 1u << (ee.currentThreadId() & 31);
            }
        }

        void waitHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            EeScheduler &ee = rt->eeScheduler();
            const int id = static_cast<int>(GPR_U32(ctx, 4));
            if (!semaHostOk(ee))
            {
                noteOther(ee, id, 0);
                return g_prevWait(rdram, ctx, rt);
            }
            ctx->pc = GPR_U32(ctx, 31);
            Sema *s = findSema(id);
            const Before b = g_check ? before(ee, id) : Before{};
            const uint64_t y0 = g_eeB3Sync.yielded[0];
            int r = 0;
            (void)ee.carrierWaitSema(id, r);
            const bool blocked = g_eeB3Sync.yielded[0] != y0;
            if (s)
            {
                ++s->op[0];
                s->hit[0] += blocked ? 1u : 0u;
                s->hostMask |= 1u << (ee.currentThreadId() & 31);
            }
            if (g_check && s)
            {
                ++s->checks;
                // the syscall: no record -> KE_UNKNOWN_SEMID; count > 0 -> taken, the id; else queued at the back and blocked
                // until a signal (the id) or a deletion / release (its code)
                const bool expectBlock = b.exists && b.count == 0;
                bool ok = blocked == expectBlock;
                if (!b.exists)
                    ok = ok && r == KE_UNKNOWN_SEMID;
                else if (!expectBlock)
                {
                    const EeSemaphore *o = ee.semaphore(id);
                    ok = ok && r == id && o && o->count == b.count - 1 && o->waiters.size() == b.waiters;
                }
                else
                {
                    const EeSemaphore *o = ee.semaphore(id);
                    ok = ok && (r == id || r == KE_WAIT_DELETE || r == KE_RELEASE_WAIT) && !(o && std::find(o->waiters.begin(), o->waiters.end(), ee.currentThreadId()) != o->waiters.end());
                }
                if (!ok)
                {
                    ++s->badOps;
                    bad("WaitSema differs from the syscall's rules", static_cast<uint32_t>(id), static_cast<uint32_t>(r), static_cast<uint32_t>(b.count));
                }
            }
        }

        void signalHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            EeScheduler &ee = rt->eeScheduler();
            const int id = static_cast<int>(GPR_U32(ctx, 4));
            if (!semaHostOk(ee))
            {
                noteOther(ee, id, 1);
                return g_prevSignal(rdram, ctx, rt);
            }
            ctx->pc = GPR_U32(ctx, 31);
            Sema *s = findSema(id);
            const Before b = g_check ? before(ee, id) : Before{};
            const EeSemaphore *pre = ee.semaphore(id);
            const bool woke = pre && !pre->waiters.empty();
            const uint64_t y0 = g_eeB3Sync.yielded[1];
            int r = 0;
            (void)ee.carrierSignalSema(id, r);
            const bool switched = g_eeB3Sync.yielded[1] != y0;
            if (s)
            {
                ++s->op[1];
                s->hit[1] += woke ? 1u : 0u;
                s->hostMask |= 1u << (ee.currentThreadId() & 31);
            }
            if (g_check && s)
            {
                ++s->checks;
                // the syscall: a waiter -> the first one readied (the id as its result), a switch when it outranks the caller
                // and is not suspended; else count + 1, or KE_SEMA_OVF at the maximum
                bool ok = true;
                if (!b.exists)
                    ok = r == KE_UNKNOWN_SEMID && !switched;
                else if (b.waiters)
                {
                    const bool expectSwitch = !b.frontSuspended && b.frontPrio < b.selfPrio;
                    ok = r == id && switched == expectSwitch;
                    if (!switched)
                    {
                        const EeSemaphore *o = ee.semaphore(id);
                        const GuestThread *t = ee.thread(b.front);
                        ok = ok && o && o->waiters.size() == b.waiters - 1u && o->count == b.count &&
                             std::find(o->waiters.begin(), o->waiters.end(), b.front) == o->waiters.end() && t &&
                             (t->status == EeThreadStatus::Ready || t->status == EeThreadStatus::Suspended);
                    }
                }
                else
                {
                    const EeSemaphore *o = ee.semaphore(id);
                    ok = !switched && o && (b.count == b.maxCount ? (r == KE_SEMA_OVF && o->count == b.count) : (r == id && o->count == b.count + 1));
                }
                if (!ok)
                {
                    ++s->badOps;
                    bad("SignalSema differs from the syscall's rules", static_cast<uint32_t>(id), static_cast<uint32_t>(r), static_cast<uint32_t>(b.waiters));
                }
            }
        }

        void pollHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            EeScheduler &ee = rt->eeScheduler();
            const int id = static_cast<int>(GPR_U32(ctx, 4));
            if (!semaHostOk(ee))
            {
                noteOther(ee, id, 2);
                return g_prevPoll(rdram, ctx, rt);
            }
            ctx->pc = GPR_U32(ctx, 31);
            Sema *s = findSema(id);
            const Before b = g_check ? before(ee, id) : Before{};
            int r = 0;
            (void)ee.carrierPollSema(id, r);
            if (s)
            {
                ++s->op[2];
                s->hit[2] += r == id ? 1u : 0u;
                s->hostMask |= 1u << (ee.currentThreadId() & 31);
            }
            if (g_check && s)
            {
                ++s->checks;
                const EeSemaphore *o = ee.semaphore(id);
                const bool ok = !b.exists ? r == KE_UNKNOWN_SEMID
                                          : (b.count > 0 ? (r == id && o && o->count == b.count - 1) : (r == KE_SEMA_ZERO && o && o->count == 0));
                if (!ok)
                {
                    ++s->badOps;
                    bad("PollSema differs from the syscall's rules", static_cast<uint32_t>(id), static_cast<uint32_t>(r), static_cast<uint32_t>(b.count));
                }
            }
        }

        // ---- t12: the memory-card manager (GAMECORE mcManager 0x11490f8, priority 47) on a carrier ---------------------------
        // Its loop: iosRecvMsg(0x121af50, sp, 1) (a blocking receive of a request pointer), then by the request's type (+0,
        // 1..14, the jump table at 0x1195a70) one ios memory-card call / manager function, its result in the request's +0x2c,
        // the request's +0 = 0 (done), and the next receive. Written over the guest registers instruction for instruction
        // from the generated function (B3's form: the same loads / stores, register values, delay slots, calls with the same
        // ra, the ELF's JALs read from the code as the generated code reads them (relocated: IndirectCall), the back edges'
        // checkpoints); its labels are the generated function's resume labels, so a hand-back anywhere goes on there.
        enum : int
        {
            v0 = 2, v1 = 3, a0 = 4, a1 = 5, a2 = 6, a3 = 7, t0 = 8, t1 = 9, t2 = 10, s0 = 16, s1 = 17, s2 = 18, s3 = 19, s4 = 20, s5 = 21, s6 = 22,
            s7 = 23, sp = 29, fp = 30
        };
        struct M
        {
            uint8_t *rdram;
            R5900Context *ctx;
            PS2Runtime *rt;
            uint32_t R(int r) const { return GPR_U32(ctx, r); }
            uint64_t R64(int r) const { return GPR_U64(ctx, r); }
            void S(int r, uint32_t v) { SET_GPR_S32(ctx, r, v); }
            void S64(int r, uint64_t v) { SET_GPR_U64(ctx, r, v); }
            void LW(int t, int base, int32_t off) { S(t, rd32(rdram, R(base) + static_cast<uint32_t>(off))); }
            void SW(int t, int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), R(t)); }
            void SW0(int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), 0u); }
            void ADDIU(int t, int s_, int32_t imm) { S(t, R(s_) + static_cast<uint32_t>(imm)); }
            void LUI(int t, uint32_t imm) { S(t, imm << 16); }
            void MOVE(int d, int s_) { S64(d, R64(s_)); } // daddu d, s, zero
            bool jal(uint32_t target, uint32_t ra, uint32_t src)
            {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::DirectCall, "JAL");
            }
            // a JAL into the ELF from GAMECORE, its target read from the code word (relocated at load)
            bool jalCode(uint32_t ra, uint32_t src)
            {
                const uint32_t target = (rd32(rdram, src) & 0x03FFFFFFu) << 2;
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
            }
            bool jalr(uint32_t target, uint32_t ra, uint32_t src)
            {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
            }
            bool back(uint32_t target) { return rt->eeCheckpointDueAt(ctx, target); }
        };
        constexpr uint32_t kT12Entry = 0x011490F8u;
        constexpr uint32_t kT12Claim[] = {0x1149138u, 0x114914Cu, 0x1149180u, 0x1149190u, 0x1149198u, 0x11491A0u, 0x11491A8u, 0x11491B0u, 0x11491B8u,
                                          0x11491C0u, 0x11491C8u, 0x11491D0u, 0x11491D8u, 0x11491F4u, 0x1149218u, 0x1149238u, 0x1149240u, 0x1149260u,
                                          0x114926Cu, 0x1149278u, 0x1149284u, 0x1149290u, 0x11492B0u, 0x11492C0u, 0x11492C8u, 0x11492D0u, 0x11492E0u,
                                          0x11492E8u, 0x1149304u, 0x1149310u, 0x114932Cu, 0x1149338u, 0x1149344u};
        uint32_t g_t12Gate = 0u, g_t12Mask = 1u;
        EeB1Stats g_eeT12{};
        struct T12Stats
        {
            uint64_t bodies = 0, turns = 0, requests[16] = {}, regsBad = 0, badEntry = 0, unwound = 0, jumped = 0;
        } g_t12;
        void bodyT12(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            M m{rdram, ctx, rt};
            ++g_t12.bodies;
            switch (ctx->pc)
            {
            case 0x1149138u: goto L138;
            case 0x114914Cu: goto L14c;
            case 0x1149180u: goto L180;
            case 0x1149190u: goto L190;
            case 0x1149198u: goto L198;
            case 0x11491A0u: goto L1a0;
            case 0x11491A8u: goto L1a8;
            case 0x11491B0u: goto L1b0;
            case 0x11491B8u: goto L1b8;
            case 0x11491C0u: goto L1c0;
            case 0x11491C8u: goto L1c8;
            case 0x11491D0u: goto L1d0;
            case 0x11491D8u: goto L1d8;
            case 0x11491F4u: goto L1f4;
            case 0x1149218u: goto L218;
            case 0x1149238u: goto L238;
            case 0x1149240u: goto L240;
            case 0x1149260u: goto L260;
            case 0x114926Cu: goto L26c;
            case 0x1149278u: goto L278;
            case 0x1149284u: goto L284;
            case 0x1149290u: goto L290;
            case 0x11492B0u: goto L2b0;
            case 0x11492C0u: goto L2c0;
            case 0x11492C8u: goto L2c8;
            case 0x11492D0u: goto L2d0;
            case 0x11492E0u: goto L2e0;
            case 0x11492E8u: goto L2e8;
            case 0x1149304u: goto L304;
            case 0x1149310u: goto L310;
            case 0x114932Cu: goto L32c;
            case 0x1149338u: goto L338;
            case 0x1149344u: goto L344;
            default: ++g_t12.badEntry; return;
            }
        L138:
            ++g_t12.turns;
            if (g_check && (m.R(s5) != 0x01190000u || m.R(s6) != 0x01220000u || m.R(s7) != 0x01190000u || m.R(fp) != 0x01220000u))
            {
                if (++g_t12.regsBad <= 4u)
                    std::fprintf(stderr, "[b6sync] t12: the loop's registers changed: s5 %08x s6 %08x s7 %08x fp %08x\n", m.R(s5), m.R(s6), m.R(s7), m.R(fp));
            }
            // 0x1149138: iosRecvMsg(0x121af50, sp, 1) (a blocking receive: the request pointer at sp+0)
            m.LUI(v0, 0x122u);
            m.MOVE(a1, sp);
            m.ADDIU(a0, v0, -0x50B0);
            m.S(a2, 1u); // (the jal's delay slot)
            if (!m.jalCode(0x114914Cu, 0x1149144u))
                goto unwound;
        L14c:
            // s1 = the request; v0 = its type - 1; sltiu v1, v0, 14 (the 64-bit register, unsigned)
            m.LW(s1, sp, 0);
            m.LW(v0, s1, 0);
            m.ADDIU(v0, v0, -1);
            m.S64(v1, m.R64(v0) < 14u ? 1u : 0u);
            ++g_t12.requests[m.R(v0) < 15u ? m.R(v0) : 15u];
            if (m.R64(v1) == 0u) // beql (delay when taken: a1 = the request's type)
            {
                m.LW(a1, s1, 0);
                goto L33c;
            }
            // the jump table 0x1195a70[v0]
            m.LUI(a0, 0x119u);
            m.S(v0, m.R(v0) << 2);
            m.ADDIU(v1, a0, 0x5A70);
            m.S(v0, m.R(v0) + m.R(v1));
            m.LW(a0, v0, 0);
            ctx->pc = m.R(a0);
            switch (m.R(a0))
            {
            case 0x1149180u: goto L180;
            case 0x1149198u: goto L198;
            case 0x11491A8u: goto L1a8;
            case 0x11491B8u: goto L1b8;
            case 0x11491C8u: goto L1c8;
            case 0x11491D8u: goto L1d8;
            case 0x1149260u: goto L260;
            case 0x1149278u: goto L278;
            case 0x1149290u: goto L290;
            case 0x11492C0u: goto L2c0;
            case 0x11492D0u: goto L2d0;
            case 0x11492E8u: goto L2e8;
            case 0x1149310u: goto L310;
            case 0x1149338u: goto L338;
            default:
                ++g_t12.jumped; // the generated code's PS2_JUMP_GUEST: the thread goes on there as generated code
                return;
            }
        L180: // 1: iosMcGetInfo(+4, +8, sp+4)
            m.LW(a1, s1, 8);
            m.ADDIU(a2, sp, 4);
            m.LW(a0, s1, 4);
            if (!m.jalCode(0x1149190u, 0x1149188u))
                goto unwound;
        L190:
            m.LW(v1, sp, 0); // (the b's delay slot)
            goto L2b4;
        L198: // 2
            m.MOVE(a0, s1);
            if (!m.jal(0x01147F80u, 0x11491A0u, 0x1149198u))
                goto unwound;
        L1a0:
            m.LW(v0, sp, 0);
            goto L348;
        L1a8: // 3
            m.MOVE(a0, s1);
            if (!m.jal(0x01148548u, 0x11491B0u, 0x11491A8u))
                goto unwound;
        L1b0:
            m.LW(v0, sp, 0);
            goto L348;
        L1b8: // 4
            m.MOVE(a0, s1);
            if (!m.jal(0x01148D38u, 0x11491C0u, 0x11491B8u))
                goto unwound;
        L1c0:
            m.LW(v0, sp, 0);
            goto L348;
        L1c8: // 5
            m.MOVE(a0, s1);
            if (!m.jal(0x01148EC8u, 0x11491D0u, 0x11491C8u))
                goto unwound;
        L1d0:
            m.LW(v0, sp, 0);
            goto L348;
        L1d8: // 6: iosMcChDir(+4, +8, 0x11959f0); 0: the directory's file list through the card's handlers
            m.LW(s4, s1, 4);
            m.LUI(v0, 0x119u);
            m.LW(s3, s1, 8);
            m.ADDIU(a2, v0, 0x59F0);
            m.MOVE(a0, s4);
            m.MOVE(a1, s3);
            if (!m.jalCode(0x11491F4u, 0x11491ECu))
                goto unwound;
        L1f4:
            {
                const bool nz = m.R64(v0) != 0u;
                m.SW(v0, s1, 0x2C);
                if (nz)
                    goto L344;
            }
            m.ADDIU(s2, fp, -0x60F0);
            m.S(s0, 0x400u);
            m.MOVE(a0, s2);
            m.ADDIU(a1, s7, 0x5A18);
            m.SW(s4, s2, 0x20);
            m.SW(s3, s2, 0x24);
            if (!m.jalCode(0x1149218u, 0x1149210u)) // strcpy
                goto unwound;
        L218:
            m.SW(s0, s2, 0x2C);
            m.SW(s0, s2, 0x28);
            m.SW0(s2, 0x30);
            m.LW(s0, s6, -0x5088);
            m.LW(v1, s0, 4);
            if (m.R64(v1) == 0u) // beql (delay when taken)
            {
                m.SW0(s1, 0x2C);
                goto L344;
            }
        L238:
            m.MOVE(a0, s2);
            if (!m.jalr(m.R(v1), 0x1149240u, 0x1149238u))
                goto unwound;
        L240:
            {
                const bool nz = m.R64(v0) != 0u;
                m.SW(v0, s1, 0x2C);
                if (nz)
                    goto L344;
            }
            m.ADDIU(s0, s0, 8);
            m.LW(v0, s0, 4);
            {
                const bool more = m.R64(v0) != 0u;
                m.MOVE(v1, v0);
                if (more)
                {
                    if (m.back(0x1149238u))
                        goto unwound;
                    goto L238;
                }
            }
            m.SW0(s1, 0x2C); // (the b's delay slot)
            goto L344;
        L260: // 7: iosMcFormat(+4, +8)
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            if (!m.jalCode(0x114926Cu, 0x1149264u))
                goto unwound;
        L26c:
            m.SW(v0, s1, 0x2C);
            goto L344;
        L278: // 8: iosMcUnformat(+4, +8)
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            if (!m.jalCode(0x1149284u, 0x114927Cu))
                goto unwound;
        L284:
            m.SW(v0, s1, 0x2C);
            goto L344;
        L290: // 9: iosMcGetDir(+4, +8, +0xc, +0x18, +0x1c, +0x20, +0x24)
            m.LW(t2, s1, 0x24);
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            m.LW(a2, s1, 0xC);
            m.LW(a3, s1, 0x18);
            m.LW(t0, s1, 0x1C);
            m.LW(t1, s1, 0x20);
            if (!m.jalCode(0x11492B0u, 0x11492A8u))
                goto unwound;
        L2b0:
            m.LW(v1, sp, 0);
        L2b4:
            m.SW(v0, v1, 0x2C); // (the b's delay slot)
            goto L344;
        L2c0: // 10
            m.MOVE(a0, s1);
            if (!m.jal(0x01149028u, 0x11492C8u, 0x11492C0u))
                goto unwound;
        L2c8:
            m.LW(v0, sp, 0);
            goto L348;
        L2d0: // 11: iosMcDeleteFile(+4, +8, +0xc)
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            m.LW(a2, s1, 0xC);
            if (!m.jalCode(0x11492E0u, 0x11492D8u))
                goto unwound;
        L2e0:
            m.SW(v0, s1, 0x2C);
            goto L344;
        L2e8: // 12: iosMcWriteFile(+4, +8, +0xc, +0x24, +0x18, +0x1c)
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            m.LW(a2, s1, 0xC);
            m.LW(a3, s1, 0x24);
            m.LW(t0, s1, 0x18);
            m.LW(t1, s1, 0x1C);
            if (!m.jalCode(0x1149304u, 0x11492FCu))
                goto unwound;
        L304:
            m.SW(v0, s1, 0x2C);
            goto L344;
        L310: // 13: iosMcReadFile(+4, +8, +0xc, +0x24, +0x18, +0x1c)
            m.LW(a0, s1, 4);
            m.LW(a1, s1, 8);
            m.LW(a2, s1, 0xC);
            m.LW(a3, s1, 0x24);
            m.LW(t0, s1, 0x18);
            m.LW(t1, s1, 0x1C);
            if (!m.jalCode(0x114932Cu, 0x1149324u))
                goto unwound;
        L32c:
            m.SW(v0, s1, 0x2C);
            goto L344;
        L338: // 14 (and any other type): LoaderSysPrintf(0x1195a48, type)
            m.LW(a1, s1, 0);
        L33c:
            m.ADDIU(a0, s5, 0x5A48);
            if (!m.jalCode(0x1149344u, 0x114933Cu))
                goto unwound;
        L344:
            m.LW(v0, sp, 0);
        L348:
            m.SW0(v0, 0); // the request done (the b's delay slot)
            if (m.back(0x1149138u))
                goto unwound;
            goto L138;
        unwound:
            ++g_t12.unwound;
        }

        uint64_t g_lastLogFrame = 0;
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kSend) || !runtime.hasFunction(kRecv) || !runtime.hasFunction(kWaitSema) || !runtime.hasFunction(kSignalSema) ||
            !runtime.hasFunction(kPollSema))
            return;
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B6_CHECK");
        g_check = c && std::atoi(c) != 0;
        if (g_check)
            g_ps2xB1Check = 1u; // the executor audit (shared with B1-B3)
        const char *l = ps2_diag::env("SOTC_B6_LOG");
        const char *on = std::getenv("SOTC_B6SYNC");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        g_boxes[0].addr = 0x0026E9D0u, g_boxes[0].name = "gp";
        g_boxes[1].addr = 0x0026D9A8u, g_boxes[1].name = "frame";
        g_boxes[2].addr = 0x001FC870u, g_boxes[2].name = "idle";
        g_boxes[3].addr = 0x0121AF50u, g_boxes[3].name = "mc", g_boxes[3].buffered = true;
        // The send: below the count wrappers (b2load / b3threads install after this) and above the lifted / generated function.
        g_prevSend = runtime.lookupFunction(kSend);
        runtime.registerFunction(kSend, sendHost);
        // The receive: inside the idle-turn hook (its poll count and idle wait stay around it), else in the slot.
        g_prevRecv = sotc_decomp::swapIosRecvInner(recvHost);
        if (!g_prevRecv)
        {
            g_prevRecv = runtime.lookupFunction(kRecv);
            runtime.registerFunction(kRecv, recvHost);
        }
        g_prevWait = runtime.lookupFunction(kWaitSema);
        runtime.registerFunction(kWaitSema, waitHost);
        g_prevSignal = runtime.lookupFunction(kSignalSema);
        runtime.registerFunction(kSignalSema, signalHost);
        g_prevPoll = runtime.lookupFunction(kPollSema);
        runtime.registerFunction(kPollSema, pollHost);
        if (g_check || g_log)
            std::fprintf(stderr, "[b6sync] installed: check %d log %u\n", g_check ? 1 : 0, g_log);
    }

    // After b3threads::install (the carrier table's first entries stay B1's / B2's / B3's): t12's carrier.
    void installLate(PS2Runtime &runtime)
    {
        if (!g_rt || !runtime.hasFunction(kT12Entry))
            return;
        if (const char *t = std::getenv("SOTC_B6_T12"))
            g_t12Mask = std::atoi(t) != 0 ? 1u : 0u;
        EeScheduler::CarrySpec spec;
        spec.entry = kT12Entry;
        spec.claimPc = kT12Claim[0];
        spec.body = bodyT12;
        spec.gate = &g_t12Gate;
        spec.stats = &g_eeT12;
        spec.claimPcs = kT12Claim;
        spec.claimCount = static_cast<int>(sizeof(kT12Claim) / 4);
        spec.stackKb = 512u;
        spec.budgetKb = 416u;
        if (!runtime.eeScheduler().addHostCarry(spec))
            std::fprintf(stderr, "[b6sync] the carrier table is full: t12 not carried\n");
    }

    // Once a frame (sotc_native's endFrame, after the switches are set for the next frame).
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        g_on = sotc_decomp::switchOn(sotc_decomp::kB6Sync) && sotc_decomp::gamecoreEntered() ? 1u : 0u;
        g_t12Gate = g_on & g_t12Mask; // t12's carrier (SOTC_B6_T12=0: not carried); a carried t12 is handed back at its next yield
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        g_lastLogFrame = frame;
        uint64_t checks = 0, badOps = 0;
        for (const HostBox &b : g_boxes)
            checks += b.checks, badOps += b.badOps;
        for (const Sema &s : g_semas)
            checks += s.checks, badOps += s.badOps;
        std::fprintf(stderr, "[b6sync] frame %llu: on %u | check %d: %llu checks, %llu bad (%llu printed) | sema ids beyond the table %llu\n", (unsigned long long)frame, g_on,
                     g_check ? 1 : 0, (unsigned long long)checks, (unsigned long long)badOps, (unsigned long long)g_bad, (unsigned long long)g_semaOther);
        char line[2400];
        int m = std::snprintf(line, sizeof(line), "[b6box] frame %llu:", (unsigned long long)frame);
        for (const HostBox &b : g_boxes)
            m += std::snprintf(line + m, sizeof(line) - m,
                               " %s %x: sends %llu (wait %llu) resumed %llu recvs %llu (release %llu) released %llu (sleeping %llu counted %llu) switched %llu resync %llu "
                               "max %llu took %llu blocked %llu empty %llu woke-recv %llu lock-waits %llu senders %x receivers %x seq %016llx%s | guest path: not carried %llu shape %llu msgsema %llu cbsema %llu recv-in %llu record %llu full %llu |",
                               b.name, b.addr, (unsigned long long)b.sends, (unsigned long long)b.sendsBlock, (unsigned long long)b.resumed, (unsigned long long)b.recvs,
                               (unsigned long long)b.recvsRelease, (unsigned long long)b.released, (unsigned long long)b.wokeSleep, (unsigned long long)b.wokeCounted,
                               (unsigned long long)b.switched, (unsigned long long)b.resync, (unsigned long long)b.maxList, (unsigned long long)b.recvTook,
                               (unsigned long long)b.recvBlock, (unsigned long long)b.recvEmpty, (unsigned long long)b.wokeRecv, (unsigned long long)b.lockWaits, b.senderMask, b.receiverMask,
                               (unsigned long long)b.seq, g_check ? (b.seq == b.shadowSeq ? " = shadow" : " != shadow") : "", (unsigned long long)b.fallback[kFbCarrier],
                               (unsigned long long)b.fallback[kFbShape], (unsigned long long)b.fallback[kFbMsgSema], (unsigned long long)b.fallback[kFbCbSema],
                               (unsigned long long)b.fallback[kFbRecvIn], (unsigned long long)b.fallback[kFbRecord], (unsigned long long)b.fallback[kFbFull]);
        std::fprintf(stderr, "%s\n", line);
        {
            const EeB1Stats &t = g_eeT12;
            m = std::snprintf(line, sizeof(line), "[b6t12] frame %llu: gate %u claims %llu turns %llu resumes %llu yields cp %llu park %llu hand-backs %llu/%llu/%llu/%llu regs bad %llu bad entry %llu jumped %llu unwound %llu | requests by type:",
                              (unsigned long long)frame, g_t12Gate, (unsigned long long)t.claims, (unsigned long long)g_t12.turns, (unsigned long long)t.resumes,
                              (unsigned long long)t.yieldCheckpoint, (unsigned long long)t.yieldLeave, (unsigned long long)t.handBackSwitch, (unsigned long long)t.handBackUnwind,
                              (unsigned long long)t.handBackThrow, (unsigned long long)t.handBackStop, (unsigned long long)g_t12.regsBad, (unsigned long long)g_t12.badEntry,
                              (unsigned long long)g_t12.jumped, (unsigned long long)g_t12.unwound);
            for (int i = 0; i < 16 && m < 1500; ++i)
                if (g_t12.requests[i])
                    m += std::snprintf(line + m, sizeof(line) - m, " %d:%llu", i + 1, (unsigned long long)g_t12.requests[i]);
            std::fprintf(stderr, "%s\n", line);
        }
        int order[64];
        int k = 0;
        for (int i = 0; i < 64; ++i)
            if (g_semas[i].id)
                order[k++] = i;
        std::sort(order, order + k, [](int x, int y) {
            const Sema &a = g_semas[x], &b = g_semas[y];
            return a.op[0] + a.op[1] + a.op[2] > b.op[0] + b.op[1] + b.op[2];
        });
        m = std::snprintf(line, sizeof(line), "[b6sema] frame %llu: id host wait(blocked) signal(woke) poll(took) / guest path wait signal poll, threads host/guest:", (unsigned long long)frame);
        for (int i = 0; i < k && m < 2200; ++i)
        {
            const Sema &s = g_semas[order[i]];
            m += std::snprintf(line + m, sizeof(line) - m, " %d %llu(%llu) %llu(%llu) %llu(%llu) / %llu %llu %llu %x/%x;", s.id, (unsigned long long)s.op[0], (unsigned long long)s.hit[0],
                               (unsigned long long)s.op[1], (unsigned long long)s.hit[1], (unsigned long long)s.op[2], (unsigned long long)s.hit[2], (unsigned long long)s.fb[0],
                               (unsigned long long)s.fb[1], (unsigned long long)s.fb[2], s.hostMask, s.otherMask);
        }
        std::fprintf(stderr, "%s\n", line);
    }
}
