// Round 125 (SOTC_FIN125): the finder lists' surfaces as a host cache with
// whole triangles (included by decomp_collision.cpp inside its namespace,
// before FlushModifiedManager's native, which moves the epochs below).
//
// A collision finder (the ground fill, the finder steps) leaves a list of
// surface ids; the character's probes then clip segments against that list
// many times a frame (ClipCollisionFinder / ...CallBack: the landing test, the
// IK's foot search). The original asks ClipCollisionDB for every entry: the
// object's layout class, its collision work, the part's records, the plane's
// cached flag and the plane, then for the few the segment crosses the
// boundary sphere and the three cached world vertices, all in guest memory
// (four to ten dependent loads an entry on the A9).
//
// Here every surface met in a list gets a slot in a host cache keyed by its
// two id words (the ground fill makes its list again every frame, in an order
// that changes): made at its first test, the plane and the boundary sphere
// (centre, r^2) as cached, the first vertex and ClipCollisionDB's triangle
// terms (its n x e1 / n x e2 on the clamped edges and their denominators, made
// once by its own VU0 macros), and its GetCollisionDBAttributeAdr answer,
// taken from the game's own per-object caches the way ClipCollisionDB takes
// them (a plane, boundary or vertex not cached yet is made by the same calls,
// with the same cache entries). A clip is then one pass over host data:
// ClipCollisionDB's tests after its lookups, the same operations on the same
// values (the plane test in round 49's NEON form, the crossing, the sphere,
// u / v), the segment shortened at each hit, the id filter (pure judge
// functions) asked only for an entry that hits; the hit record (+0 / +4 the
// id, +8 / +0xc u / v, +0x10 the local point of an independent surface, +0x30
// the kind, +0x40 the point) written once, for the last hit, as the last
// ClipCollisionDB call leaves it. The keep pass (ClipCollisionFinderWithKeep)
// and GetCollisionDBAttributeAdr ask the cache too (keepEarly125, attrOf).
//
// Lifetime: the game's caches change only through FlushModifiedManager (an
// object that moved has its cache flags cleared). A flush made by
// FlushCollisionWorkByWorkStruct moves that collision work's epoch (g_works:
// only the works some kept surface belongs to), any other flush on the game
// thread the global g_epoch, any flush on the worker's lane g_epochW. A slot
// is used as kept while its frame and the three epochs are the ones it was
// checked at; else its plane and boundary flags must still be set and their
// values the same as kept, or it is made again.
//
// Game thread only (the worker's lane makes every call). Logs: SOTC_COLL115_LOG=1
// adds [fin125] (clips, entries, plane passes, hits, made, checks, dropped, keep
// tests and attributes answered), [fin125e] (flushes by kind) and [fin125t]
// (each clip's host time by the switch's state). SOTC_FIN125_CHECK=1: each clip
// the original way first (its answer used), compared ([fin125m]).
namespace coll125
{
    inline uint64_t g_epoch = 1; // moved by a flush of a non-empty modified list not made through a collision work's flush

    // Per collision work (an object's caches: the class's +0x174), an epoch moved when FlushCollisionWorkByWorkStruct
    // flushes one of its non-empty lists: the surfaces of the objects that did not move stay checked. Only works some
    // kept surface belongs to are in the table (a flush of any other cannot change a kept surface); open addressing,
    // never removed (a full neighbourhood: the surface is checked at every use instead).
    struct WorkEpoch
    {
        uint32_t work, epoch;
    };
    constexpr uint32_t kWorks = 512;
    inline WorkEpoch g_works[kWorks];
    inline uint32_t g_curWork = 0; // the work FlushCollisionWorkByWorkStruct is flushing on the game thread (0: none)
    inline std::atomic<uint32_t> g_epochW{1}; // moved by any flush on the guest worker's lane (round 108's character jobs)
    inline uint64_t g_workBumps = 0, g_globalBumps = 0;
    inline uint32_t hashWork(uint32_t w) { return (w * 0x9E3779B1u) >> 23; }
    inline WorkEpoch *findWork(uint32_t work)
    {
        const uint32_t h = hashWork(work);
        for (uint32_t i = 0; i < 8u; ++i)
        {
            WorkEpoch &x = g_works[(h + i) & (kWorks - 1u)];
            if (x.work == work)
                return &x;
            if (x.work == 0u)
                return nullptr;
        }
        return nullptr;
    }
    inline WorkEpoch *addWork(uint32_t work)
    {
        const uint32_t h = hashWork(work);
        for (uint32_t i = 0; i < 8u; ++i)
        {
            WorkEpoch &x = g_works[(h + i) & (kWorks - 1u)];
            if (x.work == work)
                return &x;
            if (x.work == 0u)
            {
                x.work = work;
                x.epoch = 1u;
                return &x;
            }
        }
        return nullptr;
    }
    // FlushModifiedManager's native, for a non-empty list.
    inline void flushed(const PS2Runtime *runtime)
    {
        if (sotc_decomp::onWorkerLane(runtime))
        {
            g_epochW.store(g_epochW.load(std::memory_order_relaxed) + 1u, std::memory_order_relaxed);
            return;
        }
        if (g_curWork != 0u)
        {
            if (WorkEpoch *w = findWork(g_curWork))
            {
                ++w->epoch;
                ++g_workBumps;
            }
            return;
        }
        ++g_epoch;
        ++g_globalBumps;
    }

