// Round 91: recompiled guest code on a worker thread.
//
// What it is. One host thread (threads::kGuest: cores 1-2 on the Vita) that
// runs a job handed over by the game thread: host code (a native) that calls
// guest code, recompiled or native, as any native does (sotc_decomp::callGuest).
// The game thread goes on meanwhile and joins it before it needs the job's
// results. What the worker has of its own:
//  - a guest context (R5900Context): a copy of the game thread's at the
//    hand-over (gp, the VU0 state, fcr31...), sp on its own guest stack; VU0's
//    R register (iosGetRandom) is its own from the first job on, so its
//    random numbers are a stream of their own;
//  - a guest stack: 32 KB of the EE kernel area (reserveAsyncCallbackStack,
//    where the runtime's handler stacks live; the game never touches it);
//  - a scratchpad (16 KB) and VU0 data memory (the matrix stack: vi15 and
//    vf1-vf4 are context registers, the stack itself VU0 data), the latter
//    copied from the game thread's at each hand-over;
//  - a runtime shell (ps2_runtime.h, ps2_worker): what generated code reads
//    inline from its PS2Runtime (the EE clock, the host stack floor, the jump
//    serial) is the worker's own, so it never meets the EE scheduler, and
//    every call and jump it makes comes here (hookCall / hookJump).
//
// What it may run beside the game thread. The rule is ownership, checked at
// every call the worker makes (admit): the particle / wind module's code
// (kModuleBegin..kModuleEnd, round 91's job) and a list of pure functions
// (kPure: math, matrix and quaternion arithmetic on their arguments or the
// worker's own VU0 state, string helpers, the camera context's read) run
// beside the game thread. The game thread, for its part, joins before it
// enters any function of the module (its table entries wrapped: gate<I>), so
// the module's data is the worker's while the job runs. Anything else (the
// heap, the isys groups and objects, collision, the motion data, the letters,
// a kernel call) the worker may not run beside the game thread: it waits there
// (park) until the game thread joins; from then on the game thread waits for
// the job's end, so the worker runs the rest alone, as the game thread would.
// In that exclusive part the kernel calls the code makes are answered here
// (the semaphores as uncontended: nobody else runs; GetThreadId the game
// thread's EE thread; the cache flushes nothing); any other kernel call, a
// syscall instruction or a VU0 microprogram on the worker is a fault (logged,
// abort): the job's code does not make them.
//
// The game thread also joins at the heap's garbage moves, the module loader
// (RelocateElfInfoHeader, DecodeSection, isysMoveModuleObj), the next
// hand-over and the frame's end; the job's user joins where it needs the
// results (the display walk after its group pass: decomp_walk.cpp).
//
// Not as the PS2: the job's guest calls charge no EE cycles to the game's
// clock; its random numbers are its own stream.
#include "sotc_worker.h"
#include "sotc_decomp.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <new>
#include <thread>
#include <unordered_map>
#include <vector>

#if defined(__vita__)
extern "C" unsigned int _pthread_stack_default_user; // ps2_vita_runtime.cpp
#endif

