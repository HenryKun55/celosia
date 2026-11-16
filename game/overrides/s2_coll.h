// Round S2 (SOTC_S2COLL): the collision subsystem's redesign for the Vita, part 1: the finder clips on host list views
// (included by decomp_collision.cpp inside its namespace, after coll125.h; types in coll_types.h, the write-up in
// docs/collision_s2.md).
//
// The PS2's way: a character's contact list (a collision finder, refilled by the ground fill every frame) is clipped
// many times a frame (the landing ray, the IK feet, the slide's probes), and each clip walks the list entry by entry
// through the surface's id: layout class, collision work, part records, the plane cache's flag and the plane. Round 125
// (fin125) made that a host cache keyed by the id words (a 512-slot open-addressed table, each entry looked up, its
// epochs and its work's epoch checked, its LRU stamp written, then the clip). That is still a hash probe over 190-byte
// slots, three to five dependent loads and a store per entry per clip, and the plane test one entry at a time.
//
// Here a list gets a host view the first time it is clipped with every entry current in fin125's cache: the id words
// copied, each entry's slot pointer, and the entries' planes as structure-of-arrays (nx[], ny[], nz[], nw[]). The view
// is used for a later clip while nothing it rests on has moved: the same frame, no flush since (fin125's global and
// worker epochs and its count of work-epoch bumps), no fin125 slot taken over or made again (coll125::g_gen), and the
// list's id words byte for byte the same. Then every entry is current in fin125's cache, which is exactly when
// coll125::run uses its kept surface without a check; so the view path asks clip() of the same Entry objects in the
// same order and writes the record the same way: the same answer, the same record, the same guest writes.
//
// On the Vita the plane test runs four entries at a time: clip()'s first step is round 49's NEON plane distances
// ((vf0.x + n.x a.x) + n.y a.y) + n.z a.z + n.w, the same lane operations in the same order, here with the entries in
// the lanes: the same bits. An entry whose start is behind its plane or whose end is in front (clip()'s own rejection on
// those bits) is passed over without touching its slot; the others, and every entry after a hit in the same block (the
// segment is shorter then), go through clip() itself. A view is made only when all its entries are kept host surfaces
// whose planes are small (finite, below 2^61), and used only when the segment's ends and vf0 are small: then clip()
// never returns "ask ClipCollisionDB" for them, so no guest call can come between the entries.
//
// SOTC_S2COLL_CHECK=1: every clip the view answers is first run the original way (fin125 and S2 off: the guest's
// ClipCollisionDB per entry), the record kept, put back, then answered by the view; v0, the hit's id, u, v, kind and the
// point compared bit for bit ([s2check]). SOTC_S2COLL_LOG=1: counts every 600 frames ([s2coll]).
namespace s2coll
{
    inline float s2mx(float a, float b) { return a > b ? a : b; } // (std::fmax is a library call on the A9's VFPv3)
    // SOTC_S2COLL_VIEWS=0 leaves the list views out while SOTC_S2COLL stays on (the water pre-tests alone); read once.
    A9S_HOIST(bool, a9s_s2_coll_v_37, !(ps2_diag::env("SOTC_S2COLL_VIEWS") && ps2_diag::env("SOTC_S2COLL_VIEWS")[0] == '0')); // round A9 sweep (a9sweep.h)
    inline bool viewsOn()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s2_coll_v_37, bool, !(ps2_diag::env("SOTC_S2COLL_VIEWS") && ps2_diag::env("SOTC_S2COLL_VIEWS")[0] == '0'));
    }
    inline bool on(const PS2Runtime *runtime)
    {
        return sotc_decomp::switchOn(sotc_decomp::kS2Coll) && viewsOn() && coll125::on(runtime);
    }
    A9S_HOIST(int, a9s_s2_coll_v_46, ps2_diag::env("SOTC_S2COLL_CHECK") ? std::atoi(ps2_diag::env("SOTC_S2COLL_CHECK")) : 0); // round A9 sweep (a9sweep.h)
    inline int checkLevel()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s2_coll_v_46, int, ps2_diag::env("SOTC_S2COLL_CHECK") ? std::atoi(ps2_diag::env("SOTC_S2COLL_CHECK")) : 0);
    }
    A9S_HOIST(bool, a9s_s2_coll_v_51, ps2_diag::env("SOTC_S2COLL_LOG") && std::atoi(ps2_diag::env("SOTC_S2COLL_LOG")) != 0); // round A9 sweep (a9sweep.h)
    inline bool logOn()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s2_coll_v_51, bool, ps2_diag::env("SOTC_S2COLL_LOG") && std::atoi(ps2_diag::env("SOTC_S2COLL_LOG")) != 0);
    }

    constexpr int kMaxEntries = 256;
    constexpr int kViews = 16;
    struct ListView
    {
        uint32_t list = 0;
        int32_t n = 0;
        bool valid = false;
        uint64_t failFrame = ~0ull, failGen = 0, failEpoch = 0; // the last failed build (not tried again while these hold)
        uint32_t failList = 0;
        uint64_t frame = 0, epoch = 0, gen = 0, workBumps = 0;
        uint32_t epochW = 0;
        uint32_t ids[2 * kMaxEntries];
        coll125::Surf *surf[kMaxEntries];
        float nx[kMaxEntries + 4], ny[kMaxEntries + 4], nz[kMaxEntries + 4], nw[kMaxEntries + 4];
    };
    inline ListView g_views[kViews];
    inline ListView &viewFor(uint32_t list) { return g_views[(list >> 3) & (kViews - 1)]; }

    struct Stats
    {
        uint64_t clips = 0, viewClips = 0, built = 0, buildFail = 0, stale = 0, idsDiffer = 0, notSmall = 0, workerMoved = 0;
        uint64_t entries = 0, skipped = 0, clipped = 0, far = 0, hits = 0, keepView = 0;
        uint64_t checks = 0, differ = 0, keepChecks = 0, keepDiffer = 0, keepUnknown = 0;
        uint64_t logFrame = 0;
    };
    inline Stats g_st;

    inline void log()
    {
        if (!logOn() && checkLevel() == 0)
            return;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_st.logFrame + 600u)
            return;
        if (g_st.logFrame != 0u)
        {
            const double nf = static_cast<double>(frame - g_st.logFrame);
            std::fprintf(stderr,
                         "[s2coll] frame %llu: finder clips %.1f a frame, by a view %.1f (%.1f%%); views built %.2f (failed %.2f), stale %.2f, ids differ %.2f, ends not small %.2f, "
                         "worker flush %.2f; entries %.1f a frame: passed over %.1f, sphere far %.1f, clipped %.1f, hits %.2f; keep tests by a view %.1f\n",
                         (unsigned long long)frame, g_st.clips / nf, g_st.viewClips / nf, g_st.clips ? 100.0 * g_st.viewClips / g_st.clips : 0.0, g_st.built / nf,
                         g_st.buildFail / nf, g_st.stale / nf, g_st.idsDiffer / nf, g_st.notSmall / nf, g_st.workerMoved / nf, g_st.entries / nf, g_st.skipped / nf, g_st.far / nf,
                         g_st.clipped / nf, g_st.hits / nf, g_st.keepView / nf);
            if (checkLevel() != 0)
                std::fprintf(stderr, "[s2check] frame %llu: view clips compared with the original %llu, differ %llu; keep tests compared with fin125's %llu, differ %llu (fin125 not known %llu)\n",
                             (unsigned long long)frame, (unsigned long long)g_st.checks, (unsigned long long)g_st.differ, (unsigned long long)g_st.keepChecks,
                             (unsigned long long)g_st.keepDiffer, (unsigned long long)g_st.keepUnknown);
        }
        const Stats keep = g_st;
        g_st = Stats{};
        g_st.checks = keep.checks;
        g_st.differ = keep.differ;
        g_st.keepChecks = keep.keepChecks;
        g_st.keepDiffer = keep.keepDiffer;
        g_st.keepUnknown = keep.keepUnknown;
        g_st.logFrame = frame;
    }

    // SOTC_S2COLL_LOG=1: the finder clips' host time by the switch's state, cumulative from the run's start (so the last
    // line of a run is its whole A/B): clips, us each (mean, sd), ms a frame of that state's frames, a Welch z of the
    // per-clip means (positive: S2 faster).
    struct Timing
    {
        uint64_t n[2] = {}, frames[2] = {}, lastFrame[2] = {};
        double sum[2] = {}, sq[2] = {};
        uint64_t logFrame = 0;
    };
    inline Timing g_t;
    inline void timed(int st, uint64_t us)
    {
        Timing &t = g_t;
        const double d = static_cast<double>(us);
        ++t.n[st];
        t.sum[st] += d;
        t.sq[st] += d * d;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (t.lastFrame[st] != frame)
        {
            t.lastFrame[st] = frame;
            ++t.frames[st];
        }
        if (frame >= t.logFrame + 600u)
        {
            t.logFrame = frame;
            double mean[2] = {}, var[2] = {};
            for (int i = 0; i < 2; ++i)
                if (t.n[i] > 1u)
                {
                    mean[i] = t.sum[i] / t.n[i];
                    var[i] = (t.sq[i] - t.sum[i] * mean[i]) / (t.n[i] - 1u);
                }
            const double se = std::sqrt((t.n[0] ? var[0] / t.n[0] : 0.0) + (t.n[1] ? var[1] / t.n[1] : 0.0));
            std::fprintf(stderr, "[s2time] frame %llu finder clips: off %llu clips %.3f us (sd %.2f) %.4f ms a frame (%llu frames) | on %llu clips %.3f us (sd %.2f) %.4f ms a frame (%llu frames) | z %.2f\n",
                         (unsigned long long)frame, (unsigned long long)t.n[0], mean[0], std::sqrt(var[0]), t.frames[0] ? t.sum[0] / 1000.0 / t.frames[0] : 0.0,
                         (unsigned long long)t.frames[0], (unsigned long long)t.n[1], mean[1], std::sqrt(var[1]), t.frames[1] ? t.sum[1] / 1000.0 / t.frames[1] : 0.0,
                         (unsigned long long)t.frames[1], se > 0.0 ? (mean[0] - mean[1]) / se : 0.0);
        }
    }

    // Whether v answers a clip of the list at `list` (n entries) now.
    inline bool usable(const G g, ListView &v, uint32_t list, int32_t n)
    {
        if (!v.valid || v.list != list || v.n != n)
            return false;
        if (v.frame != sotc_decomp::g_gameFrame || v.epoch != coll125::g_epoch || v.gen != coll125::g_gen || v.workBumps != coll125::g_workBumps ||
            v.epochW != coll125::g_epochW.load(std::memory_order_relaxed))
        {
            ++g_st.stale;
            v.valid = false;
            return false;
        }
        if (std::memcmp(v.ids, g.rdram + list, 8u * static_cast<uint32_t>(n)) != 0)
        {
            ++g_st.idsDiffer;
            v.valid = false;
            return false;
        }
        return true;
    }

    inline bool smallPlane(const __m128 &n)
    {
        float w[4];
        std::memcpy(w, &n, 16);
        for (float f : w)
            if (!(std::fabs(f) < 0x1p61f))
                return false;
        return true;
    }

    // After coll125::run on the list: the view, when every entry is a kept host surface current now with a small plane.
    __attribute__((noinline)) inline void build(const G g, uint32_t list, int32_t n)
    {
        ListView &v = viewFor(list);
        v.valid = false;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (v.failList == list && v.failFrame == frame && v.failGen == coll125::g_gen && v.failEpoch == coll125::g_epoch)
            return;
        for (int32_t k = 0; k < n; ++k)
        {
            uint32_t w[2];
            std::memcpy(w, g.rdram + list + 8u * static_cast<uint32_t>(k), 8);
            v.ids[2 * k] = w[0];
            v.ids[2 * k + 1] = w[1];
            if (w[0] == 0u)
            {
                // (run passes over an empty id; a plane that is never in front or behind: the block test passes it, and
                // the per-entry step below passes over it as run does)
                v.surf[k] = nullptr;
                v.nx[k] = v.ny[k] = v.nz[k] = 0.0f;
                v.nw[k] = 0.0f;
                continue;
            }
            coll125::Surf *x = coll125::findSurf(w[0], w[1]);
            if (!x || x->m.state != coll125::kHost || !x->current(frame) || !smallPlane(x->e.n))
            {
                ++g_st.buildFail;
                v.failList = list;
                v.failFrame = frame;
                v.failGen = coll125::g_gen;
                v.failEpoch = coll125::g_epoch;
                return;
            }
            v.surf[k] = x;
            float nn[4];
            std::memcpy(nn, &x->e.n, 16);
            v.nx[k] = nn[0];
            v.ny[k] = nn[1];
            v.nz[k] = nn[2];
            v.nw[k] = nn[3];
        }
        for (int32_t k = n; k < n + 4; ++k)
            v.nx[k] = v.ny[k] = v.nz[k] = v.nw[k] = 0.0f;
        v.list = list;
        v.n = n;
        v.frame = frame;
        v.epoch = coll125::g_epoch;
        v.gen = coll125::g_gen;
        v.workBumps = coll125::g_workBumps;
        v.epochW = coll125::g_epochW.load(std::memory_order_relaxed);
        v.valid = true;
        ++g_st.built;
    }

    // Whether the surface's boundary sphere (s: centre, r^2) is out of the segment a -> b's reach by a margin: then clip()
    // gives no hit (its crossing point lies on the segment, and its squared distance to the centre is over r^2). The
    // margin covers the crossing point's rounding (its division and the PS2 lerp: under 1e-6 of the coordinates) and this
    // test's own float rounding (under 1e-6 of the coordinates): 3e-5 of the largest coordinate plus 1e-3, and 1e-5 of the
    // squared limit. Anything not finite: false (clip() decides).
    inline bool sphereFar(__m128 a, __m128 b, __m128 s)
    {
        float pa[4], pb[4], c[4];
        std::memcpy(pa, &a, 16);
        std::memcpy(pb, &b, 16);
        std::memcpy(c, &s, 16);
        const float r2 = c[3];
        if (!(r2 >= 0.0f) || !(r2 < 1e30f))
            return false;
        float big = 0.0f, ab[3], ac[3];
        for (int i = 0; i < 3; ++i)
        {
            if (!(std::fabs(pa[i]) < 1e15f) || !(std::fabs(pb[i]) < 1e15f) || !(std::fabs(c[i]) < 1e15f))
                return false;
            big = s2mx(big, s2mx(std::fabs(pa[i]), s2mx(std::fabs(pb[i]), std::fabs(c[i]))));
            ab[i] = pb[i] - pa[i];
            ac[i] = c[i] - pa[i];
        }
        const float l2 = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2];
        float t = 0.0f;
        if (l2 > 0.0f)
        {
            t = (ac[0] * ab[0] + ac[1] * ab[1] + ac[2] * ab[2]) / l2;
            t = t < 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
        }
        float d2 = 0.0f;
        for (int i = 0; i < 3; ++i)
        {
            const float d = ac[i] - t * ab[i];
            d2 += d * d;
        }
        const float lim = std::sqrt(r2) + 3e-5f * big + 1e-3f;
        return d2 > lim * lim * 1.00001f;
    }