    inline bool on(const PS2Runtime *runtime) { return sotc_decomp::switchOn(sotc_decomp::kFin125) && !sotc_decomp::onWorkerLane(runtime); }
    inline int checkLevel()
    {
        static const int v = ps2_diag::env("SOTC_FIN125_CHECK") ? std::atoi(ps2_diag::env("SOTC_FIN125_CHECK")) : 0;
        return v;
    }

    enum : uint8_t
    {
        kUnmade = 0, // a free slot, or one to make again
        kHost = 1,   // plane, sphere and triangle kept
        kGuest = 2,  // the original's ClipCollisionDB for this entry (a class made by a call, an address outside RAM, a flat triangle)
    };
    struct Entry
    {
        __m128 n;      // the plane (n, w) as cached
        __m128 s;      // boundary centre, r^2 (GetCollisionDBSurfBoundary's record)
        __m128 v0;     // the first cached world vertex
        __m128 c1, c2; // ClipCollisionDB's n x e1 and n x e2 (n = e1 x e2, e1 / e2 the clamped edges from v0), made once
        float d1, d2;  // c1 . e2 and c2 . e1 (the denominators of v and u), made once
    };
    // ClipCollisionDB's triangle terms (its VU0 macros on the clamped edges), from the three vertices.
    inline void triangleTerms(Entry &e, __m128 p0, __m128 p1, __m128 p2, __m128 vf0)
    {
        const __m128 vf0c = vu::clamp(vf0);
        const __m128 e1 = vu::clamp(PS2_VSUB(p1, p0)), e2 = vu::clamp(PS2_VSUB(p2, p0));
        auto crossN = [](__m128 a, __m128 b)
        {
            const __m128 acc = vu::mulN(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)));
            const __m128 r = vu::subN(acc, vu::mulN(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))));
            return _mm_blendv_ps(_mm_setzero_ps(), r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        };
        __m128 nrm = crossN(e1, e2);
        nrm = setLane3(nrm, vu::subN(nrm, nrm));
        auto dot = [&](__m128 a, __m128 b)
        {
            const __m128 prod = vu::mulN(a, b);
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            return x(vu::addN(acc, lane<2>(prod)));
        };
        e.v0 = p0;
        e.c2 = crossN(nrm, e2);
        e.c1 = crossN(nrm, e1);
        e.d2 = dot(e.c2, e1);
        e.d1 = dot(e.c1, e2);
    }
    struct Meta
    {
        uint32_t w0, w1;          // the id words it was made from
        uint32_t planeFlag, plane; // the plane's cached flag and the plane
        uint32_t bndFlag, bnd;     // the boundary's cached flag and record
        uint32_t bndW[4];          // the boundary record's words as made
        uint32_t attr;             // GetCollisionDBAttributeAdr's answer for it (when hasAttr)
        uint8_t state;
        uint8_t indep;
        uint8_t hasAttr;
    };
    // The surfaces of every list, keyed by their id words (a list is made again by the ground fill every frame,
    // in an order that changes): open addressing, 8 probes, the least recently used of them replaced.
    struct Surf
    {
        Entry e;
        Meta m;
        uint64_t frame, epoch, used; // checked at (frame, epoch); last used
        uint32_t epochW;             // the worker's flush epoch when checked
        WorkEpoch *work;             // its object's work epoch (nullptr: checked at every use) and its value when checked
        uint32_t workEpoch;
        bool current(uint64_t f) const
        {
            return frame == f && epoch == g_epoch && work && work->epoch == workEpoch && epochW == g_epochW.load(std::memory_order_relaxed);
        }
        void stamp(uint64_t f, uint64_t ep)
        {
            frame = f;
            epoch = ep;
            epochW = g_epochW.load(std::memory_order_relaxed);
            workEpoch = work ? work->epoch : 0u;
        }
    };
    constexpr uint32_t kSurf = 512;
    inline Surf g_surf[kSurf];
    inline uint64_t g_use = 0;
    inline uint32_t hashId(uint32_t w0, uint32_t w1) { return ((w0 * 0x9E3779B1u) ^ (w1 * 0x85EBCA77u)) >> 23; }
    // The surface (w0, w1) when kept, else nullptr.
    inline Surf *findSurf(uint32_t w0, uint32_t w1)
    {
        const uint32_t h = hashId(w0, w1);
        for (uint32_t i = 0; i < 8u; ++i)
        {
            Surf &x = g_surf[(h + i) & (kSurf - 1u)];
            if (x.m.state != kUnmade && x.m.w0 == w0 && x.m.w1 == w1)
                return &x;
        }
        return nullptr;
    }
    // A slot for (w0, w1): its own, else the least recently used of its 8 (marked not made).
    inline Surf &claimSurf(uint32_t w0, uint32_t w1, bool &fresh)
    {
        const uint32_t h = hashId(w0, w1);
        Surf *victim = nullptr;
        for (uint32_t i = 0; i < 8u; ++i)
        {
            Surf &x = g_surf[(h + i) & (kSurf - 1u)];
            if (x.m.state != kUnmade && x.m.w0 == w0 && x.m.w1 == w1)
            {
                x.used = ++g_use;
                fresh = false;
                return x;
            }
            if (!victim || x.used < victim->used)
                victim = &x;
        }
        victim->m.state = kUnmade;
        victim->used = ++g_use;
        fresh = true;
        return *victim;
    }

    struct Stats
    {
        uint64_t keepHost = 0, attrHost = 0;
        uint64_t calls = 0, entries = 0, planePass = 0, hits = 0, guestEntries = 0, made = 0, sets = 0, checks = 0, dropped = 0, filtered = 0, bigLists = 0, evicted = 0;
        uint64_t checked = 0, differ = 0, differHit = 0, differId = 0;
        double maxd = 0.0;
        uint64_t us[2] = {}, nq[2] = {};
    };
    inline Stats g_st;
    inline uint64_t g_logFrame = 0;
    inline void log()
    {
        static const bool want = checkLevel() != 0 || (ps2_diag::env("SOTC_COLL115_LOG") && std::atoi(ps2_diag::env("SOTC_COLL115_LOG")) != 0);
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (!want || frame < g_logFrame + 600u)
            return;
        if (g_logFrame != 0u)
        {
            const double nf = static_cast<double>(frame - g_logFrame);
            Stats &s = g_st;
            std::fprintf(stderr,
                         "[fin125] frame %llu: a frame: clips %.1f entries %.1f plane-pass %.2f hits %.2f filtered %.2f guest entries %.2f | made %.2f (slots taken over %.2f) "
                         "checks %.2f dropped %.2f big lists %.2f keep tests %.2f attributes %.2f | check %llu differ %llu (hit / no hit %llu, other id %llu, point max %.2f mm)\n",
                         (unsigned long long)frame, s.calls / nf, s.entries / nf, s.planePass / nf, s.hits / nf, s.filtered / nf, s.guestEntries / nf, s.made / nf,
                         s.evicted / nf, s.checks / nf, s.dropped / nf, s.bigLists / nf, s.keepHost / nf, s.attrHost / nf, (unsigned long long)s.checked, (unsigned long long)s.differ,
                         (unsigned long long)s.differHit, (unsigned long long)s.differId, s.maxd * 1000.0);
            for (int i = 0; i < 2; ++i)
                if (s.nq[i])
                    std::fprintf(stderr, "[fin125t] frame %llu finder clips %s: %llu calls, %.2f us each (%.3f ms a frame)\n", (unsigned long long)frame, i ? "on" : "off",
                                 (unsigned long long)s.nq[i], (double)s.us[i] / s.nq[i], (double)s.us[i] / 1000.0 / nf);
            std::fprintf(stderr, "[fin125e] frame %llu: flushes a frame: by work %.1f, other %.2f; works kept %u\n", (unsigned long long)frame, g_workBumps / nf,
                         g_globalBumps / nf, [] { unsigned n = 0; for (const WorkEpoch &w : g_works) n += w.work != 0u; return n; }());
            g_workBumps = g_globalBumps = 0;
            const double md = s.maxd;
            g_st = Stats{};
            g_st.maxd = md;
        }
        g_logFrame = frame;
    }

}

