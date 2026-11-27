// Round Y2 (SOTC_Y2=1 with d1native; y2_cloth.h, docs/decomp_y2.md): getClothDynamics (0x11399e0) as a host driver.
// Included at the end of dc2_chara.cpp (D1's flip / step / velocity cores, DC2's typed views and Snap).
//
// The D1 driver (entryDynamicsD1) is the old body's shape: the frame saved, s0-s3 / f20 / f21 set, then the step, the
// whole-set callback and the velocities as guest calls through the table, each a context hand-over (the step's adapter
// reads ~10 VU0 registers and writes ~25 registers back; the strand solver's lifted body loads and stores the whole
// register file around each of its five calls). Y2's driver:
//   - the flip (D1's core) and the step (clothStepD1Core: the function the table's step runs) as C++ calls, VU0's R in
//     and out of the context only;
//   - the whole-set callback: Wander's mantle (the strand solver, gamecore_sub_013B8DC8) as y2::strands, a C++ call;
//     any other callback (the reins' collisions, the fringe, the horse's cloths: natives of their own rounds), the
//     constraint (callback 0) or a strand set outside y2::strands' domain: the guest call, from the context the D1
//     driver has at that point (its prologue, then the step's registers as entryStepD1 writes them);
//   - the velocities (D1's core) as a C++ call; a type-1 cloth's (its layout objects: guest calls): the guest call.
// Without a guest call the context is not touched except VU0's R (the step's draws), v0 = 1 and pc; f12 / f13 stay
// dt / 1 / dt as the D1 driver leaves them. Not written then (no reader: the callers are compiled code that reloads):
// the frame's saves below sp, f0, fcr31's condition bit, the step's scratch registers.
//
// SOTC_Y2_CHECK=1 (game thread, from game frame SOTC_Y2_CHECK_FROM): each segment (flip + step, the strand solver, the
// velocities) is run Y2's way on its regions (the cloth's row records, flip byte and every row buffer; for the strands
// the rows' position buffers and the global anchor vector 0x14669f0), Y2's results kept, the regions put back, and the
// old segment run as the D1 driver runs it (a guest call through the table from the D1 context; for the strand solver
// the table entry with Y2 off for that call: y2::g_ref), then compared byte for byte (and R after the step); the old
// results stay. At the exit the callee-saved registers (s0-s7, sp, fp, ra, f20-f31) are compared with the entry's and
// v0 with 1. SOTC_Y2_RAMCHECK=<n>: every n-th checked segment, a hash of every 4 KB page of guest RAM before and after
// Y2's run: a changed page that holds no byte of the regions is counted as written outside them ([y2chk] outside).
namespace dc2
{
    namespace y2d
    {
        // The D1 driver's context at its guest calls: its prologue (once).
        struct Guest
        {
            bool open = false;
            uint32_t sp = 0;
            uint64_t ra = 0;
        };
        inline void prologue(uint8_t *rdram, R5900Context *ctx, Guest &g, uint64_t cloth64, uint64_t user, uint64_t callback, uint64_t word, float dt,
                             float invDt)
        {
            if (g.open)
                return;
            g.open = true;
            const d1::Mem m{rdram};
            float *f = ctx->f;
            m.set<uint32_t>(g.sp + 0x28u, d1b::d1bits(f[20]));
            m.set<uint32_t>(g.sp + 0x30u, d1b::d1bits(f[21]));
            m.set<uint64_t>(g.sp + 0u, GPR_U64(ctx, 16));
            m.set<uint64_t>(g.sp + 8u, GPR_U64(ctx, 17));
            m.set<uint64_t>(g.sp + 0x10u, GPR_U64(ctx, 18));
            m.set<uint64_t>(g.sp + 0x18u, GPR_U64(ctx, 19));
            m.set<uint64_t>(g.sp + 0x20u, g.ra);
            f[0] = 0.0f;
            f[20] = dt;
            f[21] = invDt;
            ctx->fcr31 &= ~0x800000u; // c.eq.s dt, 0 (dt != 0 here)
            SET_GPR_U64(ctx, 16, cloth64);
            SET_GPR_U64(ctx, 17, user);
            SET_GPR_U64(ctx, 18, callback);
            SET_GPR_U64(ctx, 19, word);
        }
        // The registers entryStepD1 leaves (as after the step's guest call).
        inline void stepRegisters(R5900Context *ctx, const d1b::StepRegs &s)
        {
            SET_GPR_U32(ctx, 9, d1b::d1bits(s.f2a));
            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
            SET_GPR_U32(ctx, 7, d1b::d1bits(s.f3));
            SET_GPR_U32(ctx, 8, d1b::d1bits(s.f4));
            ctx->vu0_vf[5] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            SET_GPR_U32(ctx, 3, d1b::d1bits(s.f2));
            ctx->vu0_vf[3] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
            SET_GPR_U32(ctx, 4, d1b::d1bits(s.f0));
            ctx->vu0_vf[4] = _mm_castsi128_ps(GPR_VEC(ctx, 4));
            ctx->f[0] = s.f0;
            ctx->f[1] = s.f1;
            ctx->f[2] = s.f2;
            ctx->f[3] = s.f3;
            ctx->f[4] = s.f4;
            ctx->f[12] = s.f12;
            ctx->vu0_vf[2] = s.vf2;
            ctx->vu0_vf[10] = s.vf10;
            ctx->vu0_vf[11] = s.vf11;
            ctx->vu0_vf[12] = s.vf12;
            ctx->vu0_vf[13] = s.vf13;
            ctx->vu0_vf[18] = s.vf18;
            ctx->vu0_vf[20] = s.vf20;
            ctx->vu0_vf[21] = s.vf21;
            ctx->vu0_vf[22] = s.vf22;
            ctx->vu0_acc = s.acc;
            if (s.rTouched)
                setR(ctx, s.r);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(s.v0));
        }
        // entryStepD1's inputs from the context (f12: dt, as the D1 driver sets it before the call).
        inline void stepRegsFrom(const R5900Context *ctx, d1b::StepRegs &s, float dt)
        {
            s.vf10 = ctx->vu0_vf[10];
            s.vf11 = ctx->vu0_vf[11];
            s.vf12 = ctx->vu0_vf[12];
            s.vf13 = ctx->vu0_vf[13];
            s.vf18 = ctx->vu0_vf[18];
            s.vf20 = ctx->vu0_vf[20];
            s.vf21 = ctx->vu0_vf[21];
            s.vf22 = ctx->vu0_vf[22];
            s.acc = ctx->vu0_acc;
            s.f12 = dt;
            std::memcpy(&s.r, &ctx->vu0_r, 4);
            s.rTouched = false;
        }