namespace sotc_worker
{
    namespace
    {
        // The particle / wind module (create_particle_texture .. the wind's
        // segments): the job's own code; the game thread joins at its entries.
        constexpr uint32_t kModuleBegin = 0x01139CA0u, kModuleEnd = 0x01147530u;
        constexpr uint32_t kModuleEntries[] = {
        0x01139CA0u, 0x01139DC8u, 0x01139F10u, 0x0113A1E8u, 0x0113A340u, 0x0113A5A8u, 0x0113A7B8u, 0x0113A918u,
        0x0113AAE0u, 0x0113ACB8u, 0x0113AEF8u, 0x0113B1D8u, 0x0113B408u, 0x0113B610u, 0x0113B928u, 0x0113C1A8u,
        0x0113C8C8u, 0x0113CA58u, 0x0113D280u, 0x0113D5F0u, 0x0113D7B0u, 0x0113D988u, 0x0113DBF8u, 0x0113DEA0u,
        0x0113E218u, 0x0113E6C8u, 0x0113E940u, 0x0113EC40u, 0x0113F220u, 0x0113F370u, 0x0113F378u, 0x0113F3A8u,
        0x0113F458u, 0x0113F4A0u, 0x0113F500u, 0x0113F530u, 0x0113F600u, 0x0113F6A8u, 0x0113F700u, 0x0113F748u,
        0x0113F760u, 0x0113F7D8u, 0x0113F808u, 0x0113F838u, 0x0113F878u, 0x0113F8B0u, 0x0113F8F8u, 0x0113FA20u,
        0x0113FA28u, 0x0113FA40u, 0x0113FA70u, 0x0113FA88u, 0x0113FA90u, 0x0113FA98u, 0x0113FAA0u, 0x0113FAC0u,
        0x0113FAD8u, 0x0113FB50u, 0x0113FB88u, 0x0113FB98u, 0x0113FBA8u, 0x0113FBB8u, 0x0113FBF0u, 0x0113FC40u,
        0x0113FC98u, 0x0113FDC8u, 0x0113FE20u, 0x0113FF20u, 0x0113FF90u, 0x01140050u, 0x011401D8u, 0x011401F8u,
        0x01140228u, 0x01140238u, 0x011403C0u, 0x011403F8u, 0x01140408u, 0x01140418u, 0x01140440u, 0x011404C0u,
        0x01140548u, 0x01140558u, 0x01140568u, 0x011405B8u, 0x01140630u, 0x01140678u, 0x01140690u, 0x011406E0u,
        0x01140730u, 0x01140738u, 0x01140740u, 0x01140748u, 0x01140898u, 0x011408E0u, 0x011409A8u, 0x01140B70u,
        0x01140C10u, 0x01140C80u, 0x01140CE0u, 0x01140E60u, 0x01140ED0u, 0x01140F50u, 0x01140FD8u, 0x01141080u,
        0x01141090u, 0x011410D0u, 0x01141150u, 0x01141230u, 0x01141400u, 0x011414E8u, 0x01141518u, 0x01141690u,
        0x011416E0u, 0x01141840u, 0x011418D8u, 0x01141918u, 0x01141968u, 0x01141B70u, 0x01141CE0u, 0x01141D20u,
        0x01141E80u, 0x01141EA8u, 0x01142030u, 0x01142080u, 0x01142148u, 0x01142198u, 0x01142230u, 0x011422D8u,
        0x01142350u, 0x011425F0u, 0x01142648u, 0x011426A8u, 0x01144E00u, 0x01144E38u, 0x01144E50u, 0x01144E58u,
        0x01144E70u, 0x01144E88u, 0x01144E90u, 0x01144F68u, 0x01145BA8u, 0x01145C70u, 0x01145CD0u, 0x01145D28u,
        0x01145F78u, 0x011461C0u, 0x011461E0u, 0x01146208u, 0x01146230u, 0x011462A0u, 0x011463B0u, 0x011464C0u,
        0x011464E0u, 0x01146668u, 0x01146818u, 0x01146980u, 0x011469D0u, 0x01146A10u, 0x01146A50u, 0x01146C08u,
        0x01146C30u, 0x01146C40u, 0x01146C58u, 0x01146C70u, 0x01146CC0u, 0x01146D10u, 0x01146D60u, 0x01146D78u,
        0x01146D90u, 0x01146DA8u, 0x01146EB8u, 0x01146FB8u, 0x01147070u, 0x01147150u, 0x011472A8u, 0x01147430u,
        0x01147438u,
        };
        constexpr int kModuleCount = sizeof(kModuleEntries) / sizeof(kModuleEntries[0]);
        constexpr uint32_t kDispParticleManager = 0x0113F220u; // called after the job (callAfterJob), not joined
        // Round 95: set_npe_blend_ratio_particle_emitter (a1 the emitter's handle,
        // f12 the ratio into its +0x16c; v0 1 when found): the console's run
        // joined here in 293 of 480 jobs (a dissolve geodisp's update in the
        // walk's group pass tail-jumps to it), waiting 4.6 ms. Called after the
        // job instead, on the worker: the step it would follow in the original
        // order has run by then; v0 = 1 (its callers do not read it).
        constexpr uint32_t kSetBlendRatio = 0x0113F878u;
        bool afterJobF(uint32_t target, uint32_t ra, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, float f12);