namespace coll125
{
    // Entry k of the set: the entry's caches still what was kept (flags set, values the same).
    inline bool stillGood(const G g, const Meta &m, const Entry &e)
    {
        if (g.rdram[m.planeFlag] == 0u || g.rdram[m.bndFlag] == 0u)
            return false;
        if (std::memcmp(g.rdram + m.plane, &e.n, 16) != 0)
            return false;
        return std::memcmp(g.rdram + m.bnd, m.bndW, 16) == 0;
    }

    // Make entry k (id at `id`) from the game's caches, as ClipCollisionDB reaches them; scratch: a guest
    // frame below the clip's (ClipCollisionDB's own: spIn - 0xF0). False: the entry goes to ClipCollisionDB.
    inline bool make(const G g, Entry &e, Meta &m, uint32_t id, uint32_t spIn)
    {
        R5900Context *ctx = g.ctx;
        const uint32_t sp = spIn - 0xF0u;
        const uint32_t w0 = g.u32(id), w1 = g.u32(id + 4u);
        m.w0 = w0;
        m.w1 = w1;
        m.state = kGuest;
        uint32_t cls;
        if (!layoutClass(g, w0, cls) || cls == 0u || !G::plain(cls, 0x200u))
            return false;
        uint32_t normal, flag;
        if (!coll119::planeAddrs(g, id, normal, flag) || (normal & 0xFu) != 0u)
            return false;
        const bool indep = static_cast<int32_t>(w1) < 0;
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        const uint32_t work = g.u32(cls + 0x174u);
        if (!G::plain(work, 0x80u))
            return false;
        uint32_t bndFlag, bnd;
        if (indep)
        {
            const uint32_t t2 = g.u32(work + 0x24u) + s1 * 0x60u + 0x48u;
            if (!G::plain(t2, 0x18u))
                return false;
            bndFlag = g.u32(t2 + 8u) + s3;
            bnd = g.u32(t2 + 0x14u) + s3 * 16u;
        }
        else
        {
            const uint32_t t1 = g.u32(g.u32(g.u32(cls + 0x184u) + 0x20u) + 8u);
            const uint32_t rec = s1 * 0x90u + t1 + 0x54u;
            if (!G::plain(rec, 4u) || !G::plain(g.u32(rec) + s3 * 4u, 4u))
                return false;
            const uint32_t si = g.u32(g.u32(rec) + s3 * 4u);
            const uint32_t s2 = work + 0x70u;
            bndFlag = g.u32(s2 + 8u) + si;
            bnd = g.u32(s2 + 0x14u) + si * 16u;
        }
        if (!G::plain(bndFlag, 1u) || !G::plain(bnd, 16u) || (bnd & 0xFu) != 0u)
            return false;
        // the plane, made as ClipCollisionDB makes it when not cached
        if (g.rdram[flag] == 0u)
        {
            if (indep)
            {
                const uint32_t s2 = g.u32(work + 0x24u) + s1 * 0x60u + 0x30u;
                verticesIndep(g, sp, sp + 0x10u, cls, s1, s3, 0x0111BAA8u, sp);
                plane(g, sp, normal);
                entryModified(g, s2, s3, 0x0111BC58u, sp);
            }
            else
            {
                const uint32_t t2 = g.u32(g.u32(cls + 0x184u) + 0x20u);
                const uint32_t s2 = work + 0x58u;
                const uint32_t index = g.u32(g.u32(s1 * 0x90u + g.u32(t2 + 8u) + 0x54u) + s3 * 4u);
                call(g, kVertices, 0x0111BBC0u, sp,
                     {sp + 0x20u, sp + 0x30u, work + 0x28u, g.u32(work + 0x24u), g.u32(cls + 0x15Cu), g.u32(t2 + 8u), g.u32(t2 + 0x14u), g.u32(t2 + 0x18u) + index * 0x40u});
                plane(g, sp + 0x20u, normal);
                entryModified(g, s2, index, 0x0111BC58u, sp);
            }
            if (g.rdram[flag] == 0u)
                return false;
        }
        // the boundary sphere (centre, r^2) as GetCollisionDBSurfBoundary gives it
        surfBoundary(g, sp + 0x10u, id, 0x0111BD64u, sp);
        if (g.rdram[bndFlag] == 0u)
            return false;
        // the three vertices
        if (indep)
            verticesIndep(g, sp + 0x80u, 0u, cls, s1, s3, 0x0111BDE8u, sp);
        else
        {
            const uint32_t t4 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            const uint32_t idx = g.u32(g.u32(s1 * 0x90u + g.u32(t4 + 8u) + 0x54u) + s3 * 4u);
            call(g, kVertices, 0x0111BE48u, sp,
                 {sp + 0x80u, 0u, work + 0x28u, g.u32(work + 0x24u), g.u32(cls + 0x15Cu), g.u32(t4 + 8u), g.u32(t4 + 0x14u), g.u32(t4 + 0x18u) + idx * 0x40u});
        }
        triangleTerms(e, g.q(g.u32(sp + 0x80u)), g.q(g.u32(sp + 0x84u)), g.q(g.u32(sp + 0x88u)), ctx->vu0_vf[0]);
        e.n = g.q(normal);
        e.s = g.q(sp + 0x10u);
        m.planeFlag = flag;
        m.plane = normal;
        m.bndFlag = bndFlag;
        m.bnd = bnd;
        std::memcpy(m.bndW, g.rdram + bnd, 16);
        m.indep = indep ? 1u : 0u;
        m.hasAttr = 0u;
        m.state = kHost;
        {
            uint32_t attr;
            if (attributeAddress(g, id, attr))
            {
                m.attr = attr;
                m.hasAttr = 1u;
            }
        }
        (void)ctx;
        ++g_st.made;
        return true;
    }