        // ---- the check -------------------------------------------------------------------------------------------
        int g_ramEvery = -1;
        uint64_t g_ramCount = 0;
        std::vector<uint64_t> g_pageHash;
        inline uint64_t pageHash(const uint8_t *p)
        {
            uint64_t h = 0x9E3779B97F4A7C15ull;
            for (int i = 0; i < 4096; i += 8)
            {
                uint64_t w;
                std::memcpy(&w, p + i, 8);
                h = (h ^ w) * 0x100000001B3ull;
            }
            return h;
        }
        // Whole-RAM page hashes around a Y2 segment (every g_ramEvery-th checked segment).
        struct RamScan
        {
            bool on = false;
            void before(const uint8_t *ram)
            {
                if (g_ramEvery < 0)
                {
                    const char *e = std::getenv("SOTC_Y2_RAMCHECK");
                    g_ramEvery = e ? std::atoi(e) : 0;
                }
                on = g_ramEvery > 0 && (g_ramCount++ % static_cast<uint64_t>(g_ramEvery)) == 0u;
                if (!on)
                    return;
                g_pageHash.resize(PS2_RAM_SIZE / 4096u);
                for (uint32_t i = 0; i < PS2_RAM_SIZE / 4096u; ++i)
                    g_pageHash[i] = pageHash(ram + i * 4096u);
            }
            // The bytes of changed pages that hold no byte of the regions.
            uint64_t after(const uint8_t *ram, const Snap &snap)
            {
                if (!on)
                    return 0u;
                ++y2::g_st.ramChecks;
                uint64_t outside = 0;
                for (uint32_t i = 0; i < PS2_RAM_SIZE / 4096u; ++i)
                {
                    if (pageHash(ram + i * 4096u) == g_pageHash[i])
                        continue;
                    const uint32_t lo = i * 4096u, hi = lo + 4096u;
                    bool mine = false;
                    for (const auto &x : snap.regions)
                        if (x.first < hi && x.first + x.second > lo)
                            mine = true;
                    if (!mine)
                    {
                        outside += 4096u;
                        if (outside <= 4096u * 4u)
                            std::fprintf(stderr, "[y2chk] frame %llu page %08x changed outside the regions\n", (unsigned long long)sotc_decomp::g_gameFrame,
                                         (unsigned)lo);
                    }
                }
                return outside;
            }
        };
        Snap g_y2snap;
        __attribute__((noinline)) void noteY2(int seg, bool same, const char *what, uint32_t a)
        {
            static const char *const kSeg[4] = {"flip+step", "strands", "velocity", "registers"};
            ++y2::g_st.checks[seg];
            if (!same)
            {
                ++y2::g_st.differ[seg];
                if (y2::g_st.differ[seg] <= 20u)
                    std::fprintf(stderr, "[y2chk] frame %llu %s differs (%s) at %08x\n", (unsigned long long)sotc_decomp::g_gameFrame, kSeg[seg], what,
                                 (unsigned)a);
            }
        }
        // A segment the check's way: regions saved, Y2's run (mine) kept, the regions put back, the old run, compared.
        template <class Regions, class Mine, class Old>
        bool segment(int seg, uint8_t *rdram, Regions regions, Mine mine, Old old)
        {
            Snap &sn = g_y2snap;
            sn.clear();
            regions(sn);
            sn.save(rdram);
            RamScan scan;
            scan.before(rdram);
            mine();
            y2::g_st.outside[seg] += scan.after(rdram, sn);
            sn.keepMine(rdram);
            sn.restore(rdram);
            old();
            const bool same = sn.sameAsMine(rdram);
            static uint64_t reports = 0;
            if (!same && reports < 24u)
            {
                // The first differing qwords (Y2's, then the old run's), the region and its offset.
                size_t o = 0;
                int shown = 0;
                for (const auto &x : sn.regions)
                {
                    for (uint32_t i = 0; i < x.second && shown < 3; i += 16u)
                    {
                        const uint32_t n = x.second - i < 16u ? x.second - i : 16u;
                        if (std::memcmp(rdram + x.first + i, sn.mine.data() + o + i, n) == 0)
                            continue;
                        uint32_t a[4] = {}, b[4] = {};
                        std::memcpy(a, sn.mine.data() + o + i, n);
                        std::memcpy(b, rdram + x.first + i, n);
                        float fa[4], fb[4];
                        std::memcpy(fa, a, 16);
                        std::memcpy(fb, b, 16);
                        std::fprintf(stderr,
                                     "[y2diff] frame %llu seg %d region %08x+%x (%u bytes): y2 %08x %08x %08x %08x (%.6g %.6g %.6g %.6g) old %08x %08x %08x %08x "
                                     "(%.6g %.6g %.6g %.6g)\n",
                                     (unsigned long long)sotc_decomp::g_gameFrame, seg, (unsigned)x.first, (unsigned)i, (unsigned)x.second, a[0], a[1], a[2],
                                     a[3], fa[0], fa[1], fa[2], fa[3], b[0], b[1], b[2], b[3], fb[0], fb[1], fb[2], fb[3]);
                        ++shown;
                    }
                    o += x.second;
                }
                ++reports;
            }
            return same;
        }
    } // namespace y2d

    void entryDynamicsY2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x40u, cloth = GPR_U32(ctx, 4);
        const float dt = ctx->f[12], invDt = ctx->f[13];
        y2::tick();
        z2::tick();
        if (on() || !sotc_decomp::hotList47On() || !sotc_decomp::fastNativeOn() || sp >= PS2_RAM_SIZE - 0x40u || cloth >= PS2_RAM_SIZE - 0x100u ||
            s8cloth::g_open || ps2_worker::isShell(runtime))
        {
            ++y2::g_st.dynOld;
            return entryDynamicsD1(rdram, ctx, runtime);
        }
        if (d1::eq(dt, 0.0f)) // nothing moves: the D1 driver's short path
        {
            ++y2::g_st.dynZero;
            return entryDynamicsD1(rdram, ctx, runtime);
        }
        const d1::Mem m{rdram};
        const uint32_t grid = m.u32(cloth);
        if (grid >= PS2_RAM_SIZE - 0x30u)
        {
            ++y2::g_st.dynOld;
            return entryDynamicsD1(rdram, ctx, runtime);
        }
        const int32_t rows = m.s32(grid + 4u), cols = m.s32(grid + 8u);
        const bool type1 = m.get<int16_t>(grid) == 1;
        ++y2::g_st.dyn;
        const uint64_t cloth64 = GPR_U64(ctx, 4), user = GPR_U64(ctx, 5), callback = GPR_U64(ctx, 7), word = GPR_U64(ctx, 8);
        const uint32_t owner = static_cast<uint32_t>(user), params = static_cast<uint32_t>(word);
        const bool full = GPR_U64(ctx, 6) != 0u;
        const bool chk = y2::checking() && !y2::g_ref;
        const bool z2on = z2::on();                  // round Z2 (z2_dynamics.inc)
        const bool z2chk = z2on && z2::checking();
        const bool regChk = chk || z2chk;
        // The velocities' core walks counts the D1 adapter accepts; else the guest call (the D1 entry decides).
        const bool velocityHost = !type1 && rows >= 0 && rows <= 4096 && cols >= 0 && cols <= 4096;
        // The entry's callee-saved registers (the check's last compare).
        uint64_t saved[11] = {};
        uint32_t savedF[12] = {};
        if (__builtin_expect(regChk, 0))
        {
            for (int i = 0; i < 8; ++i)
                saved[i] = GPR_U64(ctx, 16 + i);
            saved[8] = entrySp;
            saved[9] = GPR_U64(ctx, 30);
            saved[10] = ra;
            std::memcpy(savedF, &ctx->f[20], sizeof(savedF));
        }
        y2d::Guest g;
        g.sp = sp;
        g.ra = ra;
        auto open = [&]() { y2d::prologue(rdram, ctx, g, cloth64, user, callback, word, dt, invDt); };
        if (__builtin_expect(chk, 0))
            open(); // the old segments are guest calls from the D1 driver's context
        pf141::noteCloth(rdram, cloth); // round 141 (prefetch only)
        // The cloth's regions: the row records, the flip byte, every row buffer.
        auto regionsCloth = [&](Snap &sn)
        {
            sn.add(cloth + 0x80u, 1u);
            const uint32_t recs = m.u32(cloth + 0xCu);
            if (rows <= 0 || rows > 4096 || cols <= 0 || cols > 4096 || !inRam(recs, static_cast<uint64_t>(rows) * 0x18u))
                return;
            sn.add(recs, static_cast<uint32_t>(rows) * 0x18u);
            const uint32_t bytes = static_cast<uint32_t>(cols) * 16u;
            for (int32_t i = 0; i < rows; ++i)
                for (uint32_t k = 0; k < 6u; ++k)
                    if (k != 3u)
                        sn.add(m.u32(recs + static_cast<uint32_t>(i) * 0x18u + k * 4u) & ~0xFu, bytes);
        };

        // ---- the flip and the step ------------------------------------------------------------------------------
        d1b::StepRegs s;
        y2d::stepRegsFrom(ctx, s, dt);
        auto flipStep = [&]()
        {
            const sotc_p2::Scope bracket(y2::bracketStep());
            d1b::clothFlipD1Core(m, cloth);
            if (r1chara::on())
                d1b::clothStepD1Core<true>(m, cloth, owner, full, s);
            else
                d1b::clothStepD1Core<false>(m, cloth, owner, full, s);
        };
        if (__builtin_expect(chk, 0))
        {
            const uint32_t r0 = rOf(ctx);
            uint32_t rMine = r0;
            const bool same = y2d::segment(
                0, rdram, regionsCloth,
                [&]()
                {
                    flipStep();
                    rMine = s.rTouched ? s.r : r0;
                },
                [&]()
                {
                    d1b::clothFlipD1Core(m, cloth);
                    SET_GPR_U64(ctx, 4, cloth64);
                    SET_GPR_U64(ctx, 5, user);
                    ctx->f[12] = dt;
                    ctx->f[13] = invDt;
                    sotc_decomp::callGuest(rdram, ctx, runtime, 0x011370E8u, 0x01139AC0u, sp);
                });
            y2d::noteY2(0, same, "memory", cloth);
            if (rOf(ctx) != rMine)
                y2d::noteY2(0, false, "R", cloth);
        }
        else
        {
            flipStep();
            if (s.rTouched)
                setR(ctx, s.r);
        }

        // ---- the whole-set callback -----------------------------------------------------------------------------
        auto openStep = [&]()
        {
            if (!g.open)
            {
                open();
                y2d::stepRegisters(ctx, s);
            }
        };
        auto guestCallback = [&]()
        {
            openStep();
            SET_GPR_U64(ctx, 6, word);
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            if (callback != 0u)
                sotc_decomp::callGuest(rdram, ctx, runtime, static_cast<uint32_t>(callback), 0x01139AD4u, sp); // jalr s2 (cloth, user, word)
            else
            {
                SET_GPR_U64(ctx, 6, 0u);
                sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137858u, 0x01139AF0u, sp);
            }
        };
        if (callback == y2::kStrands)
        {
            if (__builtin_expect(chk, 0))
            {
                bool ran = false;
                const bool same = y2d::segment(
                    1, rdram,
                    [&](Snap &sn)
                    {
                        y2::strandsRegions(rdram, cloth,
                                           [&](uint32_t a, uint32_t n)
                                           {
                                               if (inRam(a, n))
                                                   sn.add(a, n);
                                           });
                    },
                    [&]() { ran = y2::strands(rdram, runtime, cloth, owner, params); },
                    [&]()
                    {
                        y2::g_ref = true;
                        guestCallback();
                        y2::g_ref = false;
                    });
                static int dumps = 0;
                if (ran && !same && dumps < 3)
                {
                    // Y2's frame values next to the old body's frame (dead stack, still as it left it).
                    ++dumps;
                    const uint32_t fsp = sp - 0x2B0u;
                    auto dump = [&](const char *name, const float *mine, uint32_t off, int n)
                    {
                        std::fprintf(stderr, "[y2dbg] frame %llu %s:", (unsigned long long)sotc_decomp::g_gameFrame, name);
                        for (int i = 0; i < n; ++i)
                        {
                            uint32_t a, b;
                            std::memcpy(&a, mine + i, 4);
                            b = m.u32(fsp + off + 4u * i);
                            std::fprintf(stderr, " %08x%s%08x", a, a == b ? "=" : "/", b);
                        }
                        std::fprintf(stderr, "\n");
                    };
                    dump("apos2", y2::g_dbg.apos2, 0x0u, 4);
                    dump("apos1", y2::g_dbg.apos1, 0x10u, 4);
                    dump("M1", y2::g_dbg.m1, 0xA0u, 16);
                    dump("M2", y2::g_dbg.m2, 0xE0u, 16);
                    std::fprintf(stderr, "[y2dbg] third %d f24 %08x\n", y2::g_dbg.third ? 1 : 0, d1b::d1bits(y2::g_dbg.f24));
                    if (y2::g_dbg.third)
                    {
                        dump("q", y2::g_dbg.q, 0x1B0u, 4);
                        dump("M3", y2::g_dbg.m3, 0x20u, 16);
                    }
                }
                if (ran)
                    y2d::noteY2(1, same, "memory", cloth);
                else
                    ++y2::g_st.strandsOld; // the old body both ways: not a check
            }
            else if (!y2::strands(rdram, runtime, cloth, owner, params))
            {
                ++y2::g_st.strandsOld;
                guestCallback();
            }
        }
        else if (z2::Kind kind = z2::kThree; z2on && z2d::kindOf(callback, kind))
        {
            // Round Z2: the callback as a C++ call; a guest call inside it (a fallback) from the D1 driver's context.
            ++z2::g_st.calls[kind];
            auto guest = [&](uint32_t target, uint32_t a0, uint32_t a1, uint32_t ra, uint32_t csp, uint32_t a2 = 0u) -> uint32_t
            {
                openStep();
                ++z2::g_st.guestCalls[kind];
                SET_GPR_U64(ctx, 4, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a0))));
                SET_GPR_U64(ctx, 5, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a1))));
                SET_GPR_U64(ctx, 6, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a2))));
                sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, csp);
                return GPR_U32(ctx, 2);
            };
            auto mine = [&]() { return z2d::callback(rdram, runtime, kind, cloth, owner, params, sp, guest); };
            auto oldLeg = [&]()
            {
                if (!z2::recompRef())
                    return guestCallback();
                openStep();
                SET_GPR_U64(ctx, 6, word);
                SET_GPR_U64(ctx, 4, cloth64);
                SET_GPR_U64(ctx, 5, user);
                SET_GPR_U32(ctx, 31, 0x01139AD4u);
                SET_GPR_U32(ctx, 29, sp);
                z2::runRecompiled(rdram, ctx, runtime, static_cast<uint32_t>(callback));
            };
            if (__builtin_expect(z2chk, 0) && z2::checkNow(kind, runtime))
            {
                openStep(); // both legs from the old leg's context
                const EeScheduler::ThrowScope noReturn(runtime->eeScheduler());
                struct Close
                {
                    ~Close() { z2::abandon(); }
                } close;
                if (z2::begin(kind, rdram, ctx, runtime))
                {
                    const bool ran = mine();
                    z2::afterMine(rdram, ctx, runtime);
                    oldLeg();
                    if (ran)
                    {
                        ++z2::g_st.host[kind];
                        z2::finish(kind, rdram, runtime, sp, cloth);
                    }
                    else
                        ++z2::g_st.fallback[kind];
                }
                else if (mine())
                    ++z2::g_st.host[kind];
                else
                {
                    ++z2::g_st.fallback[kind];
                    guestCallback();
                }
            }
            else if (mine())
                ++z2::g_st.host[kind];
            else
            {
                ++z2::g_st.fallback[kind];
                guestCallback();
            }
        }
        else
        {
            ++y2::g_st.guestCallback;
            if (z2on)
                ++z2::g_st.otherCallback;
            guestCallback();
        }

        // ---- the velocities -------------------------------------------------------------------------------------
        auto guestVelocity = [&]()
        {
            open();
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            ctx->f[12] = dt;
            ctx->f[13] = invDt;
            sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137640u, 0x01139B04u, sp);
        };
        auto velocity = [&]()
        {
            const sotc_p2::Scope bracket(y2::bracketVelocity());
            __m128 vf5, vf6;
            d1b::velocityD1Core(m, cloth, rows, cols, vdupq_n_f32(invDt), vf5, vf6);
        };
        if (!velocityHost && type1 && z2on && z2::partOn(z2::kType1))
        {
            // Round Z2: a type-1 cloth's velocities as a C++ call (isysGroupForFirst / Exit and any fallback: guest calls
            // from the old body's frame, the D1 driver's context).
            ++z2::g_st.calls[z2::kType1];
            auto guest = [&](uint32_t target, uint32_t a0, uint32_t a1, uint32_t ra, uint32_t csp, uint32_t a2 = 0u) -> uint32_t
            {
                open();
                ++z2::g_st.guestCalls[z2::kType1];
                SET_GPR_U64(ctx, 4, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a0))));
                SET_GPR_U64(ctx, 5, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a1))));
                SET_GPR_U64(ctx, 6, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a2))));
                sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, csp);
                return GPR_U32(ctx, 2);
            };
            auto mine = [&]()
            {
                const sotc_p2::Scope bracket(z2::bracket(z2::kType1));
                return z2d::type1(rdram, ctx, runtime, cloth, rows, cols, invDt, sp, guest);
            };
            auto oldLeg = [&]()
            {
                if (!z2::recompRef())
                    return guestVelocity();
                open();
                SET_GPR_U64(ctx, 4, cloth64);
                SET_GPR_U64(ctx, 5, user);
                ctx->f[12] = dt;
                ctx->f[13] = invDt;
                SET_GPR_U32(ctx, 31, 0x01139B04u);
                SET_GPR_U32(ctx, 29, sp);
                z2::runRecompiled(rdram, ctx, runtime, 0x01137640u);
            };
            if (__builtin_expect(z2chk, 0) && z2::checkNow(z2::kType1, runtime))
            {
                open();
                const EeScheduler::ThrowScope noReturn(runtime->eeScheduler());
                struct Close
                {
                    ~Close() { z2::abandon(); }
                } close;
                if (z2::begin(z2::kType1, rdram, ctx, runtime))
                {
                    const bool ran = mine();
                    z2::afterMine(rdram, ctx, runtime);
                    oldLeg();
                    if (ran)
                    {
                        ++z2::g_st.host[z2::kType1];
                        z2::finish(z2::kType1, rdram, runtime, sp, cloth);
                    }
                    else
                        ++z2::g_st.fallback[z2::kType1];
                }
                else if (mine())
                    ++z2::g_st.host[z2::kType1];
                else
                {
                    ++z2::g_st.fallback[z2::kType1];
                    guestVelocity();
                }
            }
            else if (mine())
                ++z2::g_st.host[z2::kType1];
            else
            {
                ++z2::g_st.fallback[z2::kType1];
                guestVelocity();
            }
        }
        else if (!velocityHost)
        {
            ++y2::g_st.type1Velocity;
            guestVelocity();
        }
        else if (__builtin_expect(chk, 0))
            y2d::noteY2(2, y2d::segment(2, rdram, regionsCloth, velocity, guestVelocity), "memory", cloth);
        else
            velocity();

        // ---- the exit -------------------------------------------------------------------------------------------
        if (g.open)
        {
            // The D1 driver's epilogue (s0-s3, f20 / f21, ra from the frame; sp back; f12 / f13 as it sets them).
            ctx->f[12] = dt;
            ctx->f[13] = invDt;
            SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0u));
            SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 8u));
            SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x10u));
            SET_GPR_U64(ctx, 19, m.get<uint64_t>(sp + 0x18u));
            ctx->f[21] = d1b::d1float(m.u32(sp + 0x30u));
            ctx->f[20] = d1b::d1float(m.u32(sp + 0x28u));
            SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0x20u));
            SET_GPR_U64(ctx, 29, entrySp);
        }
        SET_GPR_U64(ctx, 2, 1u);
        if (__builtin_expect(regChk, 0))
        {
            bool same = GPR_U64(ctx, 29) == saved[8] && GPR_U64(ctx, 30) == saved[9] && GPR_U64(ctx, 31) == saved[10];
            for (int i = 0; i < 8; ++i)
                same = same && GPR_U64(ctx, 16 + i) == saved[i];
            same = same && std::memcmp(&ctx->f[20], savedF, sizeof(savedF)) == 0;
            if (chk)
                y2d::noteY2(3, same, "callee-saved registers", cloth);
            if (z2chk)
                z2::noteRegisters(same);
        }
        ctx->pc = static_cast<uint32_t>(GPR_U64(ctx, 31));
    }
} // namespace dc2
