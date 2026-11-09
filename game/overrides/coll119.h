// Round 119 (SOTC_VIEW119): a finder list's surfaces as a host contact set
// (included by decomp_collision.cpp inside its namespace, after the plane
// helpers and before clipKeep's round 119 test).
//
// A collision finder (the ground fill, the finder steps) leaves a list of
// surface ids (8 bytes each: the object, then the part / surface word); the
// character's probes then go through that list several times a frame
// (ClipCollisionFinder / ...CallBack: the landing test, the IK's foot search;
// ClipCollisionFinderWithKeep: the slide's keep test), and every entry asks
// ClipCollisionDB / clipKeep, whose first step is the surface's plane: the
// object's layout class, its collision work, the part's modified-list record
// and its plane table (four to six dependent loads into guest memory), then
// the plane's cached flag and the plane itself. Most entries end there (the
// segment on one side of the plane; the point far enough in front).
//
// Here the list gets a host view for the game frame: per entry, at its first
// test, the address of its plane and of its plane's cached flag, as
// ClipCollisionDB / clipKeep compute them, with the entry's two id words. A
// later pass over the same list (the same address and count, the same frame,
// the entry's id words the same) reads the flag and the plane straight away and makes the same first test (round 49's NEON form, bit for bit; not
// small or the flag clear: the original call, which computes the plane as
// before). An entry that test throws out leaves what ClipCollisionDB / clipKeep
// would (f0 = 0); every other entry makes the call as before.
//
// The addresses come from per-object collision work that lives as long as the
// object (kept for one game frame only, and only while the list's words hash
// the same). Game thread only (the worker's lane makes every call).