    // The hit of entry e for the segment a -> b: ClipCollisionDB's tests after its lookups, the same operations
    // on the same values (its plane test in round 49's NEON form, the crossing, the sphere, u / v with the PS2's
    // FPU / VU0 macros), from the kept plane, sphere and vertices: 1 hit (p, u, v as ClipCollisionDB writes them),
    // 0 no hit, -1 not decided here (a lane not small: the caller makes the call).
    inline int clip(R5900Context *ctx, const Entry &e, __m128 a, __m128 b, __m128 &p, float &u, float &v)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        float d0, d1;
        const float zero = 0.0f;
#if PS2X_VU_NEON_A32
        bool small = false;
        const uint64_t dBits = fast::planeDistances(e.n, vf0, a, b, small);
        if (__builtin_expect(!small, 0))
            return -1;
        if (fast::belowZero(static_cast<uint32_t>(dBits)) || fast::aboveZero(static_cast<uint32_t>(dBits >> 32)))
            return 0;
        d0 = fast::asFloat(static_cast<uint32_t>(dBits));
        d1 = fast::asFloat(static_cast<uint32_t>(dBits >> 32));
#else
        const __m128 n = vu::clamp(e.n), vf0c = vu::clamp(vf0);
        auto distance = [&](__m128 q)
        {
            const __m128 prod = vu::mulN(n, vu::clamp(q));
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            const __m128 d = vu::addN(acc, lane<2>(prod));
            return x(vu::addN(d, lane<3>(n)));
        };
        d0 = distance(a);
        if (FPU_C_OLT_S(d0, zero))
            return 0;
        d1 = distance(b);
        if (FPU_C_OLT_S(zero, d1))
            return 0;
#endif
        ++g_st.planePass;
        const float span = FPU_SUB_S(d1, d0);
        const bool atStart = FPU_C_OLT_S(span, zero) ? FPU_C_OLT_S(-0.0004999999f, span) : FPU_C_OLT_S(span, 0.0004999999f);
        if (atStart)
            p = a;
        else
        {
            const float t = ps2FpuDivide(ctx->fcr31, d1, span);
            const __m128 vt = _mm_set1_ps(t);
            const __m128 oneMinus = PS2_VSUB(vf0, lane<0>(vt));
            const __m128 acc = PS2_VMUL(a, lane<0>(vt));
            p = setXyz(a, PS2_VADD(acc, PS2_VMUL(b, lane<3>(oneMinus))));
        }
        {
            __m128 dv = PS2_VSUB(e.s, p);
            dv = setXyz(dv, PS2_VMUL(dv, dv));
            dv = setLane0(dv, PS2_VADD(dv, lane<1>(dv)));
            const float dist2 = x(PS2_VADD(dv, lane<2>(dv)));
            float r2;
            std::memcpy(&r2, reinterpret_cast<const float *>(&e.s) + 3, 4);
            if (FPU_C_OLT_S(r2, dist2))
                return 0;
        }
        // u / v: the triangle's terms made once (triangleTerms), the crossing's side here, as ClipCollisionDB
        const __m128 vf0c = vu::clamp(vf0);
        const __m128 w = vu::clamp(PS2_VSUB(p, e.v0));
        auto dot = [&](__m128 a, __m128 b)
        {
            const __m128 prod = vu::mulN(a, b);
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            return x(vu::addN(acc, lane<2>(prod)));
        };
        u = ps2FpuDivide(ctx->fcr31, dot(e.c2, w), e.d2);
        v = ps2FpuDivide(ctx->fcr31, dot(e.c1, w), e.d1);
        const float lo = -2e-05f;
        if (FPU_C_OLT_S(u, lo) || FPU_C_OLT_S(v, lo))
            return 0;
        const float sum = FPU_SUB_S(FPU_ADD_S(u, v), 1.0f);
        return FPU_C_OLT_S(2e-05f, sum) ? 0 : 1;
    }

    inline uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    // ClipCollisionFinder / ...CallBack's loop on the surface cache (see above): a0 the record, a1 / a2
    // the segment, a3 the finder, n its count (1..256), sp the clip's frame (the segment's copy at sp
    // as the original keeps it). Returns the last hit's entry address (0: none); the record as the last
    // ClipCollisionDB that hit leaves it.
    template <bool Filtered>
    uint32_t run(const G g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, int32_t n, uint32_t filter, uint32_t user, uint32_t sp, uint32_t filterSite,
                 uint32_t clipSite)
    {
        ++g_st.calls;
        const uint32_t list = g.u32(a3 + 0xCu);
        const uint64_t frame = sotc_decomp::g_gameFrame;
        const __m128 a = g.q(a1);
        __m128 b = g.q(a2);
        g.put(sp, b);
        bool guestB = true; // the guest copy at sp is b
        uint32_t best = 0u;
        bool bestHost = false; // the last hit made here (not by ClipCollisionDB): its id words and kind, copied (a later
        uint32_t bestW0 = 0u, bestW1 = 0u;   // entry's slot may take its slot over)
        bool bestIndep = false;
        float bu = 0.0f, bv = 0.0f;
        uint32_t cand = list;
        for (int32_t k = 0; k < n; ++k, cand += 8u)
        {
            uint32_t w[2];
            std::memcpy(w, g.rdram + cand, 8);
            if (w[0] == 0u)
                continue;
            bool fresh;
            Surf &x = claimSurf(w[0], w[1], fresh);
            Meta &m = x.m;
            Entry &e = x.e;
            if (!fresh && !x.current(frame))
            {
                // checked again since the caches may have changed (a flush, a new frame)
                ++g_st.checks;
                if (m.state == kHost && stillGood(g, m, e))
                    x.stamp(frame, g_epoch);
                else
                {
                    if (m.state == kHost)
                        ++g_st.dropped;
                    fresh = true;
                }
            }
            if (fresh)
            {
                if (m.state == kUnmade && x.frame != 0u)
                    ++g_st.evicted;
                // (stamped with the epoch before: a flush made by a call in it has the entry checked at its next use)
                const uint64_t ep = g_epoch;
                const uint32_t we = 0u;
                (void)we;
                x.work = nullptr;
                make(g, e, m, cand, sp);
                {
                    uint32_t cls;
                    if (m.state == kHost && layoutClass(g, m.w0, cls) && cls != 0u && G::plain(cls + 0x174u, 4u))
                        x.work = addWork(g.u32(cls + 0x174u));
                }
                x.stamp(frame, ep);
            }
            ++g_st.entries;
            __m128 p;
            float u = 0.0f, v = 0.0f;
            const int r = m.state == kHost ? clip(g.ctx, e, a, b, p, u, v) : -1;
            if (r < 0)
            {
                ++g_st.guestEntries;
                if (Filtered && !directFilter(g, filter, cand, user, filterSite + 8u, sp))
                    continue;
                if (!guestB)
                {
                    g.put(sp, b);
                    guestB = true;
                }
                if (clipDB(g, a0, a1, sp, cand, clipSite + 8u, sp) == 0u)
                    continue;
                b = g.q(a0 + 0x40u);
                g.put(sp, b);
                best = cand;
                bestHost = false;
                continue;
            }
            if (r == 0)
                continue;
            if (Filtered && !directFilter(g, filter, cand, user, filterSite + 8u, sp))
            {
                ++g_st.filtered;
                continue;
            }
            ++g_st.hits;
            b = p;
            guestB = false;
            best = cand;
            bestHost = true;
            bestW0 = m.w0;
            bestW1 = m.w1;
            bestIndep = m.indep != 0u;
            bu = u;
            bv = v;
        }
        if (bestHost)
        {
            const __m128 pt = b;
            g.put(a0 + 0x40u, pt);
            g.put32(a0 + 8u, fbits(bu));
            g.put32(a0 + 0xCu, fbits(bv));
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
        return best;
    }

    // The keep pass (ClipCollisionFinderWithKeep, decomp_hot44b): whether clipKeep's first return (the point at
    // least `keep` in front of the surface's plane) answers the entry at `id`, from the surface cache
    // (checked this frame and epoch, the entry made from the same id words): 1 yes, 0 no, -1 not known here.
    int keepEarly125(const uint8_t *rdram, const R5900Context *ctx, const PS2Runtime *runtime, uint32_t id, uint32_t point, float keep)
    {
        if (!on(runtime) || !G::plain(point & ~0xFu, 16u) || !G::plain(id, 8u))
            return -1;
        uint32_t w[2];
        std::memcpy(w, rdram + id, 8);
        const Surf *x = findSurf(w[0], w[1]);
        if (!x || x->m.state != kHost || !x->current(sotc_decomp::g_gameFrame))
            return -1;
        {
            // clipKeep's first test (its round 49 NEON form) on the kept plane
#if PS2X_VU_NEON_A32
            __m128 pa;
            std::memcpy(&pa, rdram + (point & ~0xFu), 16);
            bool small = false;
            const uint64_t bits = fast::planeDistances(x->e.n, ctx->vu0_vf[0], pa, pa, small);
            if (!small)
                return -1;
            ++g_st.keepHost;
            const float ahead = FPU_SUB_S(fast::asFloat(static_cast<uint32_t>(bits)), keep), zero = 0.0f;
            return FPU_C_OLE_S(zero, ahead) ? 1 : 0;
#else
            (void)keep, (void)ctx;
            return -1;
#endif
        }
    }

    // GetCollisionDBAttributeAdr for a kept surface (checked this frame): the address kept at its making.
    bool attrOf(const PS2Runtime *runtime, uint32_t w0, uint32_t w1, uint32_t &out)
    {
        if (!on(runtime))
            return false;
        const Surf *x = findSurf(w0, w1);
        if (!x || x->m.state != kHost || !x->m.hasAttr || !x->current(sotc_decomp::g_gameFrame))
            return false;
        out = x->m.attr;
        ++g_st.attrHost;
        return true;
    }
}
