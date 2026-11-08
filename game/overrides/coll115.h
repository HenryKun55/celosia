// Round 115 (SOTC_COLL115): the segment test's tree walk and leaves on
// host-format collision data (included by decomp_collision.cpp inside its
// namespace, after the leaf, ClipCollisionDB and the id filters).
//
// What the original does per collision tree (ClipCollisionDirectCallBack's
// tree branch, and the registered node / low / high entries the object-array
// test reaches): a front-to-back walk of the tree's split nodes in the tree's
// space with the segment clipped at each split (the PS2's FPU and VU0 ops,
// guest frames per level), and per leaf: the clipped piece through the
// tree-to-world matrix, every surface's world boundary sphere (a per-frame
// guest cache, gathered through the surface id) through the segment's
// matrix and tested against the segment's cylinder, then the surface and id
// filters and ClipCollisionDB (the plane test, the crossing, the boundary
// sphere, the triangle). The first leaf with a hit ends the walk.
//
// Here: the walk on plain floats over the guest's 8-byte nodes (no guest
// frames, no f0 writes), and each leaf's surfaces as a host block built at
// the leaf's first visit from the model's own data (the part's vertex
// records: the three local vertices of each triangle): a bounding sphere
// and the plane of each triangle in the model's local space, four surfaces
// a block (structure of arrays), so a leaf is one linear pass with no
// gathers and no per-surface matrix: the piece goes into local space once a
// leaf (the tree's space is the local space times the class's uniform
// scale, checked per walk from the class's matrices). A surface goes on to
// the filters and the original's own ClipCollisionDB only when its sphere
// meets the piece and the piece crosses its plane the way the side mode
// asks (both with a margin, so the host floats never throw out what the
// original would hit): ClipCollisionDB makes the hit as before (its record,
// the point, u / v), so the answers are the original's but for the piece
// ends (plain floats instead of the PS2's at the splits: sub-millimetre).
// The filters are judge functions (pure); they are asked only for those
// surfaces.
//
// Host data lifetime: a model (keyed by its tree's address) is checked once
// a game frame against the guest (the tree's and the part record's
// pointers, the root node, the first vertex / index words), each leaf on
// every visit (its leaf word); a mismatch drops what was built. Kept under
// SOTC_COLL115_MB (2) MB: past it the models not used this frame go.
// Game thread only (the worker's lane runs the original).
//
// SOTC_COLL115_LOG=1: counts every 600 frames ("[coll115] ...").
// SOTC_COLL115_CHECK=1: each ClipCollisionDirectCallBack run the original
// way and then this way from the same state; the answers compared by caller
// ("[coll115c] ...": v0, the surface hit, the hit point's distance).

namespace coll115
{
    inline bool on(const PS2Runtime *runtime)
    {
        return sotc_decomp::switchOn(sotc_decomp::kColl115) && !sotc_decomp::onWorkerLane(runtime);
    }
    inline uint64_t nowUs()
    {
#if defined(__vita__)
        return sceKernelGetProcessTimeWide();
#else
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
    }
    struct Params
    {
        size_t capBytes = 2u << 20;
        bool log = false;
        int check = 0;
        Params()
        {
            if (const char *e = std::getenv("SOTC_COLL115_MB"))
                capBytes = static_cast<size_t>(std::atof(e) * 1048576.0);
            log = std::getenv("SOTC_COLL115_LOG") && std::atoi(std::getenv("SOTC_COLL115_LOG")) != 0;
            if (const char *e = std::getenv("SOTC_COLL115_CHECK"))
                check = std::atoi(e);
        }
    };
    inline const Params &params()
    {
        static const Params p;
        return p;
    }

    struct Stats
    {
        uint64_t walks = 0, walkFallback = 0, xformFallback = 0, nodes = 0, leaves = 0, leafFallback = 0, surfaces = 0, candidates = 0, clips = 0,
                 hits = 0, overflows = 0, maxDepth = 0, leafBuilds = 0, builtSurfaces = 0, modelDrops = 0, leafDrops = 0, evictions = 0, queries = 0;
        uint64_t frame = 0;
        uint64_t objects = 0, objFiltered = 0, objNear = 0, partsNear = 0, surfLoop = 0, skinParts = 0, skinSurf = 0, skinMade = 0;
        uint64_t nQ[2] = {}, usQ[2] = {}, nW[2] = {}, usW[2] = {}; // by state (0 the original, 1 coll115): ClipCollisionDirectCallBack, its tree walks
    };
    inline Stats g_st;

    // A leaf's host block: count surfaces in (count + 3) / 4 groups of four
    // at pool[first]: cx, cy, cz, r, nx, ny, nz, d (4 floats each, 32 floats
    // a group); their ids at idPool[first / 8 ...]. ok false: a surface the host form does not
    // cover (the original leaf runs).
    struct Leaf
    {
        uint32_t word = 0;
        uint32_t first = 0;
        uint16_t count = 0;
        bool built = false, ok = false;
    };
    struct Model
    {
        uint32_t tree = 0, ids = 0, leaves = 0, nodes = 0, record = 0, verts = 0, list = 0, so = 0;
        uint64_t rootWords = 0, vertWords = 0, listWords = 0;
        bool packed = false, shortIds = false;
        uint64_t checkedFrame = ~0ull, usedFrame = 0;
        std::vector<Leaf> leaf;
        std::vector<float> pool; // (floats, not v4: no over-aligned heap blocks; loaded through memcpy)
        std::vector<uint16_t> idPool;
        size_t bytes() const { return pool.capacity() * sizeof(float) + idPool.capacity() * 2u + leaf.capacity() * sizeof(Leaf); }
    };
    inline std::unordered_map<uint32_t, Model> g_models;
    inline int g_depth = 0;
    inline size_t g_bytes = 0;

    inline uint64_t words64(const G g, uint32_t a) { return static_cast<uint64_t>(g.u32(a)) | (static_cast<uint64_t>(g.u32(a + 4u)) << 32); }

