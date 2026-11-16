// Round S2 (SOTC_S2COLL), part 2: the water probes (included by decomp_collision.cpp inside its namespace, before the
// round-40 water natives waterProbe / waterSurface; the write-up in docs/collision_s2.md).
//
// The PS2's way: a water mesh (the collision DB's +0x38 count / +0x3C table of 0x20-byte triangles: +4 the height, +8..
// +0x1C three xz vertices) is a flat list, and both probes run every triangle through the full barycentric test (three
// VU0 outer products, four dots, two PS2 divisions) for every call. Avion's lake has a big mesh, and Wander's swim asks
// the probes many times a frame: D9's / S2's sampler put the two natives at 4.1% of the game thread on the lake route,
// the largest collision item there.
//
// Both natives' outcome is decided by few triangles, and the rest only leave values the next triangle overwrites:
// - manager_sub_01117EA0 (the water surface under a segment's end): a triangle sends the call to the recompiled code
//   only when the end's xz is in it (u, v >= -2e-5, u + v <= 1 + 2e-5); otherwise it only sets f0, and f0 after the loop
//   is the last triangle's. So the loop's own body runs for the triangles whose xz box (with a margin: 1e-3 of the box
//   + 1e-3 + 2e-6 of the coordinates) holds the end, for every triangle whose shape the box test cannot vouch for (not
//   finite, over 1e6, or thin: twice its area under 1e-3 of |e1|1 |e2|1), and for the last one.
// - manager_sub_01117340 (the water surface a segment crosses), with no triangle filter: a triangle without p1y < h <
//   p0y (its height between the ends, the code's own compares) only writes its words into the work block and sets f0,
//   both overwritten by the next triangle; so the body runs for the triangles with the height between the ends and for
//   the last one, which leaves the work block and f0 as the loop does. A triangle with the height between the ends
//   writes the crossing block and tests the crossing point, which lies between the segment's ends: an earlier one whose
//   xz box (margin as above) misses the ends' xz box cannot hold it and only writes words the last such triangle
//   overwrites, so it is passed over too; the last such one always runs. (With a filter, a guest call per triangle: as
//   before.)
// The bodies are the natives' own (the same operations on the same values, the same writes), so the answers, f0 and
// the blocks are the original's. SOTC_S2COLL_CHECK=1 runs both ways from the same state on every call and compares the
// outcome, f0's bits and the work / crossing blocks ([s2water]).
namespace s2water
{
    // (plain compares: std::fmax / fmin are library calls on the A9's VFPv3; every operand is finite where they are used)
    inline float s2mx(float a, float b) { return a > b ? a : b; }
    inline float s2mn(float a, float b) { return a < b ? a : b; }
    // SOTC_S2COLL_WATER=0 leaves the water pre-tests out while SOTC_S2COLL stays on; read once.
    inline bool waterOn()
    {
        static const bool v = !(ps2_diag::env("SOTC_S2COLL_WATER") && ps2_diag::env("SOTC_S2COLL_WATER")[0] == '0');
        return v;
    }
    inline bool on(const PS2Runtime *runtime) { return sotc_decomp::switchOn(sotc_decomp::kS2Coll) && waterOn() && !sotc_decomp::onWorkerLane(runtime); }
    inline bool checkOn()
    {
        static const bool v = ps2_diag::env("SOTC_S2COLL_CHECK") && std::atoi(ps2_diag::env("SOTC_S2COLL_CHECK")) != 0;
        return v;
    }
    inline bool logOn()
    {
        static const bool v = checkOn() || (ps2_diag::env("SOTC_S2COLL_LOG") && std::atoi(ps2_diag::env("SOTC_S2COLL_LOG")) != 0);
        return v;
    }
    struct Stats
    {
        uint64_t calls[2] = {}, tris[2] = {}, body[2] = {};
        uint64_t checks[2] = {}, differ[2] = {};
        uint64_t logFrame = 0;
    };
    inline Stats g_st;
    inline void log()
    {
        if (!logOn())
            return;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_st.logFrame + 600u)
            return;
        if (g_st.logFrame != 0u)
        {
            const double nf = static_cast<double>(frame - g_st.logFrame);
            std::fprintf(stderr, "[s2water] frame %llu: under-end probes %.1f a frame (%.1f triangles, %.1f through the body); crossing probes %.1f (%.1f triangles, %.1f through the body)",
                         (unsigned long long)frame, g_st.calls[0] / nf, g_st.tris[0] / nf, g_st.body[0] / nf, g_st.calls[1] / nf, g_st.tris[1] / nf, g_st.body[1] / nf);
            if (checkOn())
                std::fprintf(stderr, " | check: under-end %llu differ %llu, crossing %llu differ %llu", (unsigned long long)g_st.checks[0], (unsigned long long)g_st.differ[0],
                             (unsigned long long)g_st.checks[1], (unsigned long long)g_st.differ[1]);
            std::fprintf(stderr, "\n");
        }
        for (int i = 0; i < 2; ++i)
            g_st.calls[i] = g_st.tris[i] = g_st.body[i] = 0u;
        g_st.logFrame = frame;
    }
    inline void compared(int kind, bool same)
    {
        ++g_st.checks[kind];
        if (!same)
        {
            ++g_st.differ[kind];
            if (g_st.differ[kind] <= 20u)
                std::fprintf(stderr, "[s2water] frame %llu %s probe DIFFER\n", (unsigned long long)sotc_decomp::g_gameFrame, kind ? "crossing" : "under-end");
        }
    }
    // SOTC_S2COLL_LOG=1: each probe call's host time by the switch's state (kind 0 under-end, 1 crossing), cumulative
    // from the run's start; a Welch z of the per-call means (positive: S2 faster); us a frame over each state's frames.
    struct Timing
    {
        uint64_t n[2][2] = {}, frames[2] = {}, lastFrame[2] = {};
        double sum[2][2] = {}, sq[2][2] = {};
        uint64_t logFrame = 0;
    };
    inline Timing g_t;
    inline void timed(int kind, int st, uint64_t us)
    {
        Timing &t = g_t;
        const double d = static_cast<double>(us);
        ++t.n[kind][st];
        t.sum[kind][st] += d;
        t.sq[kind][st] += d * d;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (t.lastFrame[st] != frame)
        {
            t.lastFrame[st] = frame;
            ++t.frames[st];
        }
        if (frame < t.logFrame + 600u)
            return;
        t.logFrame = frame;
        for (int k = 0; k < 2; ++k)
        {
            double mean[2] = {}, var[2] = {};
            for (int i = 0; i < 2; ++i)
                if (t.n[k][i] > 1u)
                {
                    mean[i] = t.sum[k][i] / t.n[k][i];
                    var[i] = (t.sq[k][i] - t.sum[k][i] * mean[i]) / (t.n[k][i] - 1u);
                }
            const double se = std::sqrt((t.n[k][0] ? var[0] / t.n[k][0] : 0.0) + (t.n[k][1] ? var[1] / t.n[k][1] : 0.0));
            std::fprintf(stderr, "[s2wtime] frame %llu %s probes: off %llu calls %.3f us %.4f ms a frame | on %llu calls %.3f us %.4f ms a frame | z %.2f\n",
                         (unsigned long long)frame, k ? "crossing" : "under-end", (unsigned long long)t.n[k][0], mean[0], t.frames[0] ? t.sum[k][0] / 1000.0 / t.frames[0] : 0.0,
                         (unsigned long long)t.n[k][1], mean[1], t.frames[1] ? t.sum[k][1] / 1000.0 / t.frames[1] : 0.0, se > 0.0 ? (mean[0] - mean[1]) / se : 0.0);
        }
    }
    inline float rd(const uint8_t *rdram, uint32_t a)
    {
        float f;
        std::memcpy(&f, rdram + a, 4);
        return f;
    }
    // Whether the triangle at t surely leaves (px, pz) out of the barycentric test (see above).
    inline bool outside(const uint8_t *rdram, uint32_t t, float px, float pz)
    {
        float v[6];
        std::memcpy(v, rdram + t + 8u, 24); // x0 z0 x1 z1 x2 z2
        float big = s2mx(std::fabs(px), std::fabs(pz));
        for (float f : v)
        {
            if (!(std::fabs(f) < 1e6f))
                return false;
            big = s2mx(big, std::fabs(f));
        }
        const float ax = v[2] - v[0], az = v[3] - v[1], bx = v[4] - v[0], bz = v[5] - v[1];
        const float area = std::fabs(ax * bz - az * bx);
        if (!(area > 1e-3f * (std::fabs(ax) + std::fabs(az)) * (std::fabs(bx) + std::fabs(bz))))
            return false;
        const float minx = s2mn(v[0], s2mn(v[2], v[4])), maxx = s2mx(v[0], s2mx(v[2], v[4]));
        const float minz = s2mn(v[1], s2mn(v[3], v[5])), maxz = s2mx(v[1], s2mx(v[3], v[5]));
        const float m = 1e-3f * s2mx(maxx - minx, maxz - minz) + 1e-3f + 2e-6f * big;
        return px < minx - m || px > maxx + m || pz < minz - m || pz > maxz + m;
    }
    // Whether the triangle at t surely leaves every point of the xz box [x0, x1] x [z0, z1] out of the barycentric test
    // (the same margins as outside(); the box is the crossing point's reach: it lies between the segment's ends).
    inline bool outsideBox(const uint8_t *rdram, uint32_t t, float x0, float x1, float z0, float z1)
    {
        float v[6];
        std::memcpy(v, rdram + t + 8u, 24); // x0 z0 x1 z1 x2 z2
        float big = s2mx(s2mx(std::fabs(x0), std::fabs(x1)), s2mx(std::fabs(z0), std::fabs(z1)));
        for (float f : v)
        {
            if (!(std::fabs(f) < 1e6f))
                return false;
            big = s2mx(big, std::fabs(f));
        }
        const float ax = v[2] - v[0], az = v[3] - v[1], bx = v[4] - v[0], bz = v[5] - v[1];
        const float area = std::fabs(ax * bz - az * bx);
        if (!(area > 1e-3f * (std::fabs(ax) + std::fabs(az)) * (std::fabs(bx) + std::fabs(bz))))
            return false;
        const float minx = s2mn(v[0], s2mn(v[2], v[4])), maxx = s2mx(v[0], s2mx(v[2], v[4]));
        const float minz = s2mn(v[1], s2mn(v[3], v[5])), maxz = s2mx(v[1], s2mx(v[3], v[5]));
        const float m = 1e-3f * s2mx(maxx - minx, maxz - minz) + 1e-3f + 2e-6f * big;
        return x1 < minx - m || x0 > maxx + m || z1 < minz - m || z0 > maxz + m;
    }

    // manager_sub_01117EA0's loop: tri(t) runs the native's body for the triangle at t (true: to the recompiled code).
    template <typename Tri>
    bool probe(const uint8_t *rdram, uint32_t t0, int32_t count, float px, float pz, Tri tri)
    {
        log();
        ++g_st.calls[0];
        g_st.tris[0] += static_cast<uint64_t>(count);
        const bool boxes = std::fabs(px) < 1e6f && std::fabs(pz) < 1e6f;
        for (int32_t i = 0; i + 1 < count; ++i)
        {
            const uint32_t t = t0 + 0x20u * static_cast<uint32_t>(i);
            if (boxes && outside(rdram, t, px, pz))
                continue;
            ++g_st.body[0];
            if (tri(t))
                return true;
        }
        ++g_st.body[0];
        return tri(t0 + 0x20u * static_cast<uint32_t>(count - 1));
    }
    // manager_sub_01117340's loop (no triangle filter): tri(t) runs the native's body (its writes included).
    // A triangle with its height between the ends writes the crossing block; the last such one's words stay, so that one
    // always runs the body; an earlier one whose xz box (margin as above) misses the box between the ends' xz cannot
    // hold the crossing point and only writes words a later one overwrites: passed over.
    template <typename Tri>
    bool surface(const uint8_t *rdram, uint32_t t0, int32_t count, float p0x, float p0y, float p0z, float p1x, float p1y, float p1z, Tri tri)
    {
        log();
        ++g_st.calls[1];
        g_st.tris[1] += static_cast<uint64_t>(count);
        auto between = [&](int32_t i)
        {
            const float h = rd(rdram, t0 + 0x20u * static_cast<uint32_t>(i) + 4u);
            return FPU_C_OLT_S(p1y, h) && FPU_C_OLT_S(h, p0y);
        };
        int32_t lastBetween = -1;
        for (int32_t i = count - 1; i >= 0; --i)
            if (between(i))
            {
                lastBetween = i;
                break;
            }
        const bool boxes = std::fabs(p0x) < 1e6f && std::fabs(p0z) < 1e6f && std::fabs(p1x) < 1e6f && std::fabs(p1z) < 1e6f;
        const float x0 = s2mn(p0x, p1x), x1 = s2mx(p0x, p1x), z0 = s2mn(p0z, p1z), z1 = s2mx(p0z, p1z);
        for (int32_t i = 0; i + 1 < count && i <= lastBetween; ++i)
        {
            const uint32_t t = t0 + 0x20u * static_cast<uint32_t>(i);
            if (!between(i))
                continue;
            if (i != lastBetween && boxes && outsideBox(rdram, t, x0, x1, z0, z1))
                continue;
            ++g_st.body[1];
            if (tri(t))
                return true;
        }
        ++g_st.body[1];
        return tri(t0 + 0x20u * static_cast<uint32_t>(count - 1));
    }
}