        // Pure functions outside the module, [entry, next function): they read
        // and write only their arguments, the context's registers, the
        // worker's own VU0 data (the matrix stack) or data nobody writes while
        // the job runs (the camera context, the screen size).
        struct Range
        {
            uint32_t begin, end;
        };
        constexpr Range kPure[] = {
            {0x0012821Cu, 0x001282CCu}, // memcpy
            {0x001283D0u, 0x00128490u}, // memset
            {0x00128E6Cu, 0x00128FB0u}, // strcmp
            {0x001290C4u, 0x00129200u}, // strlen
            {0x001B8620u, 0x001B8638u}, // iosUnitCurrentMatrix
            {0x001B8638u, 0x001B8650u}, // iosPushCurrentMatrix (vf1-vf4 into VU0 data at vi15)
            {0x001B8650u, 0x001B8668u}, // iosPopCurrentMatrix
            {0x001B8878u, 0x001B8898u}, // iosTransCurrentMatrix
            {0x001B8C90u, 0x001B8CA8u}, // iosGetCurrentMatrix
            {0x001B8CD0u, 0x001B8D38u}, // iosMulCurrentMatrixR
            {0x001B9148u, 0x001B9170u}, // iosCopyMatrix
            {0x001B91A0u, 0x001B91C8u}, // iosUnitMatrix
            {0x001B9238u, 0x001B92A8u}, // iosInverseMatrix
            {0x001BA4D0u, 0x001BA690u}, // GetDifferencialQuaternionWithNoRegularize
            {0x001BBD20u, 0x001BBD68u}, // SetQuaternionByAxisRotateV
            {0x001BBDA8u, 0x001BBE10u}, // SetQuaternionByAxisRotateVWithNoRegularize
            {0x001BC308u, 0x001BC4B0u}, // GetQuaternionByAxisDirZY
            {0x001BC4B0u, 0x001BC6A0u}, // GetQuaternionByAxisDirZYExact
            {0x001BDDB0u, 0x001BDF00u}, // iosGetSinCosf
            {0x001BDF00u, 0x001BE020u}, // iosGetSinf
            {0x001BE020u, 0x001BE138u}, // iosGetCosf
            {0x001BE260u, 0x001BE2E8u}, // iosGetACosf
            {0x001BE388u, 0x001BE3E0u}, // iosGetACosfExact
            {0x001BE458u, 0x001BE4C0u}, // iosGetATan2fExact
            {0x001BE520u, 0x001BE570u}, // iosGetTableSin
            {0x001BE570u, 0x001BE5B8u}, // iosGetTableCos
            {0x001BE7E8u, 0x001BE838u}, // iosGetRandom (VU0's R: the context's)
            {0x001BE838u, 0x001BE858u}, // iosGetRandomVector
            {0x001BE898u, 0x001BE8B0u}, // iosAbsVector
            {0x001BE8B0u, 0x001BE8E0u}, // iosGetReflectVector
            {0x001BE8F0u, 0x001BE980u}, // iosGetArbitraryUpVectorElement
            {0x001C2038u, 0x001C2048u}, // getDrawScreenWidth
            {0x001C2048u, 0x001C2058u}, // getDrawScreenHeight
            {0x001DD538u, 0x001DD7C8u}, // atanf
            {0x001DD8A0u, 0x001DD8C8u}, // fabsf
            {0x001DD9C8u, 0x001DD9E0u}, // acosf
            {0x001DD9F8u, 0x001DDA10u}, // atan2f
            {0x001DDD78u, 0x001DE1A0u}, // __ieee754_acosf
            {0x001DE540u, 0x001DE720u}, // __ieee754_atan2f
            {0x001DF480u, 0x001DF550u}, // __ieee754_sqrtf
            {0x001DF6A0u, 0x001DFF08u}, // __kernel_rem_pio2f
            {0x001E0010u, 0x001E00E8u}, // floorf
            {0x001E00E8u, 0x001E01B0u}, // scalbnf
            {0x001E01B0u, 0x001E01F0u}, // copysignf
            {0x01087260u, 0x01087300u}, // clipBySphere (round 95: the clip's emitter bounds; the camera context's matrices, VU0 and its clip flags the context's)
            {0x01087498u, 0x010874E8u}, // getZ (round 95: a point through a view matrix, vf24-vf28 the context's)
            {0x0108F518u, 0x0108F530u}, // iosgluGetCameraContext (sotc_native's hook notes the same address again)
            {0x010ED688u, 0x010ED6A8u}, // GetGeoDispClass
            // An attached emitter's position / turn: their own code is a switch
            // on the pointer's kind reading the pointer's sources (positions,
            // quaternions); the motion, collision and cloth kinds' callees are
            // checked on their own (they wait for the join).
            {0x01131D80u, 0x01132098u}, // GetGeometryPointerPosition
            {0x01132098u, 0x01132270u}, // GetGeometryPointerQuaternion
            {0x01133548u, 0x01133748u}, // GetArbitraryDirectionInverseMatrix
            {0x011337E8u, 0x01133888u}, // iosRotMatrixFromDirection
            // The volume tests on a volume's own matrix and sizes (the wind
            // emitters' boxes / spheres / cylinders: GetWindVector's falloff).
            {0x01136240u, 0x011370E8u}, // CheckCubeVolumeWithInverseMatrix .. GetRateInOutCubeVolumeWithInverseMatrix
            {0x011348E0u, 0x011349E8u}, // GetCrossPositionPlaneLine
            {0x01135720u, 0x011357A8u}, // GetMatrixFromQuatPos
            {0x011358C0u, 0x011359F8u}, // GetQuaternionFromDir
            {0x01160E08u, 0x01160E80u}, // GetScreenRadiusParticle
            {0x01160F68u, 0x01160F88u}, // get_cur_camera_context
        };

        // The kernel calls answered in the exclusive part.
        constexpr uint32_t kSignalSema = 0x00107000u, kISignalSema = 0x00107010u, kWaitSema = 0x00107020u, kPollSema = 0x00107030u,
                           kIPollSema = 0x00107040u, kGetThreadId = 0x00106ED0u, kFlushCache = 0x00107260u, kIFlushCache = 0x00107290u,
                           kSyncDCache = 0x001318C0u, kISyncDCache = 0x00131940u;
        // libkernel's and the loader's code: anything else here is a fault on the worker.
        constexpr uint32_t kKernelBegin = 0x00100000u, kKernelEnd = 0x00120000u;

        constexpr uint32_t kGuestStack = 0x8000u;
        // The prepared answers' outputs: the bottom 8 KB of the guest stack (512 x 16 bytes; the job's frames use the top 24 KB).
        constexpr uint32_t kAnswerBytes = 0x2000u;
        struct Answer
        {
            uint32_t target, key, out;
        };
        std::vector<Answer> g_answers, g_pending; // the job's (the worker's), the next one's (the game thread's)
        uint32_t g_answerTargets[2] = {0u, 0u};  // the targets the job's answers are for (one compare per call)
        uint64_t g_answered = 0;
        // The calls kept for the job's end (deferCall), the worker's.
        struct Kept
        {
            uint32_t target, then, a[4], ra;
        };
        std::vector<Kept> g_kept;
        uint64_t g_keptCalls = 0;
        // A dead emitter's end callback from SequenceKickParticle: isysDeleteObj
        // of the small object that tracks the effect for its owner (nobody
        // reads its result): kept for the job's end.
        constexpr uint32_t kKickParticleEnd = 0x011697E8u;
        constexpr size_t kHostStack = 1024u * 1024u, kHostStackSlack = 128u * 1024u;

        struct alignas(64) Own
        {
            R5900Context ctx;
            alignas(64) uint8_t scratchpad[PS2_SCRATCHPAD_SIZE];
            alignas(64) uint8_t vu0Data[PS2_VU0_DATA_SIZE];
        };
        Own *g_own = nullptr;
        PS2Runtime *g_shell = nullptr, *g_runtime = nullptr;
        uint8_t *g_rdram = nullptr;
        uint32_t g_stackTop = 0;
        bool g_started = false, g_failed = false, g_rSeeded = false;
        uintptr_t g_hostFloor = 0;