    // The part-0 vertex record of the class's collision DB, its fields.
    struct Record
    {
        uint32_t record, verts, list, so;
        bool packed, shortIds;
    };
    inline bool recordOf(const G g, uint32_t cls, Record &r)
    {
        if (cls == 0u || !G::plain(cls, 0x200u))
            return false;
        const uint32_t a = g.u32(cls + 0x184u);
        if (!G::plain(a, 0x24u))
            return false;
        const uint32_t db = g.u32(a + 0x20u);
        if (!G::plain(db, 0x30u))
            return false;
        r.record = g.u32(db + 8u);
        if (!G::plain(r.record, 0x90u))
            return false;
        r.packed = g.u8(r.record + 9u) != 0u;
        r.shortIds = g.u8(r.record + 8u) != 0u;
        r.verts = g.u32(r.record + 4u);
        r.list = g.u32(r.record + 0x14u);
        r.so = g.u32(r.record + 0xCu);
        return G::plain(r.verts, 16u) && G::plain(r.list, 16u) && (!r.packed || G::plain(r.so, 0x20u));
    }

    inline void dropModel(Model &m)
    {
        g_bytes -= std::min(g_bytes, m.bytes());
        m.leaf.clear();
        m.leaf.shrink_to_fit();
        m.pool.clear();
        m.pool.shrink_to_fit();
        m.idPool.clear();
        m.idPool.shrink_to_fit();
        ++g_st.modelDrops;
    }
    // The last walks' class checks (treeScale, model): 16 entries by (tree, class, leaf matrix), kept within a
    // game frame while both matrices' 128 bytes stay the same; cleared when models go.
    struct Recent
    {
        uint32_t tree = 0, cls = 0, m158 = 0, m15c = 0;
        uint64_t frame = ~0ull;
        float k = 0.0f;
        void *model = nullptr;
        float mats[32];
    };
    inline Recent g_recent[16];
    inline void forgetRecent()
    {
        for (Recent &r : g_recent)
            r.frame = ~0ull;
    }
    inline void evict()
    {
        if (g_bytes <= params().capBytes)
            return;
        ++g_st.evictions;
        forgetRecent();
        const uint64_t frame = sotc_decomp::g_gameFrame;
        for (auto it = g_models.begin(); it != g_models.end();)
        {
            if (it->second.usedFrame != frame)
            {
                g_bytes -= std::min(g_bytes, it->second.bytes());
                it = g_models.erase(it);
            }
            else
                ++it;
        }
        if (g_bytes > params().capBytes)
        {
            g_models.clear();
            g_bytes = 0;
        }
    }

    // The model of tree `tree` for class cls, checked against the guest once a frame.
    inline Model *model(const G g, uint32_t tree, uint32_t cls)
    {
        Record r;
        if (!G::plain(tree, 0x14u) || !recordOf(g, cls, r))
            return nullptr;
        const uint32_t ids = g.u32(tree), leaves = g.u32(tree + 4u), nodes = g.u32(tree + 8u);
        if (!G::plain(ids, 2u) || !G::plain(leaves, 4u) || !G::plain(nodes, 8u))
            return nullptr;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        Model &m = g_models[tree];
        if (m.checkedFrame != frame || m.record != r.record)
        {
            const uint64_t rootWords = words64(g, nodes), vertWords = words64(g, r.verts), listWords = words64(g, r.list);
            const bool same = m.tree == tree && m.ids == ids && m.leaves == leaves && m.nodes == nodes && m.record == r.record && m.verts == r.verts &&
                              m.list == r.list && m.so == r.so && m.packed == r.packed && m.shortIds == r.shortIds && m.rootWords == rootWords &&
                              m.vertWords == vertWords && m.listWords == listWords;
            if (!same)
            {
                if (m.tree != 0u)
                    dropModel(m);
                m.tree = tree;
                m.ids = ids;
                m.leaves = leaves;
                m.nodes = nodes;
                m.record = r.record;
                m.verts = r.verts;
                m.list = r.list;
                m.so = r.so;
                m.packed = r.packed;
                m.shortIds = r.shortIds;
                m.rootWords = rootWords;
                m.vertWords = vertWords;
                m.listWords = listWords;
            }
            m.checkedFrame = frame;
        }
        m.usedFrame = frame;
        return &m;
    }

    // A triangle's three local vertices (the record's index list and vertex
    // format, as manager_sub_01114C70 reads them before its matrix).
    inline bool localVerts(const G g, const Model &m, uint32_t surface, float v[3][3])
    {
        uint32_t ids[3];
        for (uint32_t k = 0; k < 3u; ++k)
        {
            if (m.shortIds)
            {
                const uint32_t a = m.list + surface * 20u + 2u + k * 2u;
                if (!G::plain(a, 2u))
                    return false;
                ids[k] = g.u16(a);
            }
            else
            {
                const uint32_t a = m.list + surface * 0x34u + k * 4u;
                if (!G::plain(a, 4u))
                    return false;
                ids[k] = g.u32(a);
            }
        }
        float scale[4] = {}, offset[4] = {};
        if (m.packed)
            for (int i = 0; i < 3; ++i)
            {
                scale[i] = g.f32(m.so + 0x10u + i * 4u);
                offset[i] = g.f32(m.so + i * 4u);
            }
        for (uint32_t k = 0; k < 3u; ++k)
        {
            if (m.packed)
            {
                const uint32_t a = m.verts + ids[k] * 6u;
                if (!G::plain(a, 6u))
                    return false;
                for (uint32_t i = 0; i < 3u; ++i)
                    v[k][i] = static_cast<float>(static_cast<int16_t>(g.u16(a + i * 2u))) * 3.05175781e-05f * scale[i] + offset[i];
            }
            else if (m.shortIds)
            {
                const uint32_t a = m.verts + ids[k] * 12u;
                if (!G::plain(a, 12u))
                    return false;
                for (uint32_t i = 0; i < 3u; ++i)
                    v[k][i] = g.f32(a + i * 4u);
            }
            else
            {
                const uint32_t a = m.verts + ids[k] * 16u;
                if (!G::plain(a, 16u) || g.u32(a + 12u) != 0x3F800000u) // w as stored goes into the matrix: 1 only
                    return false;
                for (uint32_t i = 0; i < 3u; ++i)
                    v[k][i] = g.f32(a + i * 4u);
            }
            for (uint32_t i = 0; i < 3u; ++i)
                if (!std::isfinite(v[k][i]))
                    return false;
        }
        return true;
    }