namespace coll119
{
    // The plane and flag addresses of the surface whose 8-byte id is at `id` (ClipCollisionDB's and clipKeep's
    // chain), false when the layout class is not a plain one or an address is outside main RAM.
    inline bool planeAddrs(const G g, uint32_t id, uint32_t &normal, uint32_t &flag)
    {
        const uint32_t w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, g.u32(id), cls) || cls == 0u || !G::plain(cls, 0x200u))
            return false;
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        if (static_cast<int32_t>(w1) < 0)
        {
            const uint32_t work = g.u32(cls + 0x174u);
            if (!G::plain(work, 0x28u))
                return false;
            const uint32_t s2 = g.u32(work + 0x24u) + s1 * 0x60u + 0x30u;
            if (!G::plain(s2, 0x18u))
                return false;
            normal = g.u32(s2 + 0x14u) + s3 * 16u;
            flag = g.u32(s2 + 8u) + s3;
        }
        else
        {
            const uint32_t a = g.u32(cls + 0x184u);
            if (!G::plain(a, 0x24u))
                return false;
            const uint32_t t2 = g.u32(a + 0x20u);
            if (!G::plain(t2, 0x10u))
                return false;
            const uint32_t rec = s1 * 0x90u + g.u32(t2 + 8u) + 0x54u;
            if (!G::plain(rec, 4u))
                return false;
            const uint32_t at = g.u32(rec) + s3 * 4u;
            const uint32_t work = g.u32(cls + 0x174u);
            if (!G::plain(at, 4u) || !G::plain(work, 0x70u))
                return false;
            const uint32_t index = g.u32(at);
            const uint32_t s2 = work + 0x58u;
            normal = g.u32(s2 + 0x14u) + index * 16u;
            flag = g.u32(s2 + 8u) + index;
        }
        return G::plain(flag, 1u) && G::plain(normal & ~0xFu, 16u);
    }

    struct View
    {
        static constexpr int kMax = 256;
        uint32_t list = 0;
        int32_t n = -1;
        uint64_t frame = ~0ull, used = 0;
        // per entry, made at its first test this frame and kept while its two id words stay the same:
        // the id words, the plane's and its flag's addresses (normal 0: not known, the call is made)
        uint32_t w0[kMax], w1[kMax], normal[kMax], flag[kMax];
        uint8_t made[kMax];
    };
    inline View g_views[4];
    inline uint64_t g_viewUse = 0;
    struct Stats
    {
        uint64_t builds = 0, uses = 0, tested = 0, rejected = 0, checked = 0, differ = 0, made = 0;
    };
    inline Stats g_vst;

    inline bool on(const PS2Runtime *runtime) { return sotc_decomp::switchOn(sotc_decomp::kView119) && !sotc_decomp::onWorkerLane(runtime); }
    inline int checkLevel()
    {
        static const int v = ps2_diag::env("SOTC_VIEW119_CHECK") ? std::atoi(ps2_diag::env("SOTC_VIEW119_CHECK")) : 0;
        return v;
    }

    // The view of the n entries at list for this frame (its entries made as they are tested).
    inline View *viewOf(const G g, uint32_t list, int32_t n)
    {
        if (n <= 0 || n > View::kMax || !G::plain(list, 8u * static_cast<uint32_t>(n)))
            return nullptr;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        View *victim = &g_views[0];
        for (View &v : g_views)
        {
            if (v.list == list && v.n == n && v.frame == frame)
            {
                v.used = ++g_viewUse;
                ++g_vst.uses;
                return &v;
            }
            if (v.used < victim->used)
                victim = &v;
        }
        View &v = *victim;
        v.list = list;
        v.n = n;
        v.frame = frame;
        v.used = ++g_viewUse;
        std::memset(v.made, 0, static_cast<size_t>(n));
        ++g_vst.builds;
        return &v;
    }
    // Entry k's plane address (0: not known), made or checked against the list's words.
    inline uint32_t entry(const G g, View &v, int32_t k)
    {
        const uint32_t id = v.list + 8u * static_cast<uint32_t>(k);
        uint32_t w[2];
        std::memcpy(w, g.rdram + id, 8);
        if (v.made[k] && v.w0[k] == w[0] && v.w1[k] == w[1])
            return v.normal[k];
        uint32_t nm = 0u, fl = 0u;
        if (w[0] == 0u || !planeAddrs(g, id, nm, fl))
            nm = 0u;
        v.w0[k] = w[0];
        v.w1[k] = w[1];
        v.normal[k] = nm;
        v.flag[k] = fl;
        v.made[k] = 1u;
        ++g_vst.made;
        return nm;
    }

    // ClipCollisionDB's plane test for entry k (the segment a1 -> a2): 1 it returns 0 there (f0 = 0 its
    // only write), 0 it goes on, -1 not known this way (the call decides).
    inline int clipRejects(const G g, View &v, int32_t k, uint32_t a1, uint32_t a2)
    {
#if PS2X_VU_NEON_A32
        const uint32_t nm = entry(g, v, k);
        if (nm == 0u || g.rdram[v.flag[k]] == 0u)
            return -1;
        bool small = false;
        const uint64_t d = fast::planeDistances(g.q(nm), g.ctx->vu0_vf[0], g.q(a1), g.q(a2), small);
        if (!small)
            return -1;
        return fast::belowZero(static_cast<uint32_t>(d)) || fast::aboveZero(static_cast<uint32_t>(d >> 32)) ? 1 : 0;
#else
        (void)g, (void)v, (void)k, (void)a1, (void)a2;
        return -1;
#endif
    }

    // SOTC_COLL115_LOG=1: host time by state (0 the switch off, 1 on): coll115's tree walks by walk119,
    // ClipCollisionFinder / ...CallBack by view119, ClipCollisionDirectCallBack (coll115 on) by part119,
    // FlushCollisionWorkByWorkStruct by flush119 (ClipCollisionFinderWithKeep by view119: decomp_hot44b's line).
    struct Times
    {
        uint64_t usW[2] = {}, nW[2] = {}, usC[2] = {}, nC[2] = {}, usK[2] = {}, nK[2] = {}, usQ[2] = {}, nQ[2] = {}, usF[2] = {}, nF[2] = {};
    };
    inline Times g_t;
    inline void tick119(uint64_t frame, double n)
    {
        const char *names[5] = {"walks (walk119)", "finder clips (view119)", "keep loops (view119)", "segment queries (part119)", "work flushes (flush119)"};
        const uint64_t *us[5] = {g_t.usW, g_t.usC, g_t.usK, g_t.usQ, g_t.usF}, *nn[5] = {g_t.nW, g_t.nC, g_t.nK, g_t.nQ, g_t.nF};
        for (int k = 0; k < 5; ++k)
            for (int i = 0; i < 2; ++i)
                if (nn[k][i] != 0u)
                    std::fprintf(stderr, "[coll119t] frame %llu %s %s: %llu calls, %.2f us each (%.3f ms a frame at its rate)\n", (unsigned long long)frame, names[k],
                                 i ? "on" : "off", (unsigned long long)nn[k][i], (double)us[k][i] / nn[k][i],
                                 (double)us[k][i] / 1000.0 / n * (double)(nn[k][0] + nn[k][1]) / nn[k][i]);
        g_t = Times{};
    }
    inline uint64_t g_logFrame = 0;
    inline void log()
    {
        static const bool want = checkLevel() != 0 || (ps2_diag::env("SOTC_COLL115_LOG") && std::atoi(ps2_diag::env("SOTC_COLL115_LOG")) != 0);
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (!want || frame < g_logFrame + 600u)
            return;
        if (g_logFrame != 0u)
            std::fprintf(stderr, "[view119c] frame %llu: finder clips: views built %llu, used %llu, entries made %llu, tested %llu, thrown out %llu | check %llu differ %llu\n",
                         (unsigned long long)frame, (unsigned long long)g_vst.builds, (unsigned long long)g_vst.uses, (unsigned long long)g_vst.made, (unsigned long long)g_vst.tested,
                         (unsigned long long)g_vst.rejected, (unsigned long long)g_vst.checked, (unsigned long long)g_vst.differ);
        g_vst = Stats{};
        g_logFrame = frame;
    }
}
