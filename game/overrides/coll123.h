// Round 123 (SOTC_SEG123): a segment test's answer kept while nothing it
// depends on changed (included by decomp_collision.cpp inside its namespace,
// after seg80 and before segMemo).
//
// The console's A/B of round 119 (r119/console/err.txt) showed where coll115
// lost: its walks are 2.4x faster on the A9 (17 vs 41 us), but with coll115
// on, seg80's reuse is off, so every query walks (3.7 walks a query against
// 1.4 with the memo answering the repeats): 150 vs 110 us a query. seg80's
// reuse was dropped because it was blind: a 1 cm / 5 frame window in which a
// character or a colossus moving into the segment went unseen.
//
// seg123 keeps the reuse and removes the blindness: a query (the same caller,
// group, filters, user and its six judge words, the ends within 1 mm of the
// kept ones) reuses the kept answer only when the world it was tested
// against is the same: the group's objects that pass the query's object
// filter (in order, the hidden ones skipped as isysGroupForNext does) and
// for each of them a fingerprint of everything ClipCollisionDirectCallBack
// reads of it before its trees: the layout class, its collision DB, flags,
// scale, the root position and the three per-part matrix arrays (the parts'
// world matrices, which move with every animated bone). A fingerprint is
// computed once an object a game frame (a host table), so the validation
// costs a group walk and a few table lookups. Anything not computable that
// way (an object filter that is guest code, a class GetLayoutClass computes
// by a call, a group over 48 objects, the worker lane) runs the test.
// Not exact in one way only: the kept answer is for ends up to 1 mm away
// (SOTC_SEG123_EPS; seg80 used 1 cm): the hit point can be up to ~1 mm off
// and, at a triangle's edge, the neighbouring surface's id be the one kept.
//
// SOTC_SEG123_CHECK=1: the test runs on every kept answer too and is the
// answer used; hit / no hit and the hit point's distance are counted against
// the kept one ([seg123c]). SOTC_COLL115_LOG=1 prints [seg123] every 600
// frames. SOTC_SEG123_EPS (0.001: 1 mm), SOTC_SEG123_AGE (frames, 60).
namespace coll123
{
    struct Params
    {
        float eps = 0.01f;
        uint32_t age = 60u;
        int check = 0;
        Params()
        {
            if (const char *e = std::getenv("SOTC_SEG123_EPS"))
                eps = static_cast<float>(std::atof(e));
            if (const char *e = std::getenv("SOTC_SEG123_AGE"))
                age = static_cast<uint32_t>(std::atoi(e));
            if (const char *e = std::getenv("SOTC_SEG123_CHECK"))
                check = std::atoi(e);
        }
    };
    inline const Params &params()
    {
        static const Params p;
        return p;
    }

    constexpr int kMaxObj = 48;
    struct World
    {
        uint32_t n = 0;
        uint32_t obj[kMaxObj];
        uint32_t fp[kMaxObj];
        bool operator==(const World &o) const
        {
            return n == o.n && std::memcmp(obj, o.obj, n * 4u) == 0 && std::memcmp(fp, o.fp, n * 4u) == 0;
        }
    };
    struct Slot
    {
        seg80::Key k{};
        bool live = false;
        uint64_t frame = 0, v0 = 0, used = 0;
        float f0 = 0.0f;
        uint8_t rec[seg80::kRecord] = {};
        World w;
    };
    constexpr int kSlots = 64;
    inline Slot g_slots[kSlots];
    inline uint64_t g_use = 0;
    struct Stat
    {
        uint64_t hit = 0, nokey = 0, moved = 0, novalid = 0, checked = 0, differ = 0, other = 0;
        double maxd = 0.0;
        uint64_t fpMade = 0, fpWords = 0;
        uint64_t movedType[24] = {}; // world moved: the first differing object's type byte (+8), 23 = other / count changed
        uint64_t movedCaller[8] = {}; uint32_t callerRa[8] = {};
    };
    inline Stat g_st;
    inline uint64_t g_bypassed = 0; // queries sent straight to the test (callerOf: a caller keeping nothing)
    struct Times
    {
        uint64_t us[2] = {}, n[2] = {};
    };
    inline Times g_t; // ClipCollisionDirectCallBack's host time by seg123's state (coll115 on), SOTC_COLL115_LOG=1
    inline uint64_t g_logFrame = 0;