    // The leaf's host block, built at its first visit (or again when its word changed).
    inline Leaf *leafOf(const G g, Model &m, uint32_t index)
    {
        if (index >= m.leaf.size())
        {
            const size_t before = m.leaf.capacity();
            m.leaf.resize(std::max<size_t>(index + 1u, m.leaf.size() * 2u));
            g_bytes += (m.leaf.capacity() - before) * sizeof(Leaf);
        }
        Leaf &l = m.leaf[index];
        const uint32_t wa = m.leaves + index * 4u;
        if (!G::plain(wa, 4u))
            return nullptr;
        const uint32_t word = g.u32(wa);
        if (l.built && l.word == word)
            return &l;
        if (l.built)
            ++g_st.leafDrops; // its old block stays in the pool until the model goes
        l.built = true;
        l.word = word;
        l.ok = false;
        const uint32_t count = word & 0xFFu;
        l.count = static_cast<uint16_t>(count);
        const uint32_t idsAt = m.ids + (word >> 8) * 2u;
        if (count == 0u || !G::plain(idsAt, count * 2u))
        {
            l.ok = count == 0u;
            return &l;
        }
        const uint32_t groups = (count + 3u) / 4u;
        const size_t poolBefore = m.pool.capacity(), idBefore = m.idPool.capacity();
        l.first = static_cast<uint32_t>(m.pool.size());
        m.pool.resize(m.pool.size() + groups * 32u);
        m.idPool.resize(m.idPool.size() + groups * 4u);
        float *const f = m.pool.data() + l.first;
        uint16_t *const idOut = m.idPool.data() + l.first / 8u;
        bool ok = true;
        for (uint32_t i = 0; i < groups * 4u; ++i)
        {
            float *const grp = f + (i / 4u) * 32u;
            const uint32_t lane = i & 3u;
            float c[3] = {1e18f, 1e18f, 1e18f}, r = 0.0f, n[3] = {0.0f, 0.0f, 0.0f}, d = 0.0f;
            uint16_t id = 0;
            if (i < count)
            {
                id = g.u16(idsAt + i * 2u);
                float v[3][3];
                if (!localVerts(g, m, id, v))
                {
                    ok = false;
                    break;
                }
                for (int k = 0; k < 3; ++k)
                {
                    const float lo = std::min(v[0][k], std::min(v[1][k], v[2][k])), hi = std::max(v[0][k], std::max(v[1][k], v[2][k]));
                    c[k] = 0.5f * (lo + hi);
                }
                for (int j = 0; j < 3; ++j)
                {
                    const float dx = v[j][0] - c[0], dy = v[j][1] - c[1], dz = v[j][2] - c[2];
                    r = std::max(r, std::sqrt(dx * dx + dy * dy + dz * dz));
                }
                // the game's plane: the unit normal of (v0 - v1) x (v1 - v2), w = -n.v0
                const float a0 = v[0][0] - v[1][0], a1 = v[0][1] - v[1][1], a2 = v[0][2] - v[1][2];
                const float b0 = v[1][0] - v[2][0], b1 = v[1][1] - v[2][1], b2 = v[1][2] - v[2][2];
                n[0] = a1 * b2 - a2 * b1;
                n[1] = a2 * b0 - a0 * b2;
                n[2] = a0 * b1 - a1 * b0;
                const float len = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
                if (len > 1e-20f && std::isfinite(len))
                {
                    for (float &x : n)
                        x /= len;
                    d = -(n[0] * v[0][0] + n[1] * v[0][1] + n[2] * v[0][2]);
                }
                else
                    n[0] = n[1] = n[2] = d = 0.0f; // degenerate: never thrown out by the plane, ClipCollisionDB decides
                ++g_st.builtSurfaces;
            }
            grp[0 + lane] = c[0];
            grp[4 + lane] = c[1];
            grp[8 + lane] = c[2];
            grp[12 + lane] = r;
            grp[16 + lane] = n[0];
            grp[20 + lane] = n[1];
            grp[24 + lane] = n[2];
            grp[28 + lane] = d;
            idOut[i] = id;
        }
        l.ok = ok;
        ++g_st.leafBuilds;
        g_bytes += (m.pool.capacity() - poolBefore) * sizeof(float) + (m.idPool.capacity() - idBefore) * 2u;
        return &l;
    }

    // ---- the walk -----------------------------------------------------------
    struct Ends
    {
        float a[4], b[4];
    };
    struct Ctx
    {
        G g;
        Seg s;
        Model *m;
        uint32_t nodes, leafSp, m158, mode;
        float scale;   // the split scale (t1 + 0x14)
        float inv;     // tree space -> local space (1 / the class's uniform scale)
        float epsAbs;  // the margins in local units
    };

    // The class's matrices: tree space -> world (t1 + 0xc, the leaf's) and the
    // part-0 local -> world (class + 0x15c, the vertices'): local = tree / k
    // when the second is k times the first (rotation and translation the same).
    inline bool treeScale(const G g, uint32_t m158, uint32_t cls, float &k)
    {
        if (!G::plain(m158, 0x40u) || !G::plain(cls + 0x15Cu, 4u))
            return false;
        const uint32_t m15c = g.u32(cls + 0x15Cu);
        if (!G::plain(m15c, 0x40u))
            return false;
        float a[16], b[16];
        std::memcpy(a, g.rdram + m158, 64);
        std::memcpy(b, g.rdram + m15c, 64);
        // rows 0-2 the basis (x, y, z axes), row 3 the translation (the VU0 layout)
        const float na = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
        if (!(na > 1e-20f) || !std::isfinite(na))
            return false;
        const float kk = std::sqrt((b[0] * b[0] + b[1] * b[1] + b[2] * b[2]) / na);
        if (!(kk > 1e-6f) || !std::isfinite(kk))
            return false;
        float big = 0.0f;
        for (int i = 0; i < 12; ++i)
            big = std::max(big, std::fabs(a[i]));
        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (std::fabs(b[row * 4 + col] - kk * a[row * 4 + col]) > 1e-4f * kk * (big + 1e-6f))
                    return false;
        for (int col = 0; col < 3; ++col)
            if (std::fabs(b[12 + col] - a[12 + col]) > 1e-3f + 1e-5f * std::fabs(a[12 + col]))
                return false;
        if (a[3] != 0.0f || a[7] != 0.0f || a[11] != 0.0f || a[15] != 1.0f || b[3] != 0.0f || b[7] != 0.0f || b[11] != 0.0f || b[15] != 1.0f)
            return false;
        // no mirror: the planes' sides must map as they are
        const float det = a[0] * (a[5] * a[10] - a[6] * a[9]) - a[1] * (a[4] * a[10] - a[6] * a[8]) + a[2] * (a[4] * a[9] - a[5] * a[8]);
        if (!(det > 0.0f))
            return false;
        k = kk;
        return true;
    }

