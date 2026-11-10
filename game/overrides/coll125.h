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
// asks the cache too (keepEarly125). Round 135: GetCollisionDBAttributeAdr no longer does (see validate()).
//
// Lifetime: the game's caches change only through FlushModifiedManager (an
// object that moved has its cache flags cleared). A flush made by
// FlushCollisionWorkByWorkStruct moves that collision work's epoch (g_works:
// only the works some kept surface belongs to), any other flush on the game
// thread the global g_epoch, any flush on the worker's lane g_epochW. A slot
// is used as kept while its frame and the three epochs are the ones it was
// checked at; else it is checked again (validate()) or made again.
//
// Round 135 (the r131 check's missed hits, two causes): (1) round 125's check
// looked at the plane and boundary flags and values at the addresses kept at
// the making. An object's slot taken over by another (the area loads; the id
// words, the layout class the same, its collision work / record not) left
// those bytes as they were, so the old surface passed. validate() now walks
// the chain again from the id words (layout class, work, record, every flag /
// slot address, the vertex cache list) and compares everything ClipCollisionDB
// reads: the plane / boundary / three vertex flags and the plane, boundary and
// vertex qwords. (2) The attribute address kept at the making (asked by the id
// filters through GetCollisionDBAttributeAdr) went stale while the surface's
// geometry stayed: Agro's surfaces, the IK feet's filter (character_ikcolattr
// filter) answered "no" from the old record where the game's own lookup says
// "yes", so a feet probe missed one of Agro's surfaces.
// GetCollisionDBAttributeAdr now always asks the game's tables.
// FlushModifiedManager's native also moves the worker epoch at its end (a
// check between a worker flush's start and its last cleared flag is not kept).
//
// Game thread only (the worker's lane makes every call). Logs: SOTC_COLL115_LOG=1
// adds [fin125] (clips, entries, plane passes, hits, made, checks, dropped, keep
// tests), [fin125e] (flushes by kind) and [fin125t] (each clip's host time by
// the switch's state). SOTC_FIN125_CHECK=1: each clip the original way first
// (its answer used), compared ([fin125m]); round 135 adds [fin135] (every use
// of a kept surface validated in full: "cur bad" = changed with no flush seen,
// "r125 missed" = what round 125's check let through) and, for a differing
// clip, [fin135x] / [fin135t] / [fin135f] / [fin135c] (the slot, what the run
// did with the entry, the filter both ways, the clip both ways).
// Round 138 (SOTC_FIN138, default on): validate138 reads the chain's links at
// their kept addresses (all asked for at once, the next entry's ahead) instead
// of walking them one dependent load at a time; the check mode compares it with
// validate() on every use ([fin138]: unsafe must be 0). SOTC_FIN138_LOG=1: the
// finder clips' host time by state ([fin138t], decomp_collision.cpp).
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

    // Round 135: FlushModifiedManager's native at its end on the worker's lane: the worker epoch moved again, so a
    // surface the game thread checked between the flush's start and its last cleared flag is checked again.
    inline void flushedEnd(const PS2Runtime *runtime)
    {
        if (sotc_decomp::onWorkerLane(runtime))
            g_epochW.store(g_epochW.load(std::memory_order_relaxed) + 1u, std::memory_order_relaxed);
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
        // Round 135: everything ClipCollisionDB reads for it, kept to check it again: the chain's heads (the layout
        // class, its collision work and record), the vertex cache list (+8 the flags, +0x14 the table) and the three
        // vertices' indices in it, their flag and qword addresses and the qwords as made (the triangle terms' input).
        uint32_t cls, work, rec, vc;
        uint32_t vid[3], vflag[3], vtx[3];
        uint32_t vW[12];
        // Round 138: the chain's links as read at the making (Addr's), for validate138.
        uint32_t partTab, t2, t2v8, rv, index;
        uint8_t type;
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
    struct Check135
    {
        uint64_t uses = 0, curBad = 0, r125Missed = 0, checks = 0;
        uint64_t curWhy[6] = {}, missWhy[6] = {};
    };
    inline Check135 g_c135;
    struct Check138
    {
        uint64_t unsafe = 0, extra = 0, bothBad = 0;
    };
    inline Check138 g_c138;
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
                         "checks %.2f dropped %.2f big lists %.2f keep tests %.2f | check %llu differ %llu (hit / no hit %llu, other id %llu, point max %.2f mm)\n",
                         (unsigned long long)frame, s.calls / nf, s.entries / nf, s.planePass / nf, s.hits / nf, s.filtered / nf, s.guestEntries / nf, s.made / nf,
                         s.evicted / nf, s.checks / nf, s.dropped / nf, s.bigLists / nf, s.keepHost / nf, (unsigned long long)s.checked, (unsigned long long)s.differ,
                         (unsigned long long)s.differHit, (unsigned long long)s.differId, s.maxd * 1000.0);
            for (int i = 0; i < 2; ++i)
                if (s.nq[i])
                    std::fprintf(stderr, "[fin125t] frame %llu finder clips %s: %llu calls, %.2f us each (%.3f ms a frame)\n", (unsigned long long)frame, i ? "on" : "off",
                                 (unsigned long long)s.nq[i], (double)s.us[i] / s.nq[i], (double)s.us[i] / 1000.0 / nf);
            std::fprintf(stderr, "[fin125e] frame %llu: flushes a frame: by work %.1f, other %.2f; works kept %u\n", (unsigned long long)frame, g_workBumps / nf,
                         g_globalBumps / nf, [] { unsigned n = 0; for (const WorkEpoch &w : g_works) n += w.work != 0u; return n; }());
            if (checkLevel() != 0)
            {
                const Check135 &c = g_c135;
                std::fprintf(stderr,
                             "[fin135] frame %llu: uses %llu checks %llu | cur bad %llu (chain %llu addr %llu flag %llu plane %llu bnd %llu verts %llu) | r125 missed %llu (chain %llu addr "
                             "%llu flag %llu plane %llu bnd %llu verts %llu)\n",
                             (unsigned long long)frame, (unsigned long long)c.uses, (unsigned long long)c.checks, (unsigned long long)c.curBad, (unsigned long long)c.curWhy[0],
                             (unsigned long long)c.curWhy[1], (unsigned long long)c.curWhy[2], (unsigned long long)c.curWhy[3], (unsigned long long)c.curWhy[4],
                             (unsigned long long)c.curWhy[5], (unsigned long long)c.r125Missed, (unsigned long long)c.missWhy[0], (unsigned long long)c.missWhy[1],
                             (unsigned long long)c.missWhy[2], (unsigned long long)c.missWhy[3], (unsigned long long)c.missWhy[4], (unsigned long long)c.missWhy[5]);
                std::fprintf(stderr, "[fin138] frame %llu: validate138 vs validate on %llu uses: unsafe %llu extra %llu both bad %llu\n", (unsigned long long)frame,
                             (unsigned long long)c.uses, (unsigned long long)g_c138.unsafe, (unsigned long long)g_c138.extra, (unsigned long long)g_c138.bothBad);
            }
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
    // Round 135: the addresses ClipCollisionDB reaches for the surface (w0, w1) without calls: the layout class,
    // its collision work and record, the plane / boundary flags and slots, the vertex cache list. False: a class
    // made by a call, none, or an address outside main RAM.
    struct Addr
    {
        uint32_t cls, work, rec, planeFlag, plane, bndFlag, bnd, vc;
        // Round 138: the chain's links as read (validate138 reads each one again at its kept address, all at once):
        // a kept surface: the part table (+0x24 of the work); else the record's +0x20, its +8, the slot list and the index.
        uint32_t partTab, t2, t2v8, rv, index;
        uint8_t type; // the object's +8 (layoutClass's switch)
    };
    inline bool derive(const G g, uint32_t w0, uint32_t w1, Addr &a)
    {
        if (!layoutClass(g, w0, a.cls) || a.cls == 0u || !G::plain(a.cls, 0x200u))
            return false;
        a.type = g.u8(w0 + 8u);
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        a.work = g.u32(a.cls + 0x174u);
        a.rec = g.u32(a.cls + 0x184u);
        if (!G::plain(a.work, 0x88u) || !G::plain(a.rec, 0x24u))
            return false;
        if (static_cast<int32_t>(w1) < 0)
        {
            a.partTab = g.u32(a.work + 0x24u);
            a.t2 = a.t2v8 = a.rv = a.index = 0u;
            const uint32_t part = a.partTab + s1 * 0x60u;
            if (!G::plain(part, 0x60u))
                return false;
            a.planeFlag = g.u32(part + 0x38u) + s3;
            a.plane = g.u32(part + 0x44u) + s3 * 16u;
            a.bndFlag = g.u32(part + 0x50u) + s3;
            a.bnd = g.u32(part + 0x5Cu) + s3 * 16u;
            a.vc = part;
        }
        else
        {
            const uint32_t t2 = g.u32(a.rec + 0x20u);
            a.partTab = 0u;
            a.t2 = t2;
            if (!G::plain(t2, 0x10u))
                return false;
            a.t2v8 = g.u32(t2 + 8u);
            const uint32_t r = s1 * 0x90u + a.t2v8 + 0x54u;
            if (!G::plain(r, 4u))
                return false;
            a.rv = g.u32(r);
            const uint32_t at = a.rv + s3 * 4u;
            if (!G::plain(at, 4u))
                return false;
            const uint32_t index = g.u32(at);
            a.index = index;
            a.planeFlag = g.u32(a.work + 0x60u) + index;
            a.plane = g.u32(a.work + 0x6Cu) + index * 16u;
            a.bndFlag = g.u32(a.work + 0x78u) + index;
            a.bnd = g.u32(a.work + 0x84u) + index * 16u;
            a.vc = a.work + 0x28u;
        }
        return G::plain(a.planeFlag, 1u) && G::plain(a.bndFlag, 1u) && G::plain(a.plane, 16u) && G::plain(a.bnd, 16u) && (a.plane & 0xFu) == 0u &&
               (a.bnd & 0xFu) == 0u;
    }
    enum : unsigned
    {
        kBadChain = 1u,  // the id words reach another class / work / record (the object went, another took its place)
        kBadAddr = 2u,   // a flag / slot / vertex cache list moved
        kBadFlag = 4u,   // a cache flag cleared (a flush)
        kBadPlane = 8u,  // the plane's value changed
        kBadBnd = 16u,   // the boundary's value changed
        kBadVerts = 32u, // a vertex's value changed (a flush of the vertex caches only, a skinned surface's blend)
    };
    // Round 135: what ClipCollisionDB would read for the surface now against what was kept (0: the same: its answers
    // are the cache's for any segment). The chain again from the id words, every flag and every value it reads.
    inline unsigned validate(const G g, const Meta &m, const Entry &e)
    {
        Addr a;
        if (!derive(g, m.w0, m.w1, a) || a.cls != m.cls || a.work != m.work || a.rec != m.rec)
            return kBadChain;
        if (a.planeFlag != m.planeFlag || a.plane != m.plane || a.bndFlag != m.bndFlag || a.bnd != m.bnd || a.vc != m.vc)
            return kBadAddr;
        const uint32_t vfl = g.u32(m.vc + 8u), vtab = g.u32(m.vc + 0x14u);
        for (int k = 0; k < 3; ++k)
            if (vfl + m.vid[k] != m.vflag[k] || vtab + m.vid[k] * 16u != m.vtx[k])
                return kBadAddr;
        unsigned bad = 0u;
        if (g.rdram[m.planeFlag] == 0u || g.rdram[m.bndFlag] == 0u || g.rdram[m.vflag[0]] == 0u || g.rdram[m.vflag[1]] == 0u || g.rdram[m.vflag[2]] == 0u)
            bad |= kBadFlag;
        if (std::memcmp(g.rdram + m.plane, &e.n, 16) != 0)
            bad |= kBadPlane;
        if (std::memcmp(g.rdram + m.bnd, m.bndW, 16) != 0)
            bad |= kBadBnd;
        if (std::memcmp(g.rdram + m.vtx[0], m.vW, 16) != 0 || std::memcmp(g.rdram + m.vtx[1], m.vW + 4, 16) != 0 || std::memcmp(g.rdram + m.vtx[2], m.vW + 8, 16) != 0)
            bad |= kBadVerts;
        return bad;
    }
    // Round 125's check (the flags and the plane / boundary values only), kept for the check mode's comparison.
    inline bool stillGood125(const G g, const Meta &m, const Entry &e)
    {
        if (g.rdram[m.planeFlag] == 0u || g.rdram[m.bndFlag] == 0u)
            return false;
        if (std::memcmp(g.rdram + m.plane, &e.n, 16) != 0)
            return false;
        return std::memcmp(g.rdram + m.bnd, m.bndW, 16) == 0;
    }

    // Round 138 (SOTC_FIN138, default on; A/B fin138): validate()'s answer without its chain of dependent loads.
    // validate() walks the chain from the id words (the object's +8, the class's work / record, the record's
    // tables, the slot list, the index: five to seven loads, each one's address from the one before, most of them
    // a miss on the A9) and then reads the flags and values. Here every link is read at the address it had at the
    // making (kept in Meta) and compared with the value it had: all the addresses are known before any load, so the
    // lines are asked for together (PLD) and wait on memory once instead of once per link. Exact: when every link
    // reads as kept, derive() from the id words now follows the same addresses and gives the kept Addr, so the
    // flag / value compares below (validate()'s own) decide the same; when a link differs the surface is made again
    // (validate() might have found the same addresses on another path: a remake, never a wrong answer).
    inline void prefetch138(const G g, const Meta &m)
    {
        const uint8_t *r = g.rdram;
        __builtin_prefetch(r + m.w0 + 8u);
        __builtin_prefetch(r + m.cls + 0x174u);
        __builtin_prefetch(r + m.cls + 0x184u);
        if (m.indep)
        {
            __builtin_prefetch(r + m.work + 0x24u);
            __builtin_prefetch(r + m.vc + 0x8u);
            __builtin_prefetch(r + m.vc + 0x38u);
            __builtin_prefetch(r + m.vc + 0x5Cu);
        }
        else
        {
            const uint32_t s3 = m.w1 & 0x00FFFFFFu, s1 = (m.w1 >> 24) & 0x7Fu;
            __builtin_prefetch(r + m.rec + 0x20u);
            __builtin_prefetch(r + m.t2 + 8u);
            __builtin_prefetch(r + s1 * 0x90u + m.t2v8 + 0x54u);
            __builtin_prefetch(r + m.rv + s3 * 4u);
            __builtin_prefetch(r + m.work + 0x30u);
            __builtin_prefetch(r + m.work + 0x60u);
            __builtin_prefetch(r + m.work + 0x84u);
        }
        __builtin_prefetch(r + m.planeFlag);
        __builtin_prefetch(r + m.bndFlag);
        __builtin_prefetch(r + m.vflag[0]);
        __builtin_prefetch(r + m.vflag[1]);
        __builtin_prefetch(r + m.vflag[2]);
        __builtin_prefetch(r + m.plane);
        __builtin_prefetch(r + m.bnd);
        __builtin_prefetch(r + m.vtx[0]);
        __builtin_prefetch(r + m.vtx[1]);
        __builtin_prefetch(r + m.vtx[2]);
    }
    inline unsigned validate138(const G g, const Meta &m, const Entry &e)
    {
        prefetch138(g, m);
        const uint32_t s3 = m.w1 & 0x00FFFFFFu, s1 = (m.w1 >> 24) & 0x7Fu;
        uint32_t cls = 0u;
        bool chain = g.u8(m.w0 + 8u) == m.type && layoutClass(g, m.w0, cls) && cls == m.cls;
        chain = chain & (g.u32(m.cls + 0x174u) == m.work) & (g.u32(m.cls + 0x184u) == m.rec);
        bool addr;
        if (m.indep)
        {
            chain = chain & (g.u32(m.work + 0x24u) == m.partTab);
            const uint32_t part = m.partTab + s1 * 0x60u;
            addr = (part == m.vc) & (g.u32(part + 0x38u) + s3 == m.planeFlag) & (g.u32(part + 0x44u) + s3 * 16u == m.plane) &
                   (g.u32(part + 0x50u) + s3 == m.bndFlag) & (g.u32(part + 0x5Cu) + s3 * 16u == m.bnd);
        }
        else
        {
            chain = chain & (g.u32(m.rec + 0x20u) == m.t2) & (g.u32(m.t2 + 8u) == m.t2v8) & (g.u32(s1 * 0x90u + m.t2v8 + 0x54u) == m.rv) &
                    (g.u32(m.rv + s3 * 4u) == m.index);
            const uint32_t index = m.index;
            addr = (m.work + 0x28u == m.vc) & (g.u32(m.work + 0x60u) + index == m.planeFlag) & (g.u32(m.work + 0x6Cu) + index * 16u == m.plane) &
                   (g.u32(m.work + 0x78u) + index == m.bndFlag) & (g.u32(m.work + 0x84u) + index * 16u == m.bnd);
        }
        if (!chain)
            return kBadChain;
        const uint32_t vfl = g.u32(m.vc + 8u), vtab = g.u32(m.vc + 0x14u);
        for (int k = 0; k < 3; ++k)
            addr = addr & (vfl + m.vid[k] == m.vflag[k]) & (vtab + m.vid[k] * 16u == m.vtx[k]);
        if (!addr)
            return kBadAddr;
        unsigned bad = 0u;
        if ((g.rdram[m.planeFlag] == 0u) | (g.rdram[m.bndFlag] == 0u) | (g.rdram[m.vflag[0]] == 0u) | (g.rdram[m.vflag[1]] == 0u) | (g.rdram[m.vflag[2]] == 0u))
            bad |= kBadFlag;
        if (std::memcmp(g.rdram + m.plane, &e.n, 16) != 0)
            bad |= kBadPlane;
        if (std::memcmp(g.rdram + m.bnd, m.bndW, 16) != 0)
            bad |= kBadBnd;
        if ((std::memcmp(g.rdram + m.vtx[0], m.vW, 16) != 0) | (std::memcmp(g.rdram + m.vtx[1], m.vW + 4, 16) != 0) | (std::memcmp(g.rdram + m.vtx[2], m.vW + 8, 16) != 0))
            bad |= kBadVerts;
        return bad;
    }
    inline bool on138() { return sotc_decomp::switchOn(sotc_decomp::kFin138); }
    inline bool stillGood(const G g, const Meta &m, const Entry &e) { return (on138() ? validate138(g, m, e) : validate(g, m, e)) == 0u; }

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
        // Round 135: the vertices' places in their cache list and the chain's heads, for validate(); a vertex outside
        // the list's table (or its flag clear after the call) leaves the entry to ClipCollisionDB.
        {
            Addr a;
            if (!derive(g, w0, w1, a) || a.planeFlag != flag || a.plane != normal || a.bndFlag != bndFlag || a.bnd != bnd)
                return false;
            m.cls = a.cls;
            m.work = a.work;
            m.rec = a.rec;
            m.vc = a.vc;
            m.partTab = a.partTab;
            m.t2 = a.t2;
            m.t2v8 = a.t2v8;
            m.rv = a.rv;
            m.index = a.index;
            m.type = a.type;
            const uint32_t vfl = g.u32(a.vc + 8u), vtab = g.u32(a.vc + 0x14u);
            for (uint32_t k = 0; k < 3u; ++k)
            {
                const uint32_t ptr = g.u32(sp + 0x80u + k * 4u);
                if (ptr < vtab || ((ptr - vtab) & 0xFu) != 0u || !G::plain(ptr, 16u))
                    return false;
                m.vid[k] = (ptr - vtab) >> 4;
                m.vflag[k] = vfl + m.vid[k];
                m.vtx[k] = ptr;
                if (!G::plain(m.vflag[k], 1u) || g.rdram[m.vflag[k]] == 0u)
                    return false;
                std::memcpy(m.vW + k * 4u, g.rdram + ptr, 16);
            }
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
        m.hasAttr = 0u; // (round 135: no attribute address kept; GetCollisionDBAttributeAdr always asks the game's tables)
        m.state = kHost;
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

    // Round 135, the check mode (SOTC_FIN125_CHECK=1): each use of a kept surface validated in full. A surface used as
    // current (no flush since its check this frame) that is not what ClipCollisionDB would read is a change no flush
    // reported ([fin135] cur bad); one round 125's check (flags, plane, boundary) passed and the full one does not is
    // what round 125 missed ([fin135] r125 missed). Counted by reason (validate's bits).
    inline void checkUse(const G g, const Surf &x, uint64_t frame)
    {
        ++g_c135.uses;
        const unsigned bad = validate(g, x.m, x.e);
        {
            // Round 138: validate138 against validate on every use: "unsafe" (138 good where validate is not) must be 0;
            // "extra" (138 bad where validate is good: a remake, not a wrong answer) is counted.
            const unsigned b138 = validate138(g, x.m, x.e);
            if (b138 == 0u && bad != 0u)
            {
                ++g_c138.unsafe;
                if (g_c138.unsafe <= 20u)
                    std::fprintf(stderr, "[fin138] frame %llu UNSAFE: validate %x, validate138 good: surface %08x %08x\n", (unsigned long long)frame, bad, x.m.w0, x.m.w1);
            }
            else if (b138 != 0u && bad == 0u)
                ++g_c138.extra;
            else if (b138 != 0u)
                ++g_c138.bothBad;
        }
        auto why = [&](uint64_t *to)
        {
            for (int b = 0; b < 6; ++b)
                if (bad & (1u << b))
                    ++to[b];
        };
        if (x.current(frame))
        {
            if (bad != 0u)
            {
                ++g_c135.curBad;
                why(g_c135.curWhy);
                if (g_c135.curBad <= 20u)
                    std::fprintf(stderr, "[fin135] frame %llu cur bad %x: surface %08x %08x cls %08x work %08x\n", (unsigned long long)frame, bad, x.m.w0, x.m.w1, x.m.cls, x.m.work);
            }
            return;
        }
        ++g_c135.checks;
        if (bad != 0u && stillGood125(g, x.m, x.e))
        {
            ++g_c135.r125Missed;
            why(g_c135.missWhy);
            if (g_c135.r125Missed <= 20u)
                std::fprintf(stderr, "[fin135] frame %llu r125 missed %x: surface %08x %08x cls %08x work %08x indep %d (checked at frame %llu)\n", (unsigned long long)frame, bad,
                             x.m.w0, x.m.w1, x.m.cls, x.m.work, x.m.indep, (unsigned long long)x.frame);
        }
    }

    // Round 135, the check mode: why the host answered other than the original for the entry at `id` (the
    // original's hit): its slot, whether it was current, validate()'s bits and round 125's check.
    inline void explain(const G g, uint32_t id, const char *what)
    {
        uint32_t w[2];
        std::memcpy(w, g.rdram + id, 8);
        const Surf *x = findSurf(w[0], w[1]);
        if (!x)
        {
            std::fprintf(stderr, "[fin135x] %s: surface %08x %08x not kept\n", what, w[0], w[1]);
            return;
        }
        const uint64_t frame = sotc_decomp::g_gameFrame;
        std::fprintf(stderr, "[fin135x] %s: surface %08x %08x state %d indep %d current %d (checked at %llu, epoch %d work %d worker %d) validate %x r125 %d cls %08x type %u\n", what,
                     w[0], w[1], x->m.state, x->m.indep, x->m.state == kHost && x->current(frame) ? 1 : 0, (unsigned long long)x->frame, x->epoch == g_epoch ? 1 : 0,
                     x->work && x->work->epoch == x->workEpoch ? 1 : 0, x->epochW == g_epochW.load(std::memory_order_relaxed) ? 1 : 0,
                     x->m.state == kHost ? validate(g, x->m, x->e) : 0xFFu, x->m.state == kHost && stillGood125(g, x->m, x->e) ? 1 : 0, x->m.cls,
                     G::plain(w[0], 16u) ? g.u8(w[0] + 8u) : 0u);
    }

    // Round 135, the check mode: the entry at `id` clipped now both ways on the segment a1 -> a2: the kept surface
    // (clip) and ClipCollisionDB itself into a scratch record below the clip's frame (dead stack), with the values.
    inline void explainClip(const G g, uint32_t id, uint32_t a1, uint32_t a2, uint32_t sp)
    {
        uint32_t w[2];
        std::memcpy(w, g.rdram + id, 8);
        const Surf *x = findSurf(w[0], w[1]);
        const __m128 a = g.q(a1), b = g.q(a2);
        __m128 p = a;
        float u = 0.0f, v = 0.0f;
        const int r = x && x->m.state == kHost ? clip(g.ctx, x->e, a, b, p, u, v) : -2;
        const uint32_t scratch = (sp - 0x400u) & ~0xFu;
        const uint64_t db = clipDB(g, scratch, a1, a2, id, 0x01125E34u + 8u, scratch - 0x10u);
        float ua = 0.0f, va = 0.0f, pa[4] = {}, pq[4] = {}, aq[4], bq[4], n[4] = {}, gn[4] = {};
        std::memcpy(&ua, g.rdram + scratch + 8u, 4);
        std::memcpy(&va, g.rdram + scratch + 0xCu, 4);
        std::memcpy(pa, g.rdram + scratch + 0x40u, 16);
        std::memcpy(pq, &p, 16);
        std::memcpy(aq, &a, 16);
        std::memcpy(bq, &b, 16);
        if (x)
        {
            std::memcpy(n, &x->e.n, 16);
            std::memcpy(gn, g.rdram + x->m.plane, 16);
        }
        std::fprintf(stderr,
                     "[fin135c] surface %08x %08x: kept clip %d (u %.6f v %.6f p %.4f %.4f %.4f) | ClipCollisionDB %d (u %.6f v %.6f p %.4f %.4f %.4f) | a %.4f %.4f %.4f b %.4f %.4f %.4f | "
                     "plane kept %.6f %.6f %.6f %.6f guest %.6f %.6f %.6f %.6f\n",
                     w[0], w[1], r, u, v, pq[0], pq[1], pq[2], (int)db, ua, va, pa[0], pa[1], pa[2], aq[0], aq[1], aq[2], bq[0], bq[1], bq[2], n[0], n[1], n[2], n[3], gn[0],
                     gn[1], gn[2], gn[3]);
    }

    // Round 135, the check mode: what the last run did per entry (printed for a differing entry).
    struct Trace
    {
        uint32_t cand, w0, w1;
        int8_t r;
        uint8_t fresh, wasCurrent, state, filtered, made;
        float n[4];
    };
    inline Trace g_trace[257];
    inline int g_traceN = 0;
    // The filter asked again for the entry at `id`: as the host run asks it (fin125 on: attributeAddress answers from
    // the cache) and as the original does (fin125 off), and the attribute address both ways.
    inline void explainFilter(const G g, uint32_t filter, uint32_t id, uint32_t user, uint32_t site, uint32_t sp)
    {
        uint32_t w[2];
        std::memcpy(w, g.rdram + id, 8);
        uint32_t attrOn = 0u, attrOff = 0u;
        const bool okOn = attributeAddress(g, id, attrOn);
        const bool fOn = directFilter(g, filter, id, user, site, sp);
        sotc_decomp::setSwitch(sotc_decomp::kFin125, false);
        const bool okOff = attributeAddress(g, id, attrOff);
        const bool fOff = directFilter(g, filter, id, user, site, sp);
        sotc_decomp::setSwitch(sotc_decomp::kFin125, true);
        const Surf *x = findSurf(w[0], w[1]);
        std::fprintf(stderr, "[fin135f] filter %08x user %08x surface %08x %08x: on %d (attr %d %08x) off %d (attr %d %08x) kept attr %d %08x\n", filter, user, w[0], w[1], fOn ? 1 : 0,
                     okOn ? 1 : 0, attrOn, fOff ? 1 : 0, okOff ? 1 : 0, attrOff, x ? x->m.hasAttr : -1, x ? x->m.attr : 0u);
    }
    inline void explainTrace(uint32_t id)
    {
        for (int k = 0; k < g_traceN; ++k)
            if (g_trace[k].cand == id)
            {
                const Trace &t = g_trace[k];
                std::fprintf(stderr, "[fin135t] entry %d (%08x %08x): r %d fresh %d current %d state %d filtered %d made %d plane then %.6f %.6f %.6f %.6f\n", k, t.w0, t.w1, t.r,
                             t.fresh, t.wasCurrent, t.state, t.filtered, t.made, t.n[0], t.n[1], t.n[2], t.n[3]);
            }
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
        const bool tr = checkLevel() != 0;
        if (tr)
            g_traceN = 0;
        const bool pf138 = on138();
        for (int32_t k = 0; k < n; ++k, cand += 8u)
        {
            uint32_t w[2];
            std::memcpy(w, g.rdram + cand, 8);
            if (pf138 && k + 1 < n)
            {
                // Round 138: the next entry's validation asked for now when it will need one (kept, not current)
                uint32_t nw[2];
                std::memcpy(nw, g.rdram + cand + 8u, 8);
                if (nw[0] != 0u)
                    if (const Surf *nx = findSurf(nw[0], nw[1]); nx && nx->m.state == kHost && !nx->current(frame))
                        prefetch138(g, nx->m);
            }
            if (w[0] == 0u)
                continue;
            bool fresh;
            Surf &x = claimSurf(w[0], w[1], fresh);
            Meta &m = x.m;
            Entry &e = x.e;
            Trace *t = tr && g_traceN < 257 ? &g_trace[g_traceN++] : nullptr;
            if (t)
            {
                *t = Trace{};
                t->cand = cand;
                t->w0 = w[0];
                t->w1 = w[1];
                t->fresh = fresh;
                t->wasCurrent = !fresh && m.state == kHost && x.current(frame);
                t->r = 9;
            }
            if (__builtin_expect(checkLevel() != 0, 0) && !fresh && m.state == kHost)
                checkUse(g, x, frame);
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
            if (t)
            {
                t->r = static_cast<int8_t>(r);
                t->state = m.state;
                t->made = fresh;
                std::memcpy(t->n, &e.n, 16);
            }
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
                if (t)
                    t->filtered = 1;
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

}