#if PS2X_VU_NEON_A32
    // The block's pass mask (bit j: entry k + j may hit): not (d(a) below zero or d(b) above zero) on the bits round 49's
    // planeDistances gives each entry (lanes = entries, the same operations in the same order).
    inline unsigned blockMask(const ListView &v, int32_t k, float32x4_t vf0x, float32x4_t a, float32x4_t b)
    {
        const float32x4_t nx = vld1q_f32(v.nx + k), ny = vld1q_f32(v.ny + k), nz = vld1q_f32(v.nz + k), nw = vld1q_f32(v.nw + k);
        float32x4_t da = vaddq_f32(vf0x, vmulq_n_f32(nx, vgetq_lane_f32(a, 0)));
        da = vaddq_f32(da, vmulq_n_f32(ny, vgetq_lane_f32(a, 1)));
        da = vaddq_f32(da, vmulq_n_f32(nz, vgetq_lane_f32(a, 2)));
        da = vaddq_f32(da, nw);
        float32x4_t db = vaddq_f32(vf0x, vmulq_n_f32(nx, vgetq_lane_f32(b, 0)));
        db = vaddq_f32(db, vmulq_n_f32(ny, vgetq_lane_f32(b, 1)));
        db = vaddq_f32(db, vmulq_n_f32(nz, vgetq_lane_f32(b, 2)));
        db = vaddq_f32(db, nw);
        const uint32x4_t expMask = vdupq_n_u32(0x7F800000u), sign = vdupq_n_u32(0x80000000u);
        const uint32x4_t ua = vreinterpretq_u32_f32(da), ub = vreinterpretq_u32_f32(db);
        // belowZero(a): sign set, exponent non-zero; aboveZero(b): sign clear, exponent non-zero
        const uint32x4_t aNz = vtstq_u32(ua, expMask), bNz = vtstq_u32(ub, expMask);
        const uint32x4_t below = vandq_u32(vtstq_u32(ua, sign), aNz);
        const uint32x4_t above = vbicq_u32(bNz, vtstq_u32(ub, sign));
        const uint32x4_t reject = vorrq_u32(below, above);
        uint32_t r[4];
        vst1q_u32(r, reject);
        return (r[0] ? 0u : 1u) | (r[1] ? 0u : 2u) | (r[2] ? 0u : 4u) | (r[3] ? 0u : 8u);
    }