    // One group of four surfaces against the piece (local space): bit i set
    // when surface i's sphere meets the piece (within the margins) and the
    // piece crosses its plane the way the side mode asks (mode 0: the start
    // in front, d >= 0, and the end behind, d <= 0; 2 the other way; 1 either).
    struct Piece
    {
        float ax, ay, az, dx, dy, dz, bx, by, bz, invLen2, eps;
        uint32_t mode;
    };
#if PS2X_VU_NEON_A32
    // ARMv7 NEON (GCC leaves generic float vectors to the VFP, lane by lane,
    // without -funsafe-math-optimizations); flush-to-zero changes nothing at
    // these margins.
    inline uint32_t groupBits(const float *blk, const Piece &q)
    {
        const float32x4_t cx = vld1q_f32(blk), cy = vld1q_f32(blk + 4), cz = vld1q_f32(blk + 8), r = vld1q_f32(blk + 12);
        const float32x4_t wx = vsubq_f32(cx, vdupq_n_f32(q.ax)), wy = vsubq_f32(cy, vdupq_n_f32(q.ay)), wz = vsubq_f32(cz, vdupq_n_f32(q.az));
        const float32x4_t Dx = vdupq_n_f32(q.dx), Dy = vdupq_n_f32(q.dy), Dz = vdupq_n_f32(q.dz);
        float32x4_t t = vmulq_f32(wx, Dx);
        t = vmlaq_f32(t, wy, Dy);
        t = vmlaq_f32(t, wz, Dz);
        t = vmulq_f32(t, vdupq_n_f32(q.invLen2));
        t = vminq_f32(vmaxq_f32(t, vdupq_n_f32(0.0f)), vdupq_n_f32(1.0f));
        const float32x4_t px = vmlsq_f32(wx, t, Dx), py = vmlsq_f32(wy, t, Dy), pz = vmlsq_f32(wz, t, Dz);
        float32x4_t d2 = vmulq_f32(px, px);
        d2 = vmlaq_f32(d2, py, py);
        d2 = vmlaq_f32(d2, pz, pz);
        const float32x4_t rr = vmlaq_f32(vdupq_n_f32(q.eps), r, vdupq_n_f32(1.0001f));
        // The tests as signs of differences, in integer lanes: GCC turns NEON float compares into four VFP
        // compares and flag moves each (no -funsafe-math-optimizations). x >= 0 here when its sign bit is clear
        // (a difference of finite floats is never -0 but for equal -0 inputs; the margins cover the boundary).
        const uint32x4_t sphereNeg = vreinterpretq_u32_f32(vmlsq_f32(vmulq_f32(rr, rr), d2, vdupq_n_f32(1.0f)));
        const float32x4_t nx = vld1q_f32(blk + 16), ny = vld1q_f32(blk + 20), nz = vld1q_f32(blk + 24), nd = vld1q_f32(blk + 28);
        float32x4_t da = vmlaq_f32(nd, nx, vdupq_n_f32(q.ax));
        da = vmlaq_f32(da, ny, vdupq_n_f32(q.ay));
        da = vmlaq_f32(da, nz, vdupq_n_f32(q.az));
        float32x4_t db = vmlaq_f32(nd, nx, vdupq_n_f32(q.bx));
        db = vmlaq_f32(db, ny, vdupq_n_f32(q.by));
        db = vmlaq_f32(db, nz, vdupq_n_f32(q.bz));
        const float32x4_t e = vdupq_n_f32(q.eps);
        // front: da + eps >= 0 and eps - db >= 0; back: db + eps >= 0 and eps - da >= 0
        const uint32x4_t frontNeg = vorrq_u32(vreinterpretq_u32_f32(vaddq_f32(da, e)), vreinterpretq_u32_f32(vsubq_f32(e, db)));
        const uint32x4_t backNeg = vorrq_u32(vreinterpretq_u32_f32(vaddq_f32(db, e)), vreinterpretq_u32_f32(vsubq_f32(e, da)));
        const uint32x4_t planeNeg = q.mode == 1u ? vandq_u32(frontNeg, backNeg) : q.mode == 2u ? backNeg : frontNeg;
        const uint32x4_t fail = vshrq_n_u32(vorrq_u32(sphereNeg, planeNeg), 31); // 1 where a test failed
        static const uint32_t kw[4] = {1u, 2u, 4u, 8u};
        const uint32x4_t w = vbicq_u32(vld1q_u32(kw), vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(fail)))); // the passing lanes' weights
        uint32x2_t h = vorr_u32(vget_low_u32(w), vget_high_u32(w));
        h = vorr_u32(h, vrev64_u32(h));
        return vget_lane_u32(h, 0);
    }