    // The per-frame fingerprints (object -> hash), direct-mapped.
    struct FpEntry
    {
        uint32_t obj = 0;
        uint32_t fp = 0;
        uint64_t frame = ~0ull;
        float c[3] = {}, r = 0.0f; // the object's bound (world centre, radius x scale)
        bool bound = false;        // false: no bound (radius 0, no DB): always near
        bool full = true;          // the parts' matrices hashed in too (made only for an object near a segment)
        uint32_t cls = 0, parts = 0;
    };
    constexpr int kFp = 256;
    inline FpEntry g_fp[kFp];

    inline uint32_t mix(uint32_t h, uint32_t w) { return (h ^ w) * 0x01000193u; }
    inline bool hashBlock(const G g, uint32_t a, uint32_t bytes, uint32_t &h)
    {
        if (a == 0u)
        {
            h = mix(h, 0u);
            return true;
        }
        if (!G::plain(a, bytes) || (a & 3u) != 0u)
            return false;
        const uint8_t *p = g.rdram + a;
        for (uint32_t i = 0; i < bytes; i += 4u)
        {
            uint32_t w;
            std::memcpy(&w, p + i, 4);
            h = mix(h, w);
        }
        g_st.fpWords += bytes / 4u;
        return true;
    }
    // What the object loop reads of an object before its trees, hashed (false: not computable natively).
    inline bool fingerprint(const G g, uint32_t object, const FpEntry *&out)
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        FpEntry &e = g_fp[(object >> 4) & (kFp - 1)];
        if (e.obj == object && e.frame == frame)
        {
            out = &e;
            return true;
        }
        bool bound = false;
        float c[3] = {}, rad = 0.0f;
        uint32_t cls;
        e.obj = 0u;
        e.full = true;
        if (!layoutClass(g, object, cls))
            return false;
        uint32_t h = mix(0x811C9DC5u, object);
        h = mix(h, cls);
        if (cls != 0u)
        {
            if (!G::plain(cls, 0x1C0u))
                return false;
            h = mix(h, g.u32(cls + 0x1B8u)); // the two flag bytes (+0x1b8, +0x1bb)
            h = mix(h, g.u32(cls + 0x90u));  // scale
            const uint32_t dbr = g.u32(cls + 0x184u);
            if (!G::plain(dbr, 0x24u))
                return false;
            const uint32_t db = g.u32(dbr + 0x20u);
            h = mix(h, db);
            if (db != 0u)
            {
                if (!G::plain(db, 0x30u))
                    return false;
                const uint32_t parts = g.u32(db + 4u);
                if (parts > 256u)
                    return false;
                h = mix(h, parts);
                h = mix(h, g.u32(db + 0x2Cu));
                const uint32_t ca = g.u32(cls + 0x134u);
                if (!hashBlock(g, ca, 16u, h))
                    return false;
                const float radius = g.f32(db + 0x2Cu), scale = g.f32(cls + 0x90u);
                if (radius != 0.0f && ca != 0u)
                {
                    bound = true;
                    rad = std::fabs(radius * scale);
                    for (int i = 0; i < 3; ++i)
                        c[i] = g.f32(ca + 4u * i);
                    if (!(rad < 1e6f) || !(std::fabs(c[0]) + std::fabs(c[1]) + std::fabs(c[2]) < 1e7f) || g.f32(ca + 12u) != 1.0f)
                        bound = false;
                }
                h = mix(h, g.u32(cls + 0x174u));
                e.full = false;
                e.parts = parts;
            }
        }
        ++g_st.fpMade;
        if (!(cls != 0u && e.full == false))
            e.full = true;
        e.cls = cls;
        e.obj = object;
        e.fp = h;
        e.frame = frame;
        e.bound = bound;
        e.r = rad;
        for (int i = 0; i < 3; ++i)
            e.c[i] = c[i];
        out = &e;
        return true;
    }
    // The group's objects that pass the object filter, with their fingerprints (false: not computable natively).
    // Far from the segment: the object's bound (a sphere around everything the loop tests of it) is further from the
    // segment than its radius x sqrt 2 (the loop's cylinder with its end caps lies inside that) plus 5 cm (the ends'
    // 1 mm tolerance and rounding). Such an object cannot be hit by this segment however it is posed.
    inline bool far(const FpEntry &e, const float *a, const float *b)
    {
        if (!e.bound)
            return false;
        const float d[3] = {b[0] - a[0], b[1] - a[1], b[2] - a[2]}, v[3] = {e.c[0] - a[0], e.c[1] - a[1], e.c[2] - a[2]};
        const float dd = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
        float t = dd > 0.0f ? (v[0] * d[0] + v[1] * d[1] + v[2] * d[2]) / dd : 0.0f;
        t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const float x = v[0] - t * d[0], y = v[1] - t * d[1], z = v[2] - t * d[2];
        const float lim = e.r * 1.4143f + 0.05f;
        return x * x + y * y + z * z > lim * lim;
    }
    constexpr uint32_t kFar = 0x9E3779B9u;
    // The three per-part matrix arrays hashed into the entry's fingerprint (once a frame; false: not readable).
    inline bool full(const G g, FpEntry &e)
    {
        if (e.full)
            return true;
        uint32_t h = e.fp;
        for (uint32_t off : {0x158u, 0x15Cu, 0x164u})
            if (!hashBlock(g, g.u32(e.cls + off), e.parts * 0x40u, h))
                return false;
        e.fp = h;
        e.full = true;
        return true;
    }
    inline bool world(const G g, uint32_t group, uint32_t filter, uint32_t user, const float *a, const float *b, World &w)
    {
        constexpr uint32_t kLinks = 0x0021B900u;
        w.n = 0;
        if (group == 0u || !G::plain(group, 4u))
            return false;
        uint32_t steps = 0;
        for (uint32_t idx = g.u16(group); idx != 0u;)
        {
            const uint32_t link = kLinks + idx * 0x18u;
            if (!G::plain(link, 0x18u) || ++steps > 0x1000u)
                return false;
            idx = g.u16(link + 0xEu);
            if (g.u8(link + 0xCu) != 0u)
                continue;
            const uint32_t object = g.u32(link);
            if (object == 0u || !G::plain(object, 0x10u))
                return false;
            if ((g.u16(object + 0xAu) & 1u) != 0u)
                continue;
            if (filter != 0u)
            {
                uint64_t pass;
                if (!objectFilter(g, filter, object, user, pass))
                    return false;
                if (pass == 0u)
                    continue;
            }
            if (w.n >= static_cast<uint32_t>(kMaxObj))
                return false;
            const FpEntry *e = nullptr;
            if (!fingerprint(g, object, e))
                return false;
            w.obj[w.n] = object;
            if (far(*e, a, b))
                w.fp[w.n] = kFar;
            else
            {
                if (!full(g, *const_cast<FpEntry *>(e)))
                    return false;
                w.fp[w.n] = e->fp;
            }
            ++w.n;
        }
        return true;
    }
    inline bool sameKey(const Slot &s, const seg80::Key &k)
    {
        if (!s.live || s.k.fn != k.fn || s.k.ra != k.ra || s.k.group != k.group || s.k.user != k.user || std::memcmp(s.k.filt, k.filt, sizeof k.filt) != 0 ||
            std::memcmp(s.k.judge, k.judge, sizeof k.judge) != 0)
            return false;
        const float e = params().eps;
        for (int i = 0; i < 3; ++i)
            if (!(std::fabs(s.k.a[i] - k.a[i]) <= e) || !(std::fabs(s.k.b[i] - k.b[i]) <= e))
                return false;
        return true;
    }
    inline void log()
    {
        if (!coll115::params().log || sotc_decomp::g_gameFrame < g_logFrame + 600u)
            return;
        g_logFrame = sotc_decomp::g_gameFrame;
        const Stat &s = g_st;
        const uint64_t all = s.hit + s.nokey + s.moved + s.novalid;
        std::fprintf(stderr, "[seg123] frame %llu: queries %llu kept %llu (%.1f%%) | tested: no key %llu, world moved %llu, not validatable %llu | fingerprints %llu (%llu words)",
                     (unsigned long long)g_logFrame, (unsigned long long)all, (unsigned long long)s.hit, all ? 100.0 * s.hit / all : 0.0, (unsigned long long)s.nokey,
                     (unsigned long long)s.moved, (unsigned long long)s.novalid, (unsigned long long)s.fpMade, (unsigned long long)s.fpWords);
        if (s.moved)
        {
            std::fprintf(stderr, " | moved by type:");
            for (int t = 0; t < 24; ++t)
                if (s.movedType[t])
                    std::fprintf(stderr, " %d:%llu", t, (unsigned long long)s.movedType[t]);
            std::fprintf(stderr, " by caller:");
            for (int c = 0; c < 8; ++c)
                if (s.callerRa[c])
                    std::fprintf(stderr, " %08x:%llu", s.callerRa[c], (unsigned long long)s.movedCaller[c]);
        }
        if (s.checked)
            std::fprintf(stderr, " | check %llu: hit or not differ %llu, other surface %llu, point max %.4f m", (unsigned long long)s.checked, (unsigned long long)s.differ,
                         (unsigned long long)s.other, s.maxd);
        std::fprintf(stderr, " | bypassed %llu\n", (unsigned long long)g_bypassed);
        g_bypassed = 0;
        g_st = Stat{};
        std::fprintf(stderr, "[seg123t] frame %llu: queries with seg123 %llu, %.1f us each | without %llu, %.1f us each\n", (unsigned long long)g_logFrame,
                     (unsigned long long)g_t.n[1], g_t.n[1] ? (double)g_t.us[1] / g_t.n[1] : 0.0, (unsigned long long)g_t.n[0], g_t.n[0] ? (double)g_t.us[0] / g_t.n[0] : 0.0);
        g_t = Times{};
    }

    // Per caller (its ra): a caller that kept under a quarter of its last 32 queries (riding: the segments move more
    // than the tolerance nearly every frame) goes straight to the test for its next 96 queries (then tries again):
    // the group walk and fingerprints (~10-20 us a query in Vita3K) cost more than the few answers kept save.
    struct Caller
    {
        uint32_t ra = 0;
        uint16_t tried = 0, kept = 0, skip = 0;
    };
    constexpr int kCallers = 32;
    inline Caller g_callers[kCallers];

    inline Caller &callerOf(uint32_t ra)
    {
        Caller &c = g_callers[(ra >> 2) & (kCallers - 1)];
        if (c.ra != ra)
            c = Caller{ra, 0, 0, 0};
        return c;
    }
    inline void noteCaller(Caller &c, bool kept)
    {
        ++c.tried;
        c.kept += kept ? 1 : 0;
        if (c.tried >= 32u)
        {
            if (c.kept * 4u < c.tried) // under a quarter kept: the walk and fingerprints cost more than they save
                c.skip = 96u;
            c.tried = c.kept = 0;
        }
    }

    template <typename Body>
    void memo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t fn, Body body, int &depth)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t out = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), spIn = GPR_U32(ctx, 29);
        if (!G::plain(out, seg80::kRecord) || !G::plain(start, 16u) || !G::plain(end, 16u) || !G::plain(spIn, 4u))
            return body();
        log();
        seg80::Key k{};
        k.ra = GPR_U32(ctx, 31);
        k.fn = fn;
        k.group = GPR_U32(ctx, 7);
        for (int i = 0; i < 4; ++i)
            k.filt[i] = GPR_U32(ctx, 8 + i);
        k.user = g.u32(spIn);
        if (k.user != 0u && G::plain(k.user, 24u))
            for (int i = 0; i < 6; ++i)
                k.judge[i] = g.u32(k.user + 4u * i);
        for (int i = 0; i < 3; ++i)
        {
            std::memcpy(&k.a[i], rdram + start + 4u * i, 4);
            std::memcpy(&k.b[i], rdram + end + 4u * i, 4);
        }
        const uint64_t frame = sotc_decomp::g_gameFrame;
        Caller &caller = callerOf(k.ra);
        if (caller.skip != 0u && params().check == 0)
        {
            --caller.skip;
            ++g_bypassed;
            return body();
        }
        static World w; // (not on the stack: GCC vectorises its compares with :128 loads, and the Vita's sp is only 8-aligned; game thread only, never nested: depth)
        const bool valid = world(g, k.group, k.filt[0], k.user, k.a, k.b, w);
        Slot *victim = &g_slots[0], *same = nullptr;
        for (Slot &s : g_slots)
        {
            if (sameKey(s, k) && s.frame + params().age >= frame)
            {
                same = &s;
                break;
            }
            if (!s.live || s.used < victim->used)
                victim = &s;
        }
        if (same && valid && same->w == w)
        {
            if (__builtin_expect(params().check != 0, 0))
            {
                ++depth;
                body();
                --depth;
                ++g_st.checked;
                const uint64_t v0 = GPR_U64(ctx, 2);
                if ((v0 != 0u) != (same->v0 != 0u))
                    ++g_st.differ;
                else if (v0 != 0u)
                {
                    if (std::memcmp(rdram + out, same->rec, 8) != 0)
                        ++g_st.other;
                    float p[3], q[3];
                    std::memcpy(p, rdram + out + 0x40u, 12);
                    std::memcpy(q, same->rec + 0x40, 12);
                    const double d = std::sqrt((double)(p[0] - q[0]) * (p[0] - q[0]) + (double)(p[1] - q[1]) * (p[1] - q[1]) + (double)(p[2] - q[2]) * (p[2] - q[2]));
                    if (d > g_st.maxd)
                        g_st.maxd = d;
                }
                ++g_st.hit;
                return;
            }
            std::memcpy(rdram + out, same->rec, seg80::kRecord);
            SET_GPR_U64(ctx, 2, same->v0);
            ctx->f[0] = same->f0;
            same->used = ++g_use;
            ++g_st.hit;
            noteCaller(caller, true);
            return returnTo(ctx);
        }
        if (!valid)
            ++g_st.novalid;
        else if (same)
        {
            ++g_st.moved;
            uint32_t i = 0;
            while (i < w.n && i < same->w.n && w.obj[i] == same->w.obj[i] && w.fp[i] == same->w.fp[i])
                ++i;
            const uint32_t t = (i < w.n && i < same->w.n && w.obj[i] == same->w.obj[i]) ? g.u8(w.obj[i] + 8u) : 23u;
            ++g_st.movedType[t < 23u ? t : 23u];
            for (int c = 0; c < 8; ++c)
                if (g_st.callerRa[c] == k.ra || g_st.callerRa[c] == 0u)
                {
                    g_st.callerRa[c] = k.ra;
                    ++g_st.movedCaller[c];
                    break;
                }
        }
        else
            ++g_st.nokey;
        noteCaller(caller, false);
        ++depth;
        body();
        --depth;
        if (!valid)
        {
            if (same)
                same->live = false;
            return;
        }
        Slot &s = same ? *same : *victim;
        s.live = true;
        s.k = k;
        s.frame = frame;
        s.v0 = GPR_U64(ctx, 2);
        s.f0 = ctx->f[0];
        std::memcpy(s.rec, rdram + out, seg80::kRecord);
        s.used = ++g_use;
        s.w = w;
    }
}