#endif

    // The clip on the view v (usable): ClipCollisionFinder / ...CallBack's answer as coll125::run gives it, or false when
    // the view cannot answer it (the caller runs coll125::run). On true, best / the record are set as run sets them.
    template <bool Filtered>
    __attribute__((noinline)) bool clipView(const G g, ListView &v, uint32_t a0, uint32_t a1, uint32_t a2, int32_t n, uint32_t filter, uint32_t user, uint32_t sp, uint32_t filterSite,
                  uint32_t &bestOut)
    {
        R5900Context *ctx = g.ctx;
        const __m128 a = g.q(a1);
        __m128 b = g.q(a2);
        const __m128 vf0 = ctx->vu0_vf[0];
        if (!smallPlane(a) || !smallPlane(b) || !smallPlane(vf0))
        {
            ++g_st.notSmall;
            return false;
        }
        const uint32_t epochW = coll125::g_epochW.load(std::memory_order_relaxed);
        g.put(sp, b);
        uint32_t best = 0u;
        bool bestHost = false;
        uint32_t bestW0 = 0u, bestW1 = 0u;
        bool bestIndep = false;
        float bu = 0.0f, bv = 0.0f;
        const uint32_t list = v.list;
        uint64_t skipped = 0, clipped = 0, far = 0;
        auto step = [&](int32_t k) -> bool
        {
            coll125::Surf *x = v.surf[k];
            if (!x)
                return false;
            if (sphereFar(a, b, x->e.s))
            {
                ++far;
                return false;
            }
            ++clipped;
            __m128 p;
            float u = 0.0f, vv = 0.0f;
            const int r = coll125::clip(ctx, x->e, a, b, p, u, vv);
            if (r <= 0)
                return false;
            const uint32_t cand = list + 8u * static_cast<uint32_t>(k);
            if (Filtered && !directFilter(g, filter, cand, user, filterSite + 8u, sp))
            {
                ++coll125::g_st.filtered;
                return false;
            }
            ++coll125::g_st.hits;
            b = p;
            best = cand;
            bestHost = true;
            bestW0 = x->m.w0;
            bestW1 = x->m.w1;
            bestIndep = x->m.indep != 0u;
            bu = u;
            bv = vv;
            return true;
        };
#if PS2X_VU_NEON_A32
        const float32x4_t vf0x = vdupq_n_f32(vgetq_lane_f32(fast::f(vf0), 0));
        for (int32_t k = 0; k < n; k += 4)
        {
            unsigned mask = blockMask(v, k, vf0x, fast::f(a), fast::f(b));
            bool hitHere = false;
            const int32_t end = k + 4 < n ? k + 4 : n;
            for (int32_t j = k; j < end; ++j)
            {
                if (!hitHere && (mask & (1u << (j - k))) == 0u)
                {
                    ++skipped;
                    continue;
                }
                if (step(j))
                    hitHere = true;
            }
        }
#else
        for (int32_t k = 0; k < n; ++k)
            step(k);
#endif
        if (coll125::g_epochW.load(std::memory_order_relaxed) != epochW)
        {
            // a flush on the worker's lane while the view was read: the caller asks run instead (nothing written but
            // the segment's copy at sp, which run writes first too)
            ++g_st.workerMoved;
            return false;
        }
        g_st.entries += static_cast<uint64_t>(n);
        g_st.skipped += skipped;
        g_st.clipped += clipped;
        g_st.far += far;
        coll125::g_st.entries += static_cast<uint64_t>(n);
        ++coll125::g_st.calls;
        if (bestHost)
        {
            ++g_st.hits;
            const __m128 pt = b;
            g.put(a0 + 0x40u, pt);
            g.put32(a0 + 8u, coll125::fbits(bu));
            g.put32(a0 + 0xCu, coll125::fbits(bv));
            g.put32(a0, bestW0);
            g.put32(a0 + 4u, bestW1);
            uint32_t cls2;
            if (!bestIndep || !layoutClass(g, bestW0, cls2) || cls2 == 0u)
                g.put32(a0 + 0x30u, 3u);
            else
            {
                g.put32(a0 + 0x30u, 2u);
                const uint32_t mm = ((bestW1 >> 24) & 0x7Fu) * 0x40u + g.u32(cls2 + 0x164u);
                const __m128 local = transform(g, mm, pt);
                g.put(a0 + 0x10u, setXyz(local, PS2_VMUL(local, g.q(cls2 + 0xA0u))));
            }
        }
        bestOut = best;
        return true;
    }

    // The finder clip with S2 on (fin125 on, a list of 1..256 plain entries): a view's answer when one holds, else
    // coll125::run and a view made for the next clip. Returns the last hit's entry address (0: none).
    template <bool Filtered>
    uint32_t clipList(const G g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, int32_t n, uint32_t filter, uint32_t user, uint32_t sp, uint32_t filterSite,
                      uint32_t clipSite)
    {
        log();
        ++g_st.clips;
        const uint32_t list = g.u32(a3 + 0xCu);
        ListView &v = viewFor(list);
        uint32_t best = 0u;
        if (usable(g, v, list, n) && clipView<Filtered>(g, v, a0, a1, a2, n, filter, user, sp, filterSite, best))
        {
            ++g_st.viewClips;
            return best;
        }
        best = coll125::run<Filtered, 1>(g, a0, a1, a2, a3, n, filter, user, sp, filterSite, clipSite);
        build(g, list, n);
        return best;
    }

    // The keep pass (ClipCollisionFinderWithKeep, through decomp_collision's keepEarlyFrom): entry k of the list whose
    // entry is at `id`, answered from a view of that list when one holds (its stamps current and the entry's id words the
    // view's): then the entry's slot is current in fin125's cache, which is when coll125::keepEarly125 answers from it;
    // the same test on the same Entry. 1 / 0, or -1 (not known here: keepEarly125 and the rest as before).
    __attribute__((noinline)) inline int keepEarly(const uint8_t *rdram, const R5900Context *ctx, const PS2Runtime *runtime, int32_t k, uint32_t id, uint32_t point, float keep)
    {
#if PS2X_VU_NEON_A32
        if (k < 0 || k >= kMaxEntries || !on(runtime) || sotc_decomp::onWorkerLane(runtime) || !G::plain(point & ~0xFu, 16u) || !G::plain(id, 8u))
            return -1;
        const uint32_t list = id - 8u * static_cast<uint32_t>(k);
        const ListView &v = viewFor(list);
        if (!v.valid || v.list != list || k >= v.n || v.frame != sotc_decomp::g_gameFrame || v.epoch != coll125::g_epoch || v.gen != coll125::g_gen ||
            v.workBumps != coll125::g_workBumps || v.epochW != coll125::g_epochW.load(std::memory_order_relaxed))
            return -1;
        uint32_t w[2];
        std::memcpy(w, rdram + id, 8);
        const coll125::Surf *x = v.surf[k];
        if (!x || w[0] != v.ids[2 * k] || w[1] != v.ids[2 * k + 1])
            return -1;
        __m128 pa;
        std::memcpy(&pa, rdram + (point & ~0xFu), 16);
        bool small = false;
        const uint64_t bits = fast::planeDistances(x->e.n, ctx->vu0_vf[0], pa, pa, small);
        if (!small)
            return -1;
        ++coll125::g_st.keepHost;
        ++g_st.keepView;
        const float ahead = FPU_SUB_S(fast::asFloat(static_cast<uint32_t>(bits)), keep), zero = 0.0f;
        return FPU_C_OLE_S(zero, ahead) ? 1 : 0;
#else
        (void)rdram, (void)ctx, (void)runtime, (void)k, (void)id, (void)point, (void)keep;
        return -1;
#endif
    }

    // The check mode's keep-test compare (decomp_collision's keepEarlyFrom): the view's answer and keepEarly125's.
    inline void keepChecked(int view, int fin125)
    {
        if (fin125 < 0)
        {
            ++g_st.keepUnknown;
            return;
        }
        ++g_st.keepChecks;
        if (view != fin125)
        {
            ++g_st.keepDiffer;
            if (g_st.keepDiffer <= 20u)
                std::fprintf(stderr, "[s2check] frame %llu keep test DIFFER: view %d fin125 %d\n", (unsigned long long)sotc_decomp::g_gameFrame, view, fin125);
        }
    }

    // SOTC_S2COLL_CHECK=1: a clip the view can answer is run the original way first (fin125 off: the guest's
    // ClipCollisionDB per entry; orig runs it and leaves v0), its record kept; the record put back, the view's answer
    // made; the two compared bit for bit (v0's hit / no hit, the id, u, v, the kind, the point, an independent surface's
    // local point); the original's answer and record kept.
    template <bool Filtered, typename Orig>
    uint32_t checked(const G g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, int32_t n, uint32_t filter, uint32_t user, uint32_t sp, uint32_t filterSite,
                     uint32_t clipSite, Orig orig)
    {
        const uint32_t list = g.u32(a3 + 0xCu);
        ListView &v = viewFor(list);
        if (!usable(g, v, list, n))
            return clipList<Filtered>(g, a0, a1, a2, a3, n, filter, user, sp, filterSite, clipSite);
        uint8_t before[0x50], after[0x50], mine[0x50];
        std::memcpy(before, g.rdram + a0, 0x50);
        sotc_decomp::setSwitch(sotc_decomp::kFin125, false);
        orig();
        sotc_decomp::setSwitch(sotc_decomp::kFin125, true);
        const uint32_t bestO = GPR_U32(g.ctx, 2);
        std::memcpy(after, g.rdram + a0, 0x50);
        std::memcpy(g.rdram + a0, before, 0x50);
        uint32_t best = 0u;
        log();
        ++g_st.clips;
        if (usable(g, v, list, n) && clipView<Filtered>(g, v, a0, a1, a2, n, filter, user, sp, filterSite, best))
        {
            ++g_st.viewClips;
            if (best != 0u)
            {
                g.put32(a0, g.u32(best));
                g.put32(a0 + 4u, g.u32(best + 4u));
            }
            std::memcpy(mine, g.rdram + a0, 0x50);
            ++g_st.checks;
            bool same = (best != 0u) == (bestO != 0u);
            if (same && best != 0u)
            {
                same = A9_MEMEQ(mine, after, 0x10) && A9_MEMEQ(mine + 0x30, after + 0x30, 4) && A9_MEMEQ(mine + 0x40, after + 0x40, 12);
                uint32_t kind;
                std::memcpy(&kind, after + 0x30, 4);
                if (same && kind == 2u)
                    same = A9_MEMEQ(mine + 0x10, after + 0x10, 12);
            }
            if (!same)
            {
                ++g_st.differ;
                if (g_st.differ <= 30u)
                {
                    float pa[3], pb[3];
                    std::memcpy(pa, after + 0x40, 12);
                    std::memcpy(pb, mine + 0x40, 12);
                    uint32_t ia[2], ib[2];
                    std::memcpy(ia, after, 8);
                    std::memcpy(ib, mine, 8);
                    std::fprintf(stderr, "[s2check] frame %llu DIFFER n %d: original %08x (%08x %08x) p %.5f %.5f %.5f | view %08x (%08x %08x) p %.5f %.5f %.5f\n",
                                 (unsigned long long)sotc_decomp::g_gameFrame, n, bestO, ia[0], ia[1], pa[0], pa[1], pa[2], best, ib[0], ib[1], pb[0], pb[1], pb[2]);
                }
            }
        }
        std::memcpy(g.rdram + a0, after, 0x50);
        return bestO;
    }
}