        // The hand-over (g_mutex): kRunning from kick to the job's end (with
        // the calls after it), kDone until the join.
        enum Phase
        {
            kIdle,
            kRunning,
            kDone
        };
        std::mutex g_mutex;
        std::condition_variable g_cvWork, g_cvPark, g_cvDone;
        Phase g_phase = kIdle;
        bool g_exclusive = false; // the game thread waits for the job's end: the worker runs anything
        bool g_parked = false;
        Job g_job = nullptr;
        void *g_arg = nullptr;
        struct After
        {
            uint32_t target, ra, a[4];
            float f12;
        };
        std::vector<After> g_after;
        std::atomic<bool> g_busy{false}; // the game thread's view: kicked, not joined
        int g_eeThread = 0;
        thread_local bool t_onWorker = false, t_exclusive = false;

        // Statistics (the game thread's, but g_parkAt / g_parkUs / g_jobUs: the worker's, read at the join).
        uint64_t g_startLatUs = 0, g_jobs = 0, g_refused = 0, g_parks = 0, g_joinWaits = 0, g_joinWaitUs = 0, g_jobUsSum = 0, g_parkUsSum = 0, g_afterCalls = 0;
        uint64_t g_siteJoins[kSiteCount] = {}, g_siteWaitUs[kSiteCount] = {};
        uint64_t g_kickUs = 0, g_jobStartUs = 0, g_parkStartUs = 0, g_jobUs = 0, g_parkAtUs = 0;
        bool g_jobParked = false;
        std::unordered_map<uint32_t, uint64_t> g_parkCauses, g_moduleJoins;
        uint32_t g_parkTarget = 0;
        uint64_t g_frames = 0;
        const char *const kSiteNames[kSiteCount] = {"walk", "module", "move", "frame", "kick", "char", "access", "chan"};
        // Round 104: the job's own admit rule, its set-up and its after-join call.
        Admit g_admitFn = nullptr;
        void (*g_joinedFn)(void *) = nullptr;
        void *g_joinedArg = nullptr;
        bool g_census = false;
        bool g_nextRecomp = false; // the next kickAs's job runs the recompiled code beside the game thread
        std::unordered_map<uint32_t, uint64_t> g_calls; // the worker's call targets (census), written by the worker only

        inline uint64_t nowUs() { return native_renderer::nowUs(); }

        [[noreturn]] void fault(const char *what, uint32_t at, const R5900Context *ctx)
        {
            std::fprintf(stderr, "[worker91] FAULT: %s at %08x (pc %08x ra %08x sp %08x, exclusive %d)\n", what, at, ctx ? ctx->pc : 0u,
                         ctx ? GPR_U32(ctx, 31) : 0u, ctx ? GPR_U32(ctx, 29) : 0u, t_exclusive ? 1 : 0);
            std::fflush(stderr);
            std::abort();
        }

        bool safe(uint32_t at)
        {
            if (g_admitFn)
                return g_admitFn(at);
            if (at >= kModuleBegin && at < kModuleEnd)
                return true;
            const Range *r = std::upper_bound(std::begin(kPure), std::end(kPure), at, [](uint32_t a, const Range &x) { return a < x.begin; });
            return r != std::begin(kPure) && at < (r - 1)->end;
        }

        // The worker reached code it may not run beside the game thread: it
        // waits for the join, then runs the rest alone.
        void park(uint32_t target)
        {
            std::unique_lock<std::mutex> lock(g_mutex);
            if (!g_exclusive)
            {
                g_parked = true;
                g_parkTarget = target;
                g_parkStartUs = nowUs();
                g_jobParked = true;
                g_cvPark.wait(lock, [] { return g_exclusive; });
                g_parked = false;
            }
            t_exclusive = true;
        }
        __attribute__((always_inline)) inline void admit(uint32_t target)
        {
            if (!t_exclusive && !safe(target))
                park(target);
        }

        // A kernel call in the exclusive part: true when answered here.
        bool kernel(uint32_t target, R5900Context *ctx)
        {
            if (target < kKernelBegin || (target >= kKernelEnd && target != kSyncDCache && target != kISyncDCache))
                return false;
            switch (target)
            {
            case kWaitSema:
            case kSignalSema:
            case kISignalSema:
            case kPollSema:
            case kIPollSema:
                // Uncontended: nobody else runs while the game thread waits
                // for this job; a Wait / Signal pair leaves the count as it was.
                SET_GPR_S32(ctx, 2, GPR_S32(ctx, 4));
                return true;
            case kGetThreadId:
                SET_GPR_S32(ctx, 2, g_eeThread);
                return true;
            case kFlushCache:
            case kIFlushCache:
            case kSyncDCache:
            case kISyncDCache:
                return true;
            default:
                fault("kernel call", target, ctx);
            }
        }

        // The isys kernel's natives (decomp_isys.cpp, decomp_isys48.cpp: the
        // object locks, the groups' links) call the EE kernel straight
        // (ps2_syscalls::GetThreadId / PollSema, the scheduler): on the worker
        // their recompiled code runs instead, whose kernel calls come here.
        constexpr uint32_t kIsysBegin = 0x001BF000u, kIsysEnd = 0x001C9000u, kTexTransRequest = 0x010AAD48u;
        bool g_recompOnly = false; // round 104: beside the game thread, the recompiled code instead of the natives (no host state shared)
        inline PS2Runtime::RecompiledFunction ownFunction(PS2Runtime *shell, uint32_t target)
        {
            PS2Runtime::RecompiledFunction fn = shell->guestFunctionAt(target);
            if (g_recompOnly && !t_exclusive)
                if (const PS2Runtime::RecompiledFunction r = sotc_decomp::recompiled(target))
                    return r;
            if ((target >= kIsysBegin && target < kIsysEnd) || target == kTexTransRequest)
                if (const PS2Runtime::RecompiledFunction r = sotc_decomp::recompiled(target))
                    fn = r;
            return fn;
        }