#else
    inline uint32_t groupBits(const float *blk, const Piece &q)
    {
        uint32_t bits = 0u;
        for (int i = 0; i < 4; ++i)
        {
            const float wx = blk[i] - q.ax, wy = blk[4 + i] - q.ay, wz = blk[8 + i] - q.az, r = blk[12 + i];
            float t = (wx * q.dx + wy * q.dy + wz * q.dz) * q.invLen2;
            t = std::min(std::max(t, 0.0f), 1.0f);
            const float px = wx - t * q.dx, py = wy - t * q.dy, pz = wz - t * q.dz;
            const float rr = r * 1.0001f + q.eps;
            if (!(px * px + py * py + pz * pz <= rr * rr))
                continue;
            const float nx = blk[16 + i], ny = blk[20 + i], nz = blk[24 + i], nd = blk[28 + i];
            const float da = nx * q.ax + ny * q.ay + nz * q.az + nd, db = nx * q.bx + ny * q.by + nz * q.bz + nd;
            const bool front = da >= -q.eps && db <= q.eps, back = db >= -q.eps && da <= q.eps;
            if (q.mode == 1u ? (front || back) : q.mode == 2u ? back : front)
                bits |= 1u << i;
        }
        return bits;
    }
#endif

    // The original leaf's per-surface tail for one candidate (the filters,
    // ClipCollisionDB, the hit) with the world piece at sp / sp + 0x10.
    inline bool candidate(Ctx &c, uint32_t surface, uint32_t sp)
    {
        const G g = c.g;
        R5900Context *ctx = g.ctx;
        const uint32_t p = c.s.t1;
        const uint32_t mode = c.mode;
        const uint32_t raSurf = mode == 1 ? 0x112C1E0u : mode == 2 ? 0x112C3D0u : 0x112C008u;
        const uint32_t raId = mode == 1 ? 0x112C22Cu : mode == 2 ? 0x112C41Cu : 0x112C054u;
        const uint32_t raBoundary = mode == 1 ? 0x112C130u : mode == 2 ? 0x112C320u : 0x112BF58u;
        const uint32_t idSlot = mode == 1 ? 0x60u : mode == 2 ? 0x70u : 0x50u;
        if (const uint32_t filter = g.u32(p + 0x1Cu); filter != 0u)
        {
            // the surface filter reads the surface's frame words: its world boundary (w = 1) and that in the segment's space
            const uint32_t bnd = boundary(g, g.u32(p + 4u), 0u, surface, raBoundary, sp);
            const __m128 bound = _mm_blendv_ps(g.q(bnd), _mm_set1_ps(1.0f), _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
            g.put(sp + 0x20u, bound);
            g.put(sp + 0x30u, transform(g, g.u32(p + 0x10u), bound));
            SET_GPR_U32(ctx, 4, sp + 0x20u);
            SET_GPR_U32(ctx, 5, g.u32(p + 0x10u));
            SET_GPR_U32(ctx, 6, g.u32(p + 0x24u));
            guestCall(g.rdram, ctx, g.runtime, filter, raSurf, sp);
            if (GPR_U64(ctx, 2) == 0u)
                return false;
        }
        const uint32_t lo = g.u32(p), hi = surface | 0x80000000u;
        g.put32(sp + idSlot, lo);
        g.put32(sp + idSlot + 4u, hi);
        g.put64(sp + 0x40u, static_cast<uint64_t>(lo) | (static_cast<uint64_t>(hi) << 32));
        if (const uint32_t filter = g.u32(p + 0x20u); filter != 0u)
        {
            uint64_t seen;
            if (idFilter(g, filter, sp + 0x40u, g.u32(p + 0x24u), seen, sp))
            {
                if (seen == 0u)
                    return false;
            }
            else
            {
                SET_GPR_U32(ctx, 4, sp + 0x40u);
                SET_GPR_U32(ctx, 5, g.u32(p + 0x24u));
                SET_GPR_U32(ctx, 6, filter);
                SET_GPR_U32(ctx, 7, sp + 0x40u);
                guestCall(g.rdram, ctx, g.runtime, filter, raId, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    return false;
            }
        }
        ++g_st.clips;
        uint64_t clipped;
        if (mode == 1)
        {
            clipped = clipDBT<true>(g, c.s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C244u, sp);
            if (clipped == 0u)
                clipped = clipDBT<true>(g, c.s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C260u, sp);
        }
        else if (mode == 2)
            clipped = clipDBT<true>(g, c.s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C434u, sp);
        else
            clipped = clipDBT<true>(g, c.s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C06Cu, sp);
        if (clipped == 0u)
            return false;
        const uint32_t idLo = g.u32(sp + 0x40u), idHi = g.u32(sp + 0x44u);
        const __m128 point = g.q(c.s.a0 + 0x40u);
        g.put32(c.s.a0, idLo);
        g.put32(c.s.a0 + 4u, idHi);
        g.put(sp + 0x10u, point);
        return true;
    }

    // One leaf for the piece e (tree space): 1 a hit, 0 none.
    inline int leaf(Ctx &c, const Ends &e, uint32_t index)
    {
        ++g_st.leaves;
        const G g = c.g;
        Leaf *l = leafOf(g, *c.m, index);
        if (!l || !l->ok)
        {
            ++g_st.leafFallback;
            g_countLeaf.hit();
            __m128 ends[2];
            std::memcpy(&ends[0], e.a, 16);
            std::memcpy(&ends[1], e.b, 16);
            return leafT<true>(g, c.s, 0u, 0u, index, c.leafSp + 0xD0u, ends) != 0u ? 1 : 0;
        }
        const uint32_t count = l->count;
        if (count == 0u)
            return 0;
        g_st.surfaces += count;
        // the piece in local space
        const float k = c.inv;
        Piece q;
        q.ax = e.a[0] * k;
        q.ay = e.a[1] * k;
        q.az = e.a[2] * k;
        q.bx = e.b[0] * k;
        q.by = e.b[1] * k;
        q.bz = e.b[2] * k;
        q.dx = q.bx - q.ax;
        q.dy = q.by - q.ay;
        q.dz = q.bz - q.az;
        const float len2 = q.dx * q.dx + q.dy * q.dy + q.dz * q.dz;
        q.invLen2 = len2 > 1e-30f ? 1.0f / len2 : 0.0f;
        q.eps = c.epsAbs;
        q.mode = c.mode;
        const float *blk = c.m->pool.data() + l->first;
        const uint16_t *ids = c.m->idPool.data() + l->first / 8u;
        const uint32_t groups = (count + 3u) / 4u;
        const uint32_t sp = c.leafSp;
        bool piecePut = false, hit = false;
        for (uint32_t gi = 0; gi < groups; ++gi, blk += 32, ids += 4)
        {
            uint32_t bits = groupBits(blk, q);
            if (gi == groups - 1u && (count & 3u) != 0u)
                bits &= (1u << (count & 3u)) - 1u;
            while (bits != 0u)
            {
                const uint32_t lane = static_cast<uint32_t>(__builtin_ctz(bits));
                bits &= bits - 1u;
                ++g_st.candidates;
                if (!piecePut)
                {
                    // the piece in world space as the original leaf puts it (its tree-to-world matrix)
                    __m128 a, b;
                    std::memcpy(&a, e.a, 16);
                    std::memcpy(&b, e.b, 16);
                    g.put(sp, transform(g, c.m158, a));
                    g.put(sp + 0x10u, transform(g, c.m158, b));
                    piecePut = true;
                }
                if (candidate(c, ids[lane], sp))
                {
                    hit = true;
                    ++g_st.hits;
                }
            }
        }
        return hit ? 1 : 0;
    }

    // The walk from node t0 (sidesF's order; first >= 0: only that side of t0,
    // as low() / high()). 1 a hit, 0 none, -1 not taken (deeper than the host stack: the original from the start).
    // A float's bits as a signed integer that orders as the float does (-0 just below +0; no NaN here).
    inline int32_t key(float f)
    {
        int32_t b;
        std::memcpy(&b, &f, 4);
        return b ^ ((b >> 31) & 0x7FFFFFFF);
    }
    // Unit: the split scale is 1 (the splits compare as they are, in core registers: no VFP compare and
    // flag move per test).
    template <bool Unit>
    int walkT(Ctx &c, Ends e, uint32_t t0, int first)
    {
        struct Pend
        {
            Ends e;
            uint32_t t0;
            bool high;
        };
        constexpr int kDepth = 128;
        Pend stack[kDepth];
        int top = 0;
        bool sides = first < 0, high = first > 0;
        uint8_t *const rd = c.g.rdram;
        for (;;)
        {
            if (!G::plain(t0, 8u))
                return 0;
            float split;
            uint32_t word;
            std::memcpy(&split, rd + t0, 4);
            std::memcpy(&word, rd + t0 + 4u, 4);
            ++g_st.nodes;
            const uint32_t axis = word & 3u;
            const float f1 = e.a[axis], f0 = e.b[axis];
            if (sides)
            {
                high = Unit ? key(f0) <= key(f1) : f0 <= f1;
                if (top == kDepth)
                {
                    // Deeper than the host stack: given back to the original walk from the start when no leaf
                    // has hit yet (nothing but frames below sp and f0 written: the result record only on a hit).
                    ++g_st.overflows;
                    return -1;
                }
                if (static_cast<uint64_t>(top) >= g_st.maxDepth)
                    g_st.maxDepth = static_cast<uint64_t>(top) + 1u;
                Pend &p = stack[top++];
                p.e = e;
                p.t0 = t0;
                p.high = !high;
            }
            const float s = Unit ? split : split * c.scale;
            bool in0, in1;
            if constexpr (Unit)
            {
                const int32_t ks = key(s), k1 = key(f1), k0 = key(f0);
                in0 = high ? ks <= k1 : k1 <= ks;
                in1 = high ? ks <= k0 : k0 <= ks;
            }
            else
            {
                in0 = high ? s <= f1 : f1 <= s;
                in1 = high ? s <= f0 : f0 <= s;
            }
            int r = 0;
            if (in0 || in1)
            {
                const uint32_t child = high ? (word >> 17) & 0x3FFFu : (word >> 2) & 0x3FFFu;
                const bool isLeaf = high ? static_cast<int32_t>(word) < 0 : ((word >> 16) & 1u) != 0u;
                if (isLeaf)
                    r = leaf(c, e, child);
                else
                {
                    if (in0 != in1)
                    {
                        float t, num, den;
                        if (high != in0)
                        {
                            den = f0 - f1;
                            num = s - f1;
                        }
                        else
                        {
                            den = f1 - f0;
                            num = s - f0;
                        }
                        t = num / den;
                        const bool aFirst = high == in0;
                        const float *P = aFirst ? e.a : e.b, *Q = aFirst ? e.b : e.a;
                        float rr[4];
                        for (int i = 0; i < 3; ++i)
                            rr[i] = P[i] * t + Q[i] * (1.0f - t);
                        rr[3] = 1.0f;
                        std::memcpy(in0 ? e.b : e.a, rr, 16);
                    }
                    t0 = c.nodes + child * 8u;
                    sides = true;
                    continue;
                }
            }
            if (r == 1)
                return 1;
            if (top == 0)
                return 0;
            const Pend &p = stack[--top];
            e = p.e;
            t0 = p.t0;
            high = p.high;
            sides = false;
        }
    }

    inline int walk(Ctx &c, const Ends &e, uint32_t t0, int first) { return c.scale == 1.0f ? walkT<true>(c, e, t0, first) : walkT<false>(c, e, t0, first); }

    // The walk for the test s from the ends at a1 / a2 (tree space, guest
    // qwords) from node t0 at the frame sp; false: not taken (the caller runs
    // the original).
    inline bool run(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp, int first, uint64_t &out)
    {
        ++g_st.walks;
        const uint32_t p = s.t1;
        // (a walk reached from a filter inside a walk: the original, the host blocks stay as they are)
        if (g_depth != 0 || !G::plain(p, 0x28u) || !G::plain(a1, 16u) || !G::plain(a2, 16u) || !G::plain(s.a0, 0x50u) || sp < 0x400u || !G::plain(t0, 8u))
        {
            ++g_st.walkFallback;
            return false;
        }
        const uint32_t cls = g.u32(p + 4u), m158 = g.u32(p + 0xCu);
        float k;
        Model *m = nullptr;
        const uint32_t m15c = G::plain(cls + 0x15Cu, 4u) ? g.u32(cls + 0x15Cu) : 0u;
        Recent &rc = g_recent[((s.a3 >> 4) ^ (cls >> 6) ^ (m158 >> 6)) & 15u];
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (rc.frame == frame && rc.tree == s.a3 && rc.cls == cls && rc.m158 == m158 && rc.m15c == m15c && G::plain(m158, 0x40u) && G::plain(m15c, 0x40u) &&
            std::memcmp(rc.mats, g.rdram + m158, 64) == 0 && std::memcmp(rc.mats + 16, g.rdram + m15c, 64) == 0)
        {
            k = rc.k;
            m = static_cast<Model *>(rc.model);
        }
        else
        {
            if (!treeScale(g, m158, cls, k))
            {
                ++g_st.xformFallback;
                return false;
            }
            m = model(g, s.a3, cls);
            if (!m)
            {
                ++g_st.walkFallback;
                return false;
            }
            rc.tree = s.a3;
            rc.cls = cls;
            rc.m158 = m158;
            rc.m15c = m15c;
            rc.frame = frame;
            rc.k = k;
            rc.model = m;
            std::memcpy(rc.mats, g.rdram + m158, 64);
            std::memcpy(rc.mats + 16, g.rdram + m15c, 64);
        }
        Ctx c{g, s, m, m->nodes, (sp - 0x100u) & ~0xFu, m158, g.u32(p + 0x18u), g.f32(p + 0x14u), 1.0f / k, 0.0f};
        if (c.mode != 1u && c.mode != 2u)
            c.mode = 0u;
        c.epsAbs = 2e-3f * c.inv; // 2 mm in world units
        Ends e;
        std::memcpy(e.a, g.rdram + (a1 & ~0xFu), 16);
        std::memcpy(e.b, g.rdram + (a2 & ~0xFu), 16);
        e.a[3] = e.b[3] = 1.0f;
        ++g_depth;
        const int r = walk(c, e, t0, first);
        --g_depth;
        if (r < 0)
            return false;
        out = static_cast<uint64_t>(r);
        if (g_depth == 0)
            evict();
        return true;
    }

    inline void tick()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (!params().log || frame < g_st.frame + 600u)
            return;
        const double n = static_cast<double>(frame - g_st.frame);
        if (g_st.frame != 0u)
            std::fprintf(stderr,
                         "[coll115] frame %llu: a frame: queries %.1f walks %.1f (fallback %.2f, matrices %.2f) nodes %.1f leaves %.1f (original %.2f) surfaces %.1f "
                         "candidates %.2f clips %.2f hits %.2f overflows %.3f (deepest %u) | leaves built %.2f surfaces built %.1f leaves redone %.2f models dropped %.2f evictions %.2f | "
                         "models %u, %.2f MB\n",
                         (unsigned long long)frame, g_st.queries / n, g_st.walks / n, g_st.walkFallback / n, g_st.xformFallback / n, g_st.nodes / n, g_st.leaves / n,
                         g_st.leafFallback / n, g_st.surfaces / n, g_st.candidates / n, g_st.clips / n, g_st.hits / n, g_st.overflows / n, static_cast<unsigned>(g_st.maxDepth), g_st.leafBuilds / n, g_st.builtSurfaces / n,
                         g_st.leafDrops / n, g_st.modelDrops / n, g_st.evictions / n, static_cast<unsigned>(g_models.size()), g_bytes / 1048576.0);
        if (g_st.frame != 0u)
            std::fprintf(stderr, "[coll115o] frame %llu: a frame: objects %.1f (filtered out %.1f, near %.1f), parts near %.1f, one-by-one surfaces %.1f, skinned parts %.2f surfaces %.1f (made %.1f)\n",
                         (unsigned long long)frame, g_st.objects / n, g_st.objFiltered / n, g_st.objNear / n, g_st.partsNear / n, g_st.surfLoop / n,
                         g_st.skinParts / n, g_st.skinSurf / n, g_st.skinMade / n);
        if (g_st.frame != 0u)
            for (int i = 0; i < 2; ++i)
                if (g_st.nQ[i] != 0u)
                    std::fprintf(stderr, "[coll115t] frame %llu %s: queries %llu, %.1f us each (%.3f ms a frame of them), tree walks %llu, %.1f us each\n",
                                 (unsigned long long)frame, i ? "coll115" : "original", (unsigned long long)g_st.nQ[i], (double)g_st.usQ[i] / g_st.nQ[i],
                                 (double)g_st.usQ[i] / 1000.0 / n * (double)(g_st.nQ[0] + g_st.nQ[1]) / g_st.nQ[i], (unsigned long long)g_st.nW[i],
                                 g_st.nW[i] ? (double)g_st.usW[i] / g_st.nW[i] : 0.0);
        g_st = Stats{};
        g_st.frame = frame;
    }

    // ---- SOTC_COLL115_CHECK: the query both ways ----------------------------
    struct CheckStat
    {
        uint64_t checks = 0, hitDiffer = 0, idDiffer = 0, idTie = 0, both = 0;
        double maxDist = 0.0, sumDist = 0.0;
    };
    inline std::unordered_map<uint32_t, CheckStat> g_check;
    inline uint64_t g_checkFrame = 0;
    inline bool g_forceOff = false;
    inline void checkTick()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_checkFrame + 600u)
            return;
        g_checkFrame = frame;
        std::fprintf(stderr, "[coll115c] frame %llu:", (unsigned long long)frame);
        for (auto &e : g_check)
        {
            const CheckStat &s = e.second;
            std::fprintf(stderr, " %08x n %llu hit-differ %llu id-differ %llu (same point %llu) both-hit %llu dist max %.5f mean %.6f;", e.first,
                         (unsigned long long)s.checks, (unsigned long long)s.hitDiffer, (unsigned long long)s.idDiffer, (unsigned long long)s.idTie,
                         (unsigned long long)s.both, s.maxDist, s.both ? s.sumDist / static_cast<double>(s.both) : 0.0);
        }
        std::fprintf(stderr, "\n");
    }
    template <typename Body>
    void checked(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Body body)
    {
        alignas(16) static uint8_t saved[sizeof(R5900Context)];
        static uint8_t recA[0x50], recSaved[0x50];
        const uint32_t out = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31);
        if (!G::plain(out, 0x50u))
            return body();
        std::memcpy(saved, ctx, sizeof(R5900Context));
        std::memcpy(recSaved, rdram + out, 0x50);
        g_forceOff = true;
        body();
        g_forceOff = false;
        const uint64_t v0A = GPR_U64(ctx, 2);
        std::memcpy(recA, rdram + out, 0x50);
        std::memcpy(ctx, saved, sizeof(R5900Context));
        std::memcpy(rdram + out, recSaved, 0x50);
        body();
        const uint64_t v0B = GPR_U64(ctx, 2);
        const uint8_t *recB = rdram + out;
        CheckStat &s = g_check[ra];
        ++s.checks;
        if ((v0A != 0u) != (v0B != 0u))
            ++s.hitDiffer;
        else if (v0A != 0u)
        {
            ++s.both;
            float pa[3], pb[3];
            std::memcpy(pa, recA + 0x40, 12);
            std::memcpy(pb, recB + 0x40, 12);
            const double dist = std::sqrt((double)(pa[0] - pb[0]) * (pa[0] - pb[0]) + (double)(pa[1] - pb[1]) * (pa[1] - pb[1]) + (double)(pa[2] - pb[2]) * (pa[2] - pb[2]));
            s.maxDist = std::max(s.maxDist, dist);
            s.sumDist += dist;
            if (std::memcmp(recA, recB, 8) != 0)
            {
                ++s.idDiffer;
                if (dist < 1e-3)
                    ++s.idTie;
            }
        }
        checkTick();
    }
    inline bool active(const PS2Runtime *runtime) { return !g_forceOff && on(runtime); }

    // SOTC_COLL115_CHECK=2: each walk the original way (orig) and then this
    // way from the same result record; compared by site ("[coll115w] ...").
    inline CheckStat g_walkCheck[2];
    inline uint64_t g_walkCheckFrame = 0;
    template <typename Orig>
    bool walkChecked(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp, int first, uint64_t &out, int site, Orig orig)
    {
        if (params().check != 2 || !G::plain(s.a0, 0x50u))
            return run(g, s, a1, a2, t0, sp, first, out);
        uint8_t before[0x50], recA[0x50];
        std::memcpy(before, g.rdram + s.a0, 0x50);
        const uint64_t rA = orig();
        std::memcpy(recA, g.rdram + s.a0, 0x50);
        std::memcpy(g.rdram + s.a0, before, 0x50);
        if (!run(g, s, a1, a2, t0, sp, first, out))
        {
            std::memcpy(g.rdram + s.a0, recA, 0x50);
            out = rA;
            return true;
        }
        const uint8_t *recB = g.rdram + s.a0;
        CheckStat &st = g_walkCheck[site & 1];
        ++st.checks;
        {
            float pa[3], pb[3];
            std::memcpy(pa, recA + 0x40, 12);
            std::memcpy(pb, recB + 0x40, 12);
            const float dd = std::fabs(pa[0] - pb[0]) + std::fabs(pa[1] - pb[1]) + std::fabs(pa[2] - pb[2]);
            static int shown = 0;
            if (((rA != 0u) != (out != 0u) || (rA != 0u && (std::memcmp(recA, recB, 8) != 0 || dd > 0.01f))) && shown < 40)
            {
                ++shown;
                float e0[4], e1[4];
                std::memcpy(e0, g.rdram + (a1 & ~0xFu), 16);
                std::memcpy(e1, g.rdram + (a2 & ~0xFu), 16);
                uint32_t ia[2], ib[2];
                std::memcpy(ia, recA, 8);
                std::memcpy(ib, recB, 8);
                std::fprintf(stderr,
                             "[coll115m] frame %llu site %d first %d tree %08x cls %08x mode %u filters %08x %08x ends (%.3f %.3f %.3f) (%.3f %.3f %.3f) | orig %llu id %08x %08x at (%.3f %.3f %.3f) | new %llu id %08x %08x at (%.3f %.3f %.3f)\n",
                             (unsigned long long)sotc_decomp::g_gameFrame, site, first, s.a3, g.u32(s.t1 + 4u), g.u32(s.t1 + 0x18u), g.u32(s.t1 + 0x1Cu),
                             g.u32(s.t1 + 0x20u), e0[0], e0[1], e0[2], e1[0], e1[1], e1[2], (unsigned long long)rA, ia[0], ia[1], pa[0], pa[1], pa[2],
                             (unsigned long long)out, ib[0], ib[1], pb[0], pb[1], pb[2]);
            }
        }
        if ((rA != 0u) != (out != 0u))
            ++st.hitDiffer;
        else if (rA != 0u)
        {
            ++st.both;
            float pa[3], pb[3];
            std::memcpy(pa, recA + 0x40, 12);
            std::memcpy(pb, recB + 0x40, 12);
            const double dist = std::sqrt((double)(pa[0] - pb[0]) * (pa[0] - pb[0]) + (double)(pa[1] - pb[1]) * (pa[1] - pb[1]) + (double)(pa[2] - pb[2]) * (pa[2] - pb[2]));
            st.maxDist = std::max(st.maxDist, dist);
            st.sumDist += dist;
            if (std::memcmp(recA, recB, 8) != 0)
            {
                ++st.idDiffer;
                if (dist < 1e-3)
                    ++st.idTie;
            }
        }
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame >= g_walkCheckFrame + 600u)
        {
            g_walkCheckFrame = frame;
            for (int i = 0; i < 2; ++i)
            {
                const CheckStat &w = g_walkCheck[i];
                std::fprintf(stderr, "[coll115w] frame %llu %s: n %llu hit-differ %llu id-differ %llu (same point %llu) both-hit %llu dist max %.5f mean %.6f\n",
                             (unsigned long long)frame, i ? "objary" : "direct", (unsigned long long)w.checks, (unsigned long long)w.hitDiffer,
                             (unsigned long long)w.idDiffer, (unsigned long long)w.idTie, (unsigned long long)w.both, w.maxDist,
                             w.both ? w.sumDist / static_cast<double>(w.both) : 0.0);
            }
        }
        return true;
    }
}