        inline void hostStackCheck(uint32_t target, const R5900Context *ctx)
        {
            if (__builtin_expect(reinterpret_cast<uintptr_t>(__builtin_frame_address(0)) < g_hostFloor, 0))
                fault("host stack", target, ctx);
        }

        // Runs on from ctx->pc until the code is back at ra with sp (a callee
        // that left by a jump, a continuation at a label).
        void runOn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, uint32_t ra, uint32_t sp)
        {
            while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
            {
                const uint32_t pc = ctx->pc;
                admit(pc);
                if (kernel(pc, ctx)) // (t_exclusive here: the kernel is never safe)
                {
                    ctx->pc = GPR_U32(ctx, 31);
                    continue;
                }
                const PS2Runtime::RecompiledFunction fn = g_recompOnly ? ownFunction(shell, pc) : shell->guestFunctionAt(pc);
                if (!fn)
                    fault("no function", pc, ctx);
                fn(rdram, ctx, shell);
            }
        }

        bool hookCall(PS2Runtime *shell, uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t returnPc)
        {
            if (!g_answers.empty() && (target == g_answerTargets[0] || target == g_answerTargets[1]))
            {
                const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5);
                for (const Answer &a : g_answers)
                    if (a.target == target && a.key == a1 && a0 < PS2_RAM_SIZE - 16u)
                    {
                        std::memcpy(rdram + a0, rdram + a.out, 16);
                        ctx->pc = returnPc;
                        ++g_answered;
                        return true;
                    }
            }
            if (target == kKickParticleEnd && !t_exclusive &&
                deferCall(target, 0u, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), returnPc))
            {
                ctx->pc = returnPc;
                return true;
            }
            if (__builtin_expect(g_census, 0))
                ++g_calls[target];
            admit(target);
            ctx->pc = target;
            if (kernel(target, ctx))
            {
                ctx->pc = returnPc;
                return true;
            }
            const PS2Runtime::RecompiledFunction fn = ownFunction(shell, target);
            if (!fn)
                fault("no function", target, ctx);
            hostStackCheck(target, ctx);
            const uint32_t sp = GPR_U32(ctx, 29), serial = shell->m_guestJumpSerial;
            fn(rdram, ctx, shell);
            // A callee that came back with its own entry and no jump inside returned (finishGuestCall's rule).
            if (ctx->pc == target && serial == shell->m_guestJumpSerial)
                ctx->pc = returnPc;
            runOn(rdram, ctx, shell, returnPc, sp);
            return true;
        }

        void hookJump(PS2Runtime *shell, uint8_t *rdram, R5900Context *ctx, PS2Runtime::RecompiledFunction fn, uint32_t target)
        {
            ctx->pc = target;
            ++shell->m_guestJumpSerial;
            if (__builtin_expect(g_census, 0))
                ++g_calls[target];
            admit(target);
            if (kernel(target, ctx))
            {
                ctx->pc = GPR_U32(ctx, 31);
                return;
            }
            if (!fn || (target >= kIsysBegin && target < kIsysEnd) || target == kTexTransRequest || g_recompOnly)
                fn = ownFunction(shell, target);
            if (!fn)
                fault("no function", target, ctx);
            hostStackCheck(target, ctx);
            fn(rdram, ctx, shell);
        }

        void hookFault(PS2Runtime *, uint8_t *, R5900Context *ctx, const char *what)
        {
            if (std::strcmp(what, "break") == 0)
            {
                static uint64_t breaks = 0;
                if (++breaks <= 4u)
                    std::fprintf(stderr, "[worker91] break at %08x (ignored)\n", ctx->pc);
                return;
            }
            fault(what, ctx->pc, ctx);
        }

        void loop()
        {
            t_onWorker = true;
            {
                char probe = 0;
                g_hostFloor = reinterpret_cast<uintptr_t>(&probe) - (kHostStack - kHostStackSlack);
            }
            native_renderer::threads::place(native_renderer::threads::kGuest);
            ps2SetEeFloatingPointMode(); // the game thread's rounding (toward zero) and flush-to-zero
            for (;;)
            {
                Job job;
                void *arg;
                {
                    std::unique_lock<std::mutex> lock(g_mutex);
                    g_cvWork.wait(lock, [] { return g_phase == kRunning && g_job != nullptr; });
                    job = g_job;
                    arg = g_arg;
                    g_job = nullptr;
                }
                t_exclusive = false;
                g_jobStartUs = nowUs();
                g_shell->m_eeClock.reset();
                g_shell->m_eeClock.setGate(~0ull);
                R5900Context *const ctx = &g_own->ctx;
                job(g_rdram, ctx, g_shell, arg);
                // The kept calls, once the game thread has joined.
                if (!g_kept.empty())
                {
                    park(kKickParticleEnd);
                    for (const Kept &k : g_kept)
                    {
                        for (int i = 0; i < 4; ++i)
                            SET_GPR_S32(ctx, 4 + i, static_cast<int32_t>(k.a[i]));
                        sotc_decomp::callGuest(g_rdram, ctx, g_shell, k.target, k.ra, g_stackTop - 0x100u);
                        if (k.then != 0u && GPR_U32(ctx, 2) != 0u)
                        {
                            SET_GPR_S32(ctx, 4, static_cast<int32_t>(k.a[0]));
                            sotc_decomp::callGuest(g_rdram, ctx, g_shell, k.then, k.ra, g_stackTop - 0x100u);
                        }
                    }
                    g_keptCalls += g_kept.size();
                    g_kept.clear();
                }
                std::unique_lock<std::mutex> lock(g_mutex);
                // The calls asked for after the job (the game thread reached them first).
                while (!g_after.empty())
                {
                    const After a = g_after.front();
                    g_after.erase(g_after.begin());
                    lock.unlock();
                    for (int i = 0; i < 4; ++i)
                        SET_GPR_S32(ctx, 4 + i, static_cast<int32_t>(a.a[i]));
                    ctx->f[12] = a.f12;
                    sotc_decomp::callGuest(g_rdram, ctx, g_shell, a.target, a.ra, g_stackTop - 0x100u);
                    lock.lock();
                }
                g_jobUs = nowUs() - g_jobStartUs;
                g_parkAtUs = g_jobParked ? g_parkStartUs - g_jobStartUs : 0u;
                g_phase = kDone;
                g_cvDone.notify_all();
            }
        }

        bool start(uint8_t *rdram, PS2Runtime *runtime)
        {
            if (g_started)
                return !g_failed;
            g_started = true;
            g_stackTop = runtime->reserveAsyncCallbackStack(kGuestStack, 16u);
            if (g_stackTop == 0u)
            {
                std::fprintf(stderr, "[worker91] no guest stack: the worker is off\n");
                g_failed = true;
                return false;
            }
            g_own = new Own;
            std::memset(g_own->scratchpad, 0, sizeof(g_own->scratchpad));
            // The shell: zeroed storage; only what generated code reads inline is set.
            void *bytes = ::operator new(sizeof(PS2Runtime), std::align_val_t{64});
            std::memset(bytes, 0, sizeof(PS2Runtime));
            g_shell = static_cast<PS2Runtime *>(bytes);
            g_shell->m_eeClock.setGate(~0ull);
            g_shell->m_hostStackFloor = ~uintptr_t(0); // every call and jump to the hooks
            g_shell->m_directDispatch = true;
            g_shell->memory().bindWorkerShell(rdram, g_own->scratchpad, g_own->vu0Data);
            g_runtime = runtime;
            g_rdram = rdram;
            ps2_worker::g_hooks.call = hookCall;
            ps2_worker::g_hooks.jump = hookJump;
            ps2_worker::g_hooks.fault = hookFault;
            ps2_worker::g_real = runtime;
            ps2_worker::g_shell = g_shell;
#if defined(__vita__)
            const unsigned int stackDefault = _pthread_stack_default_user;
            _pthread_stack_default_user = kHostStack;
#endif
            std::thread(loop).detach();
#if defined(__vita__)
            _pthread_stack_default_user = stackDefault;
#endif
            std::fprintf(stderr, "[worker91] started: guest stack %08x-%08x, shell %u bytes\n", g_stackTop - kGuestStack, g_stackTop,
                         static_cast<unsigned>(sizeof(PS2Runtime)));
            return true;
        }

        // ---- the game thread's joins at the module's entries -------------------
        PS2Runtime::RecompiledFunction g_orig[kModuleCount] = {};
        bool g_gated = false;     // round 95: the module entries are wrapped (install ran)
        bool g_walked = false;    // round 95: the display walk ran this frame (noteWalk; cleared at the frame's end)
        int g_lastJoinSite = -1;  // round 95: unused bookkeeping
        uint64_t g_frameJoins = 0;
        uint32_t g_skipEntry = 0; // round 95 (early95): the game thread's next call of this entry returns at once
        template <int I>
        void gate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (g_skipEntry == kModuleEntries[I] && !t_onWorker)
            {
                g_skipEntry = 0;
                ctx->pc = GPR_U32(ctx, 31);
                return;
            }
            if (g_busy.load(std::memory_order_relaxed) && !t_onWorker)
            {
                if (kModuleEntries[I] == kSetBlendRatio &&
                    afterJobF(kSetBlendRatio, GPR_U32(ctx, 31), GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), ctx->f[12]))
                {
                    SET_GPR_S32(ctx, 2, 1);
                    ctx->pc = GPR_U32(ctx, 31);
                    return;
                }
                if (kModuleEntries[I] == kDispParticleManager &&
                    afterJobF(kDispParticleManager, GPR_U32(ctx, 31), GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), ctx->f[12]))
                {
                    // disp_particle_manager (the particles' screen matrix,
                    // from the camera context) after the job, on the worker.
                    ctx->pc = GPR_U32(ctx, 31);
                    return;
                }
                ++g_moduleJoins[kModuleEntries[I]];
                join(kSiteModule);
            }
            g_orig[I](rdram, ctx, runtime);
        }
        template <int... I>
        void installGates(PS2Runtime &runtime, std::integer_sequence<int, I...>)
        {
            ((runtime.hasFunction(kModuleEntries[I]) ? (g_orig[I] = runtime.lookupFunction(kModuleEntries[I]), runtime.registerFunction(kModuleEntries[I], gate<I>), 0) : 0), ...);
        }
        // The module loader (code and data move).
        constexpr uint32_t kLoaderSites[] = {0x001004B8u, 0x00100278u, 0x001B4C20u};
        PS2Runtime::RecompiledFunction g_loaderOrig[3] = {};
        template <int I>
        void loaderGate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            join(kSiteMove);
            g_loaderOrig[I](rdram, ctx, runtime);
        }
        void (*g_prevBeforeMove)() = nullptr;
        void beforeMove()
        {
            join(kSiteMove);
            if (g_prevBeforeMove)
                g_prevBeforeMove();
        }
        bool wanted()
        {
            // Off with the tools that wrap the table or keep host state per
            // call (the profiler, the difftest, the call statistics): the
            // worker would run their wrappers beside the game thread.
            static const int v = [] {
                const char *e = std::getenv("SOTC_WORKER91");
                if (std::getenv("SOTC_SUBSYS_PROF") || std::getenv("SOTC_DIFFTEST") || std::getenv("SOTC_CALLGUEST_STATS") || std::getenv("SOTC_TRACE_CALLS"))
                    return 0;
                return e ? std::atoi(e) : 1;
            }();
            return v != 0;
        }
    }

    bool onWorker() { return t_onWorker; }
    void skipEntryOnce(uint32_t entry) { g_skipEntry = entry; }
    bool gated() { return g_gated; }
    bool skipPending() { return g_skipEntry != 0u; }
    void noteWalk() { g_walked = true; }
    bool walked() { return g_walked; }
    bool busy() { return g_busy.load(std::memory_order_relaxed); }
    bool done()
    {
        if (!g_busy.load(std::memory_order_relaxed) || t_onWorker)
            return false;
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_phase == kDone;
    }
    uint32_t stackTop() { return g_stackTop; }

    bool kick(uint8_t *rdram, PS2Runtime *runtime, const R5900Context &from, Job job, void *arg)
    {
        return kickAs(rdram, runtime, from, job, arg, nullptr, nullptr, nullptr);
    }
    R5900Context *ownContext() { return g_own ? &g_own->ctx : nullptr; }
    uint8_t *ownScratchpad() { return g_own ? g_own->scratchpad : nullptr; }
    uint8_t *ownVu0Data() { return g_own ? g_own->vu0Data : nullptr; }
    bool parked()
    {
        if (!g_busy.load(std::memory_order_relaxed) || t_onWorker)
            return false;
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_parked;
    }
    bool lastJobParked() { return g_jobParked; }
    void setCallCensus(bool on) { g_census = on; }
    void recompiledNext() { g_nextRecomp = true; }
    void callCensusReport(const char *tag)
    {
        std::vector<std::pair<uint64_t, uint32_t>> v;
        for (const auto &[at, c] : g_calls)
            v.push_back({c, at});
        std::sort(v.rbegin(), v.rend());
        std::fprintf(stderr, "[%s] %zu call targets on the worker:", tag, v.size());
        for (const auto &[c, at] : v)
            std::fprintf(stderr, " %08x x%llu%s", at, (unsigned long long)c, safe(at) ? "" : "!");
        std::fprintf(stderr, "\n");
        g_calls.clear();
    }

    bool kickAs(uint8_t *rdram, PS2Runtime *runtime, const R5900Context &from, Job job, void *arg, Admit admitFn, void (*setup)(void *), void (*joined)(void *))
    {
        if (t_onWorker || !wanted() || ps2_worker::isShell(runtime) || !start(rdram, runtime))
        {
            ++g_refused;
            return false;
        }
        join(kSiteKick);
        g_admitFn = admitFn;
        g_recompOnly = g_nextRecomp;
        g_nextRecomp = false;
        g_joinedFn = joined;
        g_joinedArg = arg;
        // Its context: the game thread's, sp on its own stack; VU0's R its own stream.
        const __m128 r = g_own->ctx.vu0_r;
        g_own->ctx = from;
        if (g_rSeeded)
            g_own->ctx.vu0_r = r;
        g_rSeeded = true;
        SET_GPR_U32(&g_own->ctx, 29, g_stackTop - 0x40u);
        SET_GPR_U32(&g_own->ctx, 31, 0u);
        std::memcpy(g_own->vu0Data, runtime->memory().getVU0Data(), PS2_VU0_DATA_SIZE);
        g_eeThread = runtime->eeScheduler().currentThreadId();
        if (setup)
            setup(arg);
        g_answers.swap(g_pending);
        g_pending.clear();
        g_answerTargets[0] = g_answers.empty() ? 0u : g_answers.front().target;
        g_answerTargets[1] = g_answerTargets[0];
        for (const Answer &a : g_answers)
            if (a.target != g_answerTargets[0])
            {
                g_answerTargets[1] = a.target;
                break;
            }
        g_kickUs = nowUs();
        g_jobParked = false;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_job = job;
            g_arg = arg;
            g_exclusive = false;
            g_phase = kRunning;
        }
        g_busy.store(true, std::memory_order_relaxed);
        g_cvWork.notify_one();
        ++g_jobs;
        return true;
    }

    void join(Site site)
    {
        if (!g_busy.load(std::memory_order_relaxed) || t_onWorker)
            return;
        const uint64_t t0 = nowUs();
        bool waited = false;
        {
            std::unique_lock<std::mutex> lock(g_mutex);
            g_exclusive = true;
            if (g_parked)
                g_cvPark.notify_one();
            if (g_phase != kDone)
            {
                waited = true;
                g_cvDone.wait(lock, [] { return g_phase == kDone; });
            }
            g_phase = kIdle;
        }
        g_busy.store(false, std::memory_order_relaxed);
        const uint64_t wait = nowUs() - t0;
        if (g_joinedFn)
        {
            void (*fn)(void *) = g_joinedFn;
            g_joinedFn = nullptr;
            fn(g_joinedArg);
        }
        ++g_siteJoins[site];
        g_siteWaitUs[site] += wait;
        g_joinWaits += waited;
        g_joinWaitUs += wait;
        g_jobUsSum += g_jobUs;
        g_startLatUs += g_jobStartUs > g_kickUs ? g_jobStartUs - g_kickUs : 0u; // round 95: the hand-over's wake-up
        if (g_jobParked)
        {
            ++g_parks;
            g_parkUsSum += g_parkAtUs;
            ++g_parkCauses[g_parkTarget];
        }
    }

    uint32_t prepareOut(uint32_t n)
    {
        if (!g_started || g_failed || g_stackTop == 0u || (n + 1u) * 16u > kAnswerBytes || g_busy.load(std::memory_order_relaxed))
            return 0u;
        return g_stackTop - kGuestStack + n * 16u;
    }
    void prepare(uint32_t target, uint32_t key, uint32_t out) { g_pending.push_back({target, key, out}); }

    bool deferCall(uint32_t target, uint32_t then, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t ra)
    {
        if (!t_onWorker || t_exclusive)
            return false;
        g_kept.push_back({target, then, {a0, a1, a2, a3}, ra});
        return true;
    }

    namespace
    {
        bool afterJobF(uint32_t target, uint32_t ra, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, float f12)
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            if (g_phase != kRunning)
                return false;
            g_after.push_back({target, ra, {a0, a1, a2, a3}, f12});
            ++g_afterCalls;
            return true;
        }
    }
    bool callAfterJob(uint32_t target, uint32_t ra, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_phase != kRunning)
            return false;
        g_after.push_back({target, ra, {a0, a1, a2, a3}, 0.0f});
        ++g_afterCalls;
        return true;
    }

    void install(PS2Runtime &runtime)
    {
        if (!wanted())
            return;
        installGates(runtime, std::make_integer_sequence<int, kModuleCount>{});
        g_gated = runtime.hasFunction(kModuleEntries[0]);
        for (int i = 0; i < 3; ++i)
            if (runtime.hasFunction(kLoaderSites[i]))
            {
                g_loaderOrig[i] = runtime.lookupFunction(kLoaderSites[i]);
                runtime.registerFunction(kLoaderSites[i], i == 0 ? loaderGate<0> : i == 1 ? loaderGate<1> : loaderGate<2>);
            }
        g_prevBeforeMove = sotc_decomp::g_beforeGuestMove;
        sotc_decomp::g_beforeGuestMove = beforeMove;
    }

    void frameTick(uint64_t frame)
    {
        join(kSiteFrame);
        g_frameJoins += g_lastJoinSite == kSiteFrame ? 1u : 0u;
        g_walked = false;
        if (++g_frames % 600u != 0u || g_jobs == 0u)
            return;
        const double n = static_cast<double>(g_jobs);
        std::fprintf(stderr, "[worker91] frame %llu: %llu jobs (%llu refused), start %.3f ms after the hand-over, job %.3f ms on the worker, parked in %.0f%% (after %.3f ms), "
                             "%llu calls after the job, %llu prepared answers used, %llu calls kept for the end; joins waited %llu (%.3f ms a job):",
                     (unsigned long long)frame, (unsigned long long)g_jobs, (unsigned long long)g_refused, g_startLatUs / n / 1000.0, g_jobUsSum / n / 1000.0, 100.0 * g_parks / n,
                     g_parks ? g_parkUsSum / static_cast<double>(g_parks) / 1000.0 : 0.0, (unsigned long long)g_afterCalls, (unsigned long long)g_answered, (unsigned long long)g_keptCalls,
                     (unsigned long long)g_joinWaits, g_joinWaitUs / n / 1000.0);
        for (int s = 0; s < kSiteCount; ++s)
            if (g_siteJoins[s])
                std::fprintf(stderr, " %s %llu (%.3f ms)", kSiteNames[s], (unsigned long long)g_siteJoins[s], g_siteWaitUs[s] / 1000.0 / g_siteJoins[s]);
        std::vector<std::pair<uint64_t, uint32_t>> causes;
        for (const auto &[at, c] : g_parkCauses)
            causes.push_back({c, at});
        std::sort(causes.rbegin(), causes.rend());
        std::fprintf(stderr, " | parked at");
        for (size_t i = 0; i < causes.size() && i < 6u; ++i)
            std::fprintf(stderr, " %08x x%llu", causes[i].second, (unsigned long long)causes[i].first);
        causes.clear();
        for (const auto &[at, c] : g_moduleJoins)
            causes.push_back({c, at});
        std::sort(causes.rbegin(), causes.rend());
        std::fprintf(stderr, " | module joins at");
        for (size_t i = 0; i < causes.size() && i < 6u; ++i)
            std::fprintf(stderr, " %08x x%llu", causes[i].second, (unsigned long long)causes[i].first);
        std::fprintf(stderr, "\n");
        g_startLatUs = 0;
        g_jobs = g_refused = g_parks = g_joinWaits = g_joinWaitUs = g_jobUsSum = g_parkUsSum = g_afterCalls = g_answered = g_keptCalls = 0;
        for (int s = 0; s < kSiteCount; ++s)
            g_siteJoins[s] = g_siteWaitUs[s] = 0;
        g_parkCauses.clear();
        g_moduleJoins.clear();
    }
}
