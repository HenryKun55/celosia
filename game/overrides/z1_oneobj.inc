// Round Z1 (SOTC_Z1=1 with d1native; z1_coll.h / z1_coll.cpp, docs/decomp_z1.md): ClipCollisionDirectCallBackOneObj
// (0x112ca30) and ps2__CalcCollisionMultiDepSurfBoundary (0x1124368) as host code on this file's collision helpers.
// Included at the end of decomp_collision.cpp (G, call, layoutClassAny, boundary, directFilter, d1Xf ...).
//
// ClipCollisionDirectCallBackOneObj(a0 result, a1 start, a2 end, a3 object, t0 part filter, t1 surface filter, t2 id
// filter, t3 user): the segment's frame (GetArbitraryDirectionInverseMatrix: sp the direction matrix, sp + 0x40 the
// inverse, f0 the length; negative: 0); the object's layout class, both enable bytes; its bound cylinder (DB +0x2c x
// the class's scale, its centre +0x134 in the segment's space); each part (DB +8, 0x90 bytes) with surfaces: its
// cylinder (+0x1c x scale, +0x20 through the part's matrix class +0x15c), the part filter, then the tree (0112ACD0 /
// 0112B890 / the root's two sides, nearer first) or one surface at a time (getLayoutClassCollisionIndepSurfBoundary,
// the cylinder, the surface and id filters, ClipCollisionDB); then the part's skinned surfaces (+0x50 count: the group's
// cylinder +0x5c / +0x60, the part filter, the part-to-segment matrix, each surface's boundary made when its cache byte is
// clear (CalcCollisionMultiDepSurfBoundary, EntryModifiedManager), the capsule test, the filters, ClipCollisionDB). A hit
// shortens the segment for the rest of the object; v0 1 when anything was hit, the result's +0x40 the last hit point.
//
// The frame (sp = entry sp - 0x260), only the words the callees read: +0 / +0x40 the segment's matrices (written by
// GetArbitraryDirectionInverseMatrix), +0xa0 the part centre (the part filter's a0), +0xb0 the skinned group's centre
// (its a0), +0xc0..+0xe4 the tree walk's record (object, class, length, the class's +0x158 matrices, sp + 0x40, scale,
// 0, surface filter, id filter, user), +0xf0 / +0x100 / +0x110 / +0x120 / +0x130 the walk's segment (start, |d|, d, the
// two ends in the part), +0xc0 / +0xd0 / +0xf0 the one-by-one loop's end / boundary / id record, +0xd0..+0x10c the
// part-to-segment matrix, +0x140 the skinned loop's end, +0x180 its id record.
namespace decomp_skinfinder
{
    int cornersD3Core(const d1::Mem m, const uint32_t args[8], uint32_t spCall, const uint64_t saved[10], __m128 vu[11]);
    void cornersSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_quat
{
    void absVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_collision
{
    // The VU0 registers and f0 as the original leaves them, kept on the host and stored into the context before each
    // call that may run guest code (or read them) and at the exit.
    struct Z1Vu
    {
        __m128 r[32];
        __m128 acc;
        uint32_t dirty = 0u;
        bool accDirty = false;
        float f0 = 0.0f;
        bool f0Dirty = false;
        PS2_FORCE_INLINE void set(int i, __m128 v)
        {
            r[i] = v;
            dirty |= 1u << i;
        }
        PS2_FORCE_INLINE void setAcc(__m128 v)
        {
            acc = v;
            accDirty = true;
        }
        PS2_FORCE_INLINE void setF0(float v)
        {
            f0 = v;
            f0Dirty = true;
        }
        PS2_FORCE_INLINE __m128 cur(const R5900Context *ctx, int i) const { return (dirty >> i) & 1u ? r[i] : ctx->vu0_vf[i]; }
        PS2_FORCE_INLINE void flush(R5900Context *ctx)
        {
            if (dirty)
            {
                uint32_t d = dirty;
                while (d)
                {
                    const int i = __builtin_ctz(d);
                    d &= d - 1u;
                    ctx->vu0_vf[i] = r[i];
                }
                dirty = 0u;
            }
            if (accDirty)
            {
                ctx->vu0_acc = acc;
                accDirty = false;
            }
            if (f0Dirty)
            {
                ctx->f[0] = f0;
                f0Dirty = false;
            }
        }
    };

    // lqc2 vf8, v; lqc2 vf14-vf17, m; vmulax / vmadday / vmaddaz ACC; vmaddw vf10: the point through the four rows at m
    // (d1Xf's value), with the registers it leaves.
    PS2_FORCE_INLINE __m128 z1Xf(const G g, Z1Vu &vu, uint32_t m, __m128 v)
    {
        const __m128 r0 = g.q(m), r1 = g.q(m + 0x10u), r2 = g.q(m + 0x20u), r3 = g.q(m + 0x30u);
        __m128 t = d1::vmul(r0, d1::bc<0>(v));
        t = d1::vmadd(t, r1, d1::bc<1>(v));
        t = d1::vmadd(t, r2, d1::bc<2>(v));
        const __m128 out = d1::vmadd(t, r3, d1::bc<3>(v));
        vu.set(8, v);
        vu.set(14, r0);
        vu.set(15, r1);
        vu.set(16, r2);
        vu.set(17, r3);
        vu.setAcc(t);
        vu.set(10, out);
        return out;
    }

    // The bound cylinder: x^2 + y^2 < r^2, -r < z < length + r. sumInF0: the form that leaves the sum in f0 when the
    // first compare fails (else y^2).
    template <bool SumInF0>
    PS2_FORCE_INLINE bool z1Cyl(Z1Vu &vu, __m128 c, float r, float length)
    {
        const float cx = d1::lane<0>(c), cy = d1::lane<1>(c), cz = d1::lane<2>(c);
        const float r2 = d1::mul(r, r);
        const float yy = d1::mul(cy, cy);
        const float sum = d1::add(d1::mul(cx, cx), yy);
        if (!d1::lt(sum, r2))
        {
            vu.setF0(SumInF0 ? sum : yy);
            return false;
        }
        const float nr = -r;
        if (!d1::lt(nr, cz))
        {
            vu.setF0(nr);
            return false;
        }
        const float top = d1::add(length, r);
        vu.setF0(top);
        return d1::lt(cz, top);
    }

    // The skinned surface's capsule test (r2 its boundary's r^2): x^2 + y^2 < r2, then z within [0, length] or its
    // distance past an end squared under r2.
    PS2_FORCE_INLINE bool z1Capsule(Z1Vu &vu, __m128 c, float r2, float length)
    {
        const float cx = d1::lane<0>(c), cy = d1::lane<1>(c), cz = d1::lane<2>(c);
        const float yy = d1::mul(cy, cy);
        const float sum = d1::add(d1::mul(cx, cx), yy);
        if (!d1::lt(sum, r2))
        {
            vu.setF0(yy);
            return false;
        }
        const float zero = 0.0f;
        if (!d1::le(zero, cz))
        {
            const float zz = d1::mul(cz, cz);
            vu.setF0(zz);
            if (!d1::lt(zz, r2))
                return false;
            if (d1::le(cz, length))
                return true;
            const float d = d1::sub(cz, length);
            const float dd = d1::mul(d, d);
            vu.setF0(dd);
            return d1::lt(dd, r2);
        }
        vu.setF0(zero);
        if (d1::le(cz, length))
            return true;
        const float d = d1::sub(cz, length);
        const float dd = d1::mul(d, d);
        vu.setF0(dd);
        return d1::lt(dd, r2);
    }

    // ---- ps2__CalcCollisionMultiDepSurfBoundary (0x1124368) ---------------------------------------------------------
    // a0 the boundary out (centre, r^2), a2 the blend manager, a3 the bone managers, t0 the matrices, t1 the lists, t2
    // the blend lists, t3 the corner pairs: the skinned triangle's three corners (01116108, round D3's core: the vertex
    // caches made, EntryModifiedManager, the corner pointers in the frame at sp - 0x20), their centre (the sum x 1 / 3),
    // the largest squared distance to it. False: the corner core's old-body case (nothing written).
    inline bool z1MultiDepCore(const G g, Z1Vu &vu, uint32_t out, uint32_t blend, uint32_t managers, uint32_t matrices, uint32_t lists,
                               uint32_t blendLists, uint32_t pairs, uint32_t spIn, uint32_t raIn)
    {
        R5900Context *ctx = g.ctx;
        const d1::Mem m{g.rdram};
        const uint32_t sp = spIn - 0x20u;
        const uint32_t args[8] = {sp, 0u, blend, managers, matrices, lists, blendLists, pairs};
        uint64_t saved[10];
        static constexpr int kSaved[10] = {16, 17, 18, 19, 20, 21, 22, 23, 30, 31};
        for (int i = 0; i < 10; ++i)
            saved[i] = GPR_U64(ctx, kSaved[i]);
        saved[0] = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(out))); // s0 = a0
        saved[9] = 0x01124384ull;                                                            // ra after the jal
        __m128 cvu[11];
        cvu[8] = vu.cur(ctx, 29);
        cvu[9] = vu.cur(ctx, 30);
        cvu[10] = vu.cur(ctx, 31);
        const int flags = decomp_skinfinder::cornersD3Core(m, args, sp, saved, cvu);
        if (flags < 0)
            return false;
        ++runsOf(g.runtime); // as the guest call would (the kept lookups' epoch)
        g.put64(sp + 0x10u, GPR_U64(ctx, 16)); // sd s0 / sd ra (the frame's saves)
        g.put64(sp + 0x18u, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(raIn))));
        if (flags & 1)
        {
            vu.set(8, cvu[0]);
            for (int k = 0; k < 4; ++k)
                vu.set(14 + k, cvu[1 + k]);
            vu.set(10, cvu[5]);
            vu.setAcc(cvu[6]);
        }
        if (flags & 2)
        {
            vu.set(29, cvu[8]);
            vu.set(30, cvu[9]);
            vu.set(31, cvu[10]);
        }
        const float third = 0.333333313465f; // 0x3eaaaaaa
        uint64_t hi8;
        std::memcpy(&hi8, reinterpret_cast<const uint8_t *>(&ctx->r[8]) + 8, 8);
        uint32_t v8b[4] = {0x3EAAAAAAu, 0u, static_cast<uint32_t>(hi8), static_cast<uint32_t>(hi8 >> 32)};
        __m128 vf8;
        std::memcpy(&vf8, v8b, 16);
        const __m128 p0 = g.q(g.u32(sp)), p1 = g.q(g.u32(sp + 4u)), p2 = g.q(g.u32(sp + 8u));
        const __m128 xyz = d1Xyz(), xm = d1X();
        __m128 vf7 = vu.cur(ctx, 7);
        vf7 = d1::vsel(vf7, d1::vadd(p0, p1), xyz);
        vf7 = d1::vsel(vf7, d1::vadd(vf7, p2), xyz);
        vf7 = d1::vsel(vf7, d1::vmul(vf7, d1::bc<0>(vf8)), xyz);
        auto dist = [&](__m128 p)
        {
            __m128 d = d1::vsel(p, d1::vsub(p, vf7), xyz);
            d = d1::vsel(d, d1::vmul(d, d), xyz);
            d = d1::vsel(d, d1::vadd(d, d1::bc<1>(d)), xm);
            return d1::vsel(d, d1::vadd(d, d1::bc<2>(d)), xm);
        };
        const __m128 vf4 = dist(p0), vf5 = dist(p1);
        vf8 = d1::vsel(vf8, _mm_max_ps(vf4, d1::bc<0>(vf5)), xm);
        const __m128 vf6 = dist(p2);
        vf8 = d1::vsel(vf8, _mm_max_ps(vf8, d1::bc<0>(vf6)), xm);
        vf7 = d1::vsel(vf7, d1::vmul(d1::vf0(), d1::bc<0>(vf8)), d1W());
        g.put(out, vf7);
        vu.set(4, vf4);
        vu.set(5, vf5);
        vu.set(6, vf6);
        vu.set(7, vf7);
        vu.set(8, vf8);
        vu.setF0(third);
        return true;
    }

    // MultiDep as a call (the adapter: ClipCollisionDirectCallBack's / ObjAry's D1 loop and the check): false (nothing
    // written) when the corner core declines or the slot 01116108 does not hold its D1 dispatcher.
    bool z1MultiDepCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!d1::on() || sotc_decomp::guestFunction(runtime, kVertices) != &decomp_skinfinder::cornersSw)
            return false;
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31), out = GPR_U32(ctx, 4);
        if (!G::plain(out, 16u) || sp < 0x2000u || !G::plain(sp - 0x100u, 0x100u))
            return false;
        const G g{rdram, ctx, runtime};
        Z1Vu vu;
        if (!z1MultiDepCore(g, vu, out, GPR_U32(ctx, 6), GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9), GPR_U32(ctx, 10), GPR_U32(ctx, 11), sp, ra))
            return false;
        vu.flush(ctx);
        const uint32_t frame = sp - 0x20u;
        // the old body's exit registers: a0 / v1 / v0 the three corner pointers (lw), a1 0, at / t0 the third's bits
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(g.u32(frame)));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(g.u32(frame + 4u)));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(g.u32(frame + 8u)));
        SET_GPR_U64(ctx, 5, 0u);
        SET_GPR_S32(ctx, 1, 0x3EAAAAAA);
        SET_GPR_S32(ctx, 8, 0x3EAAAAAA);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        returnTo(ctx);
        return true;
    }

    // ClipCollisionDirectCallBack's / ObjAry's D1 loop: a skinned surface's boundary (the guest call it made) through
    // Z1's adapter when it takes it.
    inline bool z1MultiDepFromLoop(const G g, uint32_t ra, uint32_t sp, std::initializer_list<uint32_t> args)
    {
        if (!::z1coll::on() || !::z1coll::partOn(::z1coll::kMultiDep))
            return false;
        int r = 4;
        for (uint32_t a : args)
            SET_GPR_U32(g.ctx, r++, a);
        SET_GPR_U32(g.ctx, 31, ra);
        SET_GPR_U32(g.ctx, 29, sp);
        g.ctx->pc = 0x01124368u;
        if (!::z1coll::enter(::z1coll::kMultiDep, &z1MultiDepCall, g.rdram, g.ctx, g.runtime))
            return false;
        SET_GPR_U32(g.ctx, 29, sp);
        return true;
    }

    // ---- ClipCollisionDirectCallBackOneObj (0x112ca30) --------------------------------------------------------------
    struct Z1One
    {
        G g;
        Z1Vu &vu;
        uint32_t sp;
        ::z1coll::Fn fn;
        // A guest call with a0-a3 / t0-t3 set (registers 4-11), the shadowed registers stored first.
        PS2_FORCE_INLINE void call(uint32_t target, uint32_t ra, std::initializer_list<uint32_t> args)
        {
            vu.flush(g.ctx);
            ::z1coll::countGuestCall(fn);
            decomp_collision::call(g, target, ra, sp, args);
        }
        PS2_FORCE_INLINE uint64_t v0() const { return GPR_U64(g.ctx, 2); }
        PS2_FORCE_INLINE uint32_t layoutClassOf(uint32_t object, uint32_t ra)
        {
            uint32_t cls;
            if (layoutClass(g, object, cls))
                return cls;
            call(kGetLayoutClass, ra, {object});
            return GPR_U32(g.ctx, 2);
        }
        // EntryModifiedManager(m, index): entryModified's host case, else the guest call.
        PS2_FORCE_INLINE void entry(uint32_t m, uint32_t index, uint32_t ra)
        {
            vu.flush(g.ctx);
            entryModified(g, m, index, ra, sp);
        }
    };

    bool z1OneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x260u;
        if (sp < 0x2000u || !G::plain(sp - 0x200u, 0x460u) || sotc_decomp::onWorkerLane(runtime))
            return false;
        const uint32_t s5 = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), object = GPR_U32(ctx, 7);
        const uint32_t partFilter = GPR_U32(ctx, 8), surfFilter = GPR_U32(ctx, 9), idFilt = GPR_U32(ctx, 10), user = GPR_U32(ctx, 11);
        const bool skinCore = d1::on() && sotc_decomp::guestFunction(runtime, kVertices) == &decomp_skinfinder::cornersSw &&
                              ::z1coll::partOn(::z1coll::kMultiDep);
        const G g{rdram, ctx, runtime};
        Z1Vu vu;
        Z1One o{g, vu, sp, ::z1coll::kOneObj};
        auto finish = [&](uint64_t v0)
        {
            vu.flush(ctx);
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, spIn);
            SET_GPR_U64(ctx, 2, v0);
            returnTo(ctx);
            return true;
        };
        o.call(0x01133548u, 0x0112CAA0u, {sp + 0x40u, sp, start, end}); // GetArbitraryDirectionInverseMatrix
        const float length = ctx->f[0];
        if (d1::lt(length, 0.0f))
            return finish(0u);
        const uint32_t cls = o.layoutClassOf(object, 0x0112CABCu);
        if (static_cast<int8_t>(g.u8(cls + 0x1B8u)) == 0 || static_cast<int8_t>(g.u8(cls + 0x1BBu)) == 0)
            return finish(0u);
        const uint32_t db = g.u32(g.u32(cls + 0x184u) + 0x20u);
        const float scale = g.f32(cls + 0x90u);
        {
            const float radius = g.f32(db + 0x2Cu);
            vu.setF0(radius);
            if (!d1::eq(radius, 0.0f))
            {
                const float r = d1::mul(radius, scale);
                const __m128 c = z1Xf(g, vu, sp + 0x40u, g.q(g.u32(cls + 0x134u)));
                if (!z1Cyl<false>(vu, c, r, length))
                    return finish(0u);
            }
        }
        __m128 endCur = g.q(end); // the original's sp + 0x90
        bool objHit = false;
        const uint32_t base = g.u32(cls + 0x174u);
        const uint32_t vtxTable = g.u32(base + 0x24u), blend = base + 0x28u; // +0x1b0 / +0x1b4
        uint32_t s7 = g.u32(db + 8u);
        uint32_t m0 = g.u32(cls + 0x158u), m1 = g.u32(cls + 0x15Cu), m2 = g.u32(cls + 0x164u);
        for (int32_t part = 0; part < static_cast<int32_t>(g.u32(db + 4u)); ++part, s7 += 0x90u, m0 += 0x40u, m1 += 0x40u, m2 += 0x40u)
        {
            if (g.u32(s7 + 4u) == 0u)
                continue;
            bool partHit = false;
            if (g.u32(s7 + 0x10u) != 0u)
            {
                const float r = d1::mul(g.f32(s7 + 0x1Cu), scale);
                const __m128 wc = z1Xf(g, vu, m1, g.q(s7 + 0x20u));
                g.put(sp + 0xA0u, wc);
                const __m128 c = z1Xf(g, vu, sp + 0x40u, wc);
                bool go = z1Cyl<false>(vu, c, r, length);
                if (go && partFilter != 0u)
                {
                    ctx->f[12] = r;
                    o.call(partFilter, 0x0112CCD0u, {sp + 0xA0u, sp, sp + 0x40u, user});
                    go = o.v0() != 0u;
                }
                if (go)
                {
                    const uint32_t tree = g.u32(db + 0x1Cu);
                    if (tree != 0u)
                    {
                        const uint32_t cls2 = o.layoutClassOf(object, 0x0112CCF0u);
                        g.put32(sp + 0xC0u, object);
                        g.put32(sp + 0xC4u, cls2);
                        putf(g, sp + 0xC8u, length);
                        g.put32(sp + 0xCCu, m0);
                        g.put32(sp + 0xD0u, sp + 0x40u);
                        putf(g, sp + 0xD4u, scale);
                        g.put32(sp + 0xD8u, 0u);
                        g.put32(sp + 0xDCu, surfFilter);
                        g.put32(sp + 0xE0u, idFilt);
                        g.put32(sp + 0xE4u, user);
                        const uint32_t s3 = sp + 0x120u, s2 = sp + 0x130u;
                        const __m128 v3 = z1Xf(g, vu, m2, g.q(start));
                        g.put(s3, v3);
                        const __m128 v2 = z1Xf(g, vu, m2, endCur);
                        g.put(s2, v2);
                        g.put(sp + 0xF0u, v3);
                        const __m128 dv = d1::vsub(v2, v3); // vsub vf7, vf5, vf6
                        vu.set(7, dv);
                        g.put(sp + 0x110u, dv);
                        // iosAbsVector(sp + 0x100, sp + 0x110): its host case (decomp_quat::absVector), else the call
                        const uint32_t absTarget = sotc_decomp::jalTarget(rdram, 0x0112CDBCu);
                        if (absTarget == 0x001BE898u && sotc_decomp::camCollOn() && sotc_decomp::guestFunction(runtime, 0x001BE898u) == &decomp_quat::absVector)
                        {
                            const __m128 av = _mm_and_ps(dv, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
                            g.put(sp + 0x100u, av);
                            vu.set(5, dv);
                            vu.set(6, av);
                        }
                        else
                        {
                            vu.set(5, v2);
                            vu.set(6, v3);
                            o.call(absTarget, 0x0112CDC4u, {sp + 0x100u, sp + 0x110u});
                        }
                        uint64_t hit;
                        if (g.u32(tree + 0x10u) != 0u)
                        {
                            o.call(0x0112ACD0u, 0x0112CDF4u, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 16u, sp + 0xC0u});
                            hit = o.v0();
                        }
                        else if (static_cast<int32_t>(g.u32(g.u32(g.u32(cls2 + 0x184u) + 0x20u))) < 3)
                        {
                            o.call(0x0112B890u, 0x0112CF1Cu, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 8u, sp + 0xC0u});
                            hit = o.v0();
                        }
                        else
                        {
                            const uint32_t root = g.u32(tree + 0xCu) * 8u + g.u32(tree + 8u);
                            const uint32_t axis = (g.u32(root + 4u) & 3u) * 4u;
                            const float a1v = g.f32(s3 + axis), a0v = g.f32(s2 + axis);
                            vu.setF0(a0v);
                            if (d1::le(a0v, a1v))
                            {
                                o.call(kHigh, 0x0112CE70u, {s5, s3, s2, tree, root, sp + 0xC0u});
                                hit = o.v0() != 0u;
                                if (!hit)
                                {
                                    o.call(kLow, 0x0112CE94u, {s5, s3, s2, tree, root, sp + 0xC0u});
                                    hit = o.v0() != 0u;
                                }
                            }
                            else
                            {
                                o.call(kLow, 0x0112CEC0u, {s5, s3, s2, tree, root, sp + 0xC0u});
                                hit = o.v0() != 0u;
                                if (!hit)
                                {
                                    o.call(kHigh, 0x0112CEE4u, {s5, s3, s2, tree, root, sp + 0xC0u});
                                    hit = o.v0() != 0u;
                                }
                            }
                        }
                        partHit = hit != 0u;
                    }
                    else
                    {
                        // one surface at a time
                        const uint32_t s3 = o.layoutClassOf(object, 0x0112CF3Cu);
                        g.put(sp + 0xC0u, endCur);
                        bool hit = false;
                        const float one = 1.0f;
                        for (int32_t surface = 0; surface < static_cast<int32_t>(g.u32(s7 + 0x10u)); ++surface)
                        {
                            vu.flush(ctx);
                            const uint32_t b = boundary(g, s3, static_cast<uint32_t>(part), static_cast<uint32_t>(surface), 0x0112CF6Cu, sp);
                            __m128 bq = g.q(b);
                            const float rb = g.f32(b + 0xCu);
                            bq = d1::vsel(bq, d1::splat(one), d1W());
                            g.put(sp + 0xD0u, bq);
                            const __m128 cb = z1Xf(g, vu, sp + 0x40u, bq);
                            g.put(sp + 0xE0u, cb);
                            if (!z1Cyl<true>(vu, cb, rb, length))
                                continue;
                            if (surfFilter != 0u)
                            {
                                o.call(surfFilter, 0x0112D018u, {sp + 0xD0u, sp + 0x40u, user});
                                if (o.v0() == 0u)
                                    continue;
                            }
                            g.put32(sp + 0xF4u, (static_cast<uint32_t>(part) << 24) | static_cast<uint32_t>(surface) | 0x80000000u);
                            g.put32(sp + 0xF0u, object);
                            if (idFilt != 0u)
                            {
                                o.call(idFilt, 0x0112D058u, {sp + 0xF0u, user});
                                if (o.v0() == 0u)
                                    continue;
                            }
                            o.call(kClipDB, 0x0112D074u, {s5, start, sp + 0xC0u, sp + 0xF0u});
                            if (o.v0() != 0u)
                            {
                                hit = true;
                                const uint32_t lo = g.u32(sp + 0xF0u), hi = g.u32(sp + 0xF4u);
                                const __m128 point = g.q(s5 + 0x40u);
                                g.put32(s5, lo);
                                g.put32(s5 + 4u, hi);
                                g.put(sp + 0xC0u, point);
                            }
                        }
                        partHit = hit;
                    }
                }
            }
            const uint32_t nSkin = g.u32(s7 + 0x50u);
            if (nSkin != 0u)
            {
                const float r = d1::mul(g.f32(s7 + 0x5Cu), scale);
                const __m128 gc = z1Xf(g, vu, m1, g.q(s7 + 0x60u));
                g.put(sp + 0xB0u, gc);
                const __m128 c = z1Xf(g, vu, sp + 0x40u, gc);
                bool go = z1Cyl<false>(vu, c, r, length);
                if (go && partFilter != 0u)
                {
                    ctx->f[12] = r;
                    o.call(partFilter, 0x0112D18Cu, {sp + 0xB0u, sp, sp + 0x40u, user});
                    go = o.v0() != 0u;
                }
                if (go)
                {
                    // the part-to-segment matrix (vf14-vf17 the inverse, vf24-vf27 m1's rows through it)
                    const __m128 i0 = g.q(sp + 0x40u), i1 = g.q(sp + 0x50u), i2 = g.q(sp + 0x60u), i3 = g.q(sp + 0x70u);
                    __m128 pm[4], acc = d1::zero4();
                    for (int k = 0; k < 4; ++k)
                    {
                        const __m128 row = g.q(m1 + static_cast<uint32_t>(k) * 0x10u);
                        __m128 t = d1::vmul(i0, d1::bc<0>(row));
                        t = d1::vmadd(t, i1, d1::bc<1>(row));
                        t = d1::vmadd(t, i2, d1::bc<2>(row));
                        pm[k] = d1::vmadd(t, i3, d1::bc<3>(row));
                        acc = t;
                    }
                    for (int k = 0; k < 4; ++k)
                    {
                        g.put(sp + 0xD0u + static_cast<uint32_t>(k) * 0x10u, pm[k]);
                        vu.set(24 + k, pm[k]);
                    }
                    vu.set(14, i0);
                    vu.set(15, i1);
                    vu.set(16, i2);
                    vu.set(17, i3);
                    vu.setAcc(acc);
                    const uint32_t s3 = blend + 0x48u;
                    const uint32_t matrices = g.u32(cls + 0x15Cu), lists = g.u32(db + 8u), blendLists = g.u32(db + 0x14u);
                    __m128 endSkin = endCur; // the original's sp + 0x140
                    bool skinHit = false;
                    uint32_t s2 = g.u32(s7 + 0x58u), s4 = g.u32(s7 + 0x54u);
                    const float one = 1.0f;
                    for (int32_t k = 0; k < static_cast<int32_t>(nSkin); ++k, s2 += 0x10u, s4 += 4u)
                    {
                        const uint32_t s0 = g.u32(s4);
                        const uint32_t t3 = g.u32(db + 0x18u) + s0 * 64u;
                        const uint32_t s1 = g.u32(s3 + 0x14u) + s0 * 16u;
                        if (g.u8(g.u32(s3 + 8u) + s0) == 0u)
                        {
                            const __m128 sb = d1::vsel(g.q(s2), d1::splat(one), d1W());
                            const float rs = d1::mul(g.f32(s2 + 0xCu), scale);
                            const __m128 cs = z1Xf(g, vu, sp + 0xD0u, sb);
                            if (!z1Cyl<true>(vu, cs, rs, length))
                                continue;
                            if (!(skinCore && G::plain(s1, 16u) &&
                                  z1MultiDepCore(g, vu, s1, blend, vtxTable, matrices, lists, blendLists, t3, sp, 0x0112D350u)))
                            {
                                g.put(sp + 0x150u, sb);
                                g.put(sp + 0x160u, cs);
                                o.call(0x01124368u, 0x0112D350u, {s1, s3, blend, vtxTable, matrices, lists, blendLists, t3});
                            }
                            else
                                ::z1coll::countInline(::z1coll::kMultiDep);
                            o.entry(s3, s0, 0x0112D35Cu);
                        }
                        const __m128 bq = d1::vsel(g.q(s1), d1::splat(one), d1W());
                        const float r2 = g.f32(s1 + 0xCu);
                        const __m128 cs = z1Xf(g, vu, sp + 0x40u, bq);
                        if (!z1Capsule(vu, cs, r2, length))
                            continue;
                        if (surfFilter != 0u)
                        {
                            o.call(surfFilter, 0x0112D41Cu, {s1, 0u, user});
                            if (o.v0() == 0u)
                                continue;
                        }
                        g.put32(sp + 0x180u, object);
                        g.put32(sp + 0x184u, (static_cast<uint32_t>(part) << 24) | static_cast<uint32_t>(k));
                        if (idFilt != 0u)
                        {
                            o.call(idFilt, 0x0112D454u, {sp + 0x180u, user});
                            if (o.v0() == 0u)
                                continue;
                        }
                        g.put(sp + 0x140u, endSkin);
                        o.call(kClipDB, 0x0112D470u, {s5, start, sp + 0x140u, sp + 0x180u});
                        if (o.v0() != 0u)
                        {
                            skinHit = true;
                            const uint32_t lo = g.u32(sp + 0x180u), hi = g.u32(sp + 0x184u);
                            endSkin = g.q(s5 + 0x40u);
                            g.put32(s5, lo);
                            g.put32(s5 + 4u, hi);
                        }
                    }
                    if (skinHit)
                        partHit = true;
                }
            }
            if (partHit)
            {
                objHit = true;
                endCur = g.q(s5 + 0x40u);
            }
        }
        if (objHit)
        {
            g.put(s5 + 0x40u, endCur);
            return finish(1u);
        }
        return finish(0u);
    }

    // ---- ClipCollisionDirect (0x112ed58) ----------------------------------------------------------------------------
    // ClipCollisionDirect(a0 result, a1 start, a2 end, a3 isys group): OneObj's tests without filters for every object of
    // the group (isysGroupForFirst / Next / Exit), the segment's end moved to each object's last hit; v0 1 when anything
    // was hit. Its frame (sp = entry sp - 0x260), only the words the callees read: +0 / +0x40 the segment's matrices,
    // +0xd0..+0xf4 the walk's record (object, class, length, the class's +0x158 matrices, sp + 0x40, scale, then 0 for
    // the side mode, the two filters and the user word), +0x100 / +0x110 / +0x120 / +0x130 / +0x140 the walk's segment,
    // +0xd0 / +0x100 the one-by-one loop's end / id record, +0xe0..+0x11c the part-to-segment matrix, +0x150 the skinned
    // loop's end, +0x190 its id record. The differences from OneObj are the original's: f0 0 at the bound's radius test,
    // the skinned group's cylinder with the sum left in f0, each skinned surface's with y^2.
    bool z1Direct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x260u;
        if (sp < 0x2000u || !G::plain(sp - 0x200u, 0x460u) || sotc_decomp::onWorkerLane(runtime))
            return false;
        // the boot ELF's sites (relocated jal words, as the generated code reads them)
        const uint32_t kFirst = sotc_decomp::jalTarget(rdram, 0x0112EDD8u), kNext = sotc_decomp::jalTarget(rdram, 0x0112F7E4u),
                       kExit = sotc_decomp::jalTarget(rdram, 0x0112F808u), absTarget = sotc_decomp::jalTarget(rdram, 0x0112F0DCu);
        const uint32_t s5 = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), group = GPR_U32(ctx, 7);
        const bool skinCore = d1::on() && sotc_decomp::guestFunction(runtime, kVertices) == &decomp_skinfinder::cornersSw &&
                              ::z1coll::partOn(::z1coll::kMultiDep);
        const bool absHost = absTarget == 0x001BE898u && sotc_decomp::camCollOn() && sotc_decomp::guestFunction(runtime, 0x001BE898u) == &decomp_quat::absVector;
        const bool nextHost = kNext == 0x001C76D0u;
        const G g{rdram, ctx, runtime};
        Z1Vu vu;
        Z1One o{g, vu, sp, ::z1coll::kDirect};
        auto finish = [&](uint64_t v0)
        {
            vu.flush(ctx);
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, spIn);
            SET_GPR_U64(ctx, 2, v0);
            returnTo(ctx);
            return true;
        };
        o.call(0x01133548u, 0x0112EDB4u, {sp + 0x40u, sp, start, end}); // GetArbitraryDirectionInverseMatrix
        const float length = ctx->f[0];
        const float zero = 0.0f, one = 1.0f;
        vu.setF0(zero);
        if (d1::lt(length, zero))
            return finish(0u);
        __m128 endObj = g.q(end); // the original's sp + 0x80
        bool anyHit = false;
        o.call(kFirst, 0x0112EDE0u, {group, 0u});
        uint32_t entry = GPR_U32(ctx, 2), last = 0u;
        while (entry != 0u)
        {
            const uint32_t object = g.u32(entry);
            last = object;
            bool objHit = false;
            const uint32_t cls = o.layoutClassOf(object, 0x0112EE04u);
            if (static_cast<int8_t>(g.u8(cls + 0x1B8u)) != 0 && static_cast<int8_t>(g.u8(cls + 0x1BBu)) != 0)
            {
                const uint32_t db = g.u32(g.u32(cls + 0x184u) + 0x20u);
                const float radius = g.f32(db + 0x2Cu), scale = g.f32(cls + 0x90u);
                vu.setF0(zero);
                bool near = true;
                if (!d1::eq(radius, zero))
                {
                    const float r = d1::mul(radius, scale);
                    near = z1Cyl<false>(vu, z1Xf(g, vu, sp + 0x40u, g.q(g.u32(cls + 0x134u))), r, length);
                }
                if (near)
                {
                    __m128 endCur = endObj; // sp + 0xa0
                    const uint32_t base = g.u32(cls + 0x174u);
                    const uint32_t vtxTable = g.u32(base + 0x24u), blend = base + 0x28u; // +0x1b8 / +0x1bc
                    uint32_t s6 = g.u32(db + 8u);
                    uint32_t m0 = g.u32(cls + 0x158u), m1 = g.u32(cls + 0x15Cu), m2 = g.u32(cls + 0x164u);
                    for (int32_t part = 0; part < static_cast<int32_t>(g.u32(db + 4u)); ++part, s6 += 0x90u, m0 += 0x40u, m1 += 0x40u, m2 += 0x40u)
                    {
                        if (g.u32(s6 + 4u) == 0u)
                            continue;
                        bool partHit = false;
                        if (g.u32(s6 + 0x10u) != 0u)
                        {
                            const float r = d1::mul(g.f32(s6 + 0x1Cu), scale);
                            const __m128 wc = z1Xf(g, vu, m1, g.q(s6 + 0x20u));
                            if (z1Cyl<false>(vu, z1Xf(g, vu, sp + 0x40u, wc), r, length))
                            {
                                const uint32_t tree = g.u32(db + 0x1Cu);
                                if (tree != 0u)
                                {
                                    const uint32_t cls2 = o.layoutClassOf(object, 0x0112F014u);
                                    g.put32(sp + 0xD0u, object);
                                    g.put32(sp + 0xD4u, cls2);
                                    putf(g, sp + 0xD8u, length);
                                    g.put32(sp + 0xDCu, m0);
                                    g.put32(sp + 0xE0u, sp + 0x40u);
                                    putf(g, sp + 0xE4u, scale);
                                    g.put32(sp + 0xE8u, 0u);
                                    g.put32(sp + 0xECu, 0u);
                                    g.put32(sp + 0xF0u, 0u);
                                    g.put32(sp + 0xF4u, 0u);
                                    const uint32_t s3 = sp + 0x130u, s2 = sp + 0x140u;
                                    const __m128 v3 = z1Xf(g, vu, m2, g.q(start));
                                    g.put(s3, v3);
                                    const __m128 v2 = z1Xf(g, vu, m2, endCur);
                                    g.put(s2, v2);
                                    g.put(sp + 0x100u, v3);
                                    const __m128 dv = d1::vsub(v2, v3);
                                    vu.set(7, dv);
                                    g.put(sp + 0x120u, dv);
                                    if (absHost)
                                    {
                                        const __m128 av = _mm_and_ps(dv, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
                                        g.put(sp + 0x110u, av);
                                        vu.set(5, dv);
                                        vu.set(6, av);
                                    }
                                    else
                                    {
                                        vu.set(5, v2);
                                        vu.set(6, v3);
                                        o.call(absTarget, 0x0112F0E4u, {sp + 0x110u, sp + 0x120u});
                                    }
                                    uint64_t hit;
                                    if (g.u32(tree + 0x10u) != 0u)
                                    {
                                        o.call(0x0112ACD0u, 0x0112F114u, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 16u, sp + 0xD0u});
                                        hit = o.v0();
                                    }
                                    else if (static_cast<int32_t>(g.u32(g.u32(g.u32(cls2 + 0x184u) + 0x20u))) < 3)
                                    {
                                        o.call(0x0112B890u, 0x0112F23Cu, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 8u, sp + 0xD0u});
                                        hit = o.v0();
                                    }
                                    else
                                    {
                                        const uint32_t root = g.u32(tree + 0xCu) * 8u + g.u32(tree + 8u);
                                        const uint32_t axis = (g.u32(root + 4u) & 3u) * 4u;
                                        const float a1v = g.f32(s3 + axis), a0v = g.f32(s2 + axis);
                                        vu.setF0(a0v);
                                        if (d1::le(a0v, a1v))
                                        {
                                            o.call(kHigh, 0x0112F190u, {s5, s3, s2, tree, root, sp + 0xD0u});
                                            hit = o.v0() != 0u;
                                            if (!hit)
                                            {
                                                o.call(kLow, 0x0112F1B4u, {s5, s3, s2, tree, root, sp + 0xD0u});
                                                hit = o.v0() != 0u;
                                            }
                                        }
                                        else
                                        {
                                            o.call(kLow, 0x0112F1E0u, {s5, s3, s2, tree, root, sp + 0xD0u});
                                            hit = o.v0() != 0u;
                                            if (!hit)
                                            {
                                                o.call(kHigh, 0x0112F204u, {s5, s3, s2, tree, root, sp + 0xD0u});
                                                hit = o.v0() != 0u;
                                            }
                                        }
                                    }
                                    partHit = hit != 0u;
                                }
                                else
                                {
                                    const uint32_t s7 = o.layoutClassOf(object, 0x0112F25Cu);
                                    g.put(sp + 0xD0u, endCur);
                                    bool hit = false;
                                    for (int32_t surface = 0; surface < static_cast<int32_t>(g.u32(s6 + 0x10u)); ++surface)
                                    {
                                        vu.flush(ctx);
                                        const uint32_t b = boundary(g, s7, static_cast<uint32_t>(part), static_cast<uint32_t>(surface), 0x0112F2A4u, sp);
                                        const __m128 bq = d1::vsel(g.q(b), d1::splat(one), d1W());
                                        const float rb = g.f32(b + 0xCu);
                                        g.put(sp + 0xE0u, bq);
                                        if (!z1Cyl<true>(vu, z1Xf(g, vu, sp + 0x40u, bq), rb, length))
                                            continue;
                                        g.put32(sp + 0x104u, (static_cast<uint32_t>(part) << 24) | static_cast<uint32_t>(surface) | 0x80000000u);
                                        g.put32(sp + 0x100u, object);
                                        o.call(kClipDB, 0x0112F35Cu, {s5, start, sp + 0xD0u, sp + 0x100u});
                                        if (o.v0() != 0u)
                                        {
                                            hit = true;
                                            const uint32_t lo = g.u32(sp + 0x100u), hi = g.u32(sp + 0x104u);
                                            const __m128 point = g.q(s5 + 0x40u);
                                            g.put32(s5, lo);
                                            g.put32(s5 + 4u, hi);
                                            g.put(sp + 0xD0u, point);
                                        }
                                    }
                                    partHit = hit;
                                }
                            }
                        }
                        const uint32_t nSkin = g.u32(s6 + 0x50u);
                        if (nSkin != 0u)
                        {
                            const float r = d1::mul(g.f32(s6 + 0x5Cu), scale);
                            const __m128 gc = z1Xf(g, vu, m1, g.q(s6 + 0x60u));
                            if (z1Cyl<true>(vu, z1Xf(g, vu, sp + 0x40u, gc), r, length))
                            {
                                const __m128 i0 = g.q(sp + 0x40u), i1 = g.q(sp + 0x50u), i2 = g.q(sp + 0x60u), i3 = g.q(sp + 0x70u);
                                __m128 pm[4], acc = d1::zero4();
                                for (int k = 0; k < 4; ++k)
                                {
                                    const __m128 row = g.q(m1 + static_cast<uint32_t>(k) * 0x10u);
                                    __m128 t = d1::vmul(i0, d1::bc<0>(row));
                                    t = d1::vmadd(t, i1, d1::bc<1>(row));
                                    t = d1::vmadd(t, i2, d1::bc<2>(row));
                                    pm[k] = d1::vmadd(t, i3, d1::bc<3>(row));
                                    acc = t;
                                }
                                for (int k = 0; k < 4; ++k)
                                {
                                    g.put(sp + 0xE0u + static_cast<uint32_t>(k) * 0x10u, pm[k]);
                                    vu.set(24 + k, pm[k]);
                                }
                                vu.set(14, i0);
                                vu.set(15, i1);
                                vu.set(16, i2);
                                vu.set(17, i3);
                                vu.setAcc(acc);
                                const uint32_t s3 = blend + 0x48u;
                                const uint32_t matrices = g.u32(cls + 0x15Cu), lists = g.u32(db + 8u), blendLists = g.u32(db + 0x14u);
                                __m128 endSkin = endCur; // sp + 0x150
                                bool skinHit = false;
                                uint32_t s2 = g.u32(s6 + 0x58u), s4 = g.u32(s6 + 0x54u);
                                for (int32_t k = 0; k < static_cast<int32_t>(nSkin); ++k, s2 += 0x10u, s4 += 4u)
                                {
                                    const uint32_t s0 = g.u32(s4);
                                    const uint32_t t3 = g.u32(db + 0x18u) + s0 * 64u;
                                    const uint32_t s1 = g.u32(s3 + 0x14u) + s0 * 16u;
                                    if (g.u8(g.u32(s3 + 8u) + s0) == 0u)
                                    {
                                        const __m128 sb = d1::vsel(g.q(s2), d1::splat(one), d1W());
                                        const float rs = d1::mul(g.f32(s2 + 0xCu), scale);
                                        if (!z1Cyl<false>(vu, z1Xf(g, vu, sp + 0xE0u, sb), rs, length))
                                            continue;
                                        if (!(skinCore && G::plain(s1, 16u) &&
                                              z1MultiDepCore(g, vu, s1, blend, vtxTable, matrices, lists, blendLists, t3, sp, 0x0112F610u)))
                                        {
                                            g.put(sp + 0x160u, sb);
                                            o.call(0x01124368u, 0x0112F610u, {s1, s3, blend, vtxTable, matrices, lists, blendLists, t3});
                                        }
                                        else
                                            ::z1coll::countInline(::z1coll::kMultiDep);
                                        o.entry(s3, s0, 0x0112F61Cu);
                                    }
                                    const __m128 bq = d1::vsel(g.q(s1), d1::splat(one), d1W());
                                    const float r2 = g.f32(s1 + 0xCu);
                                    if (!z1Capsule(vu, z1Xf(g, vu, sp + 0x40u, bq), r2, length))
                                        continue;
                                    g.put32(sp + 0x194u, (static_cast<uint32_t>(part) << 24) | static_cast<uint32_t>(k));
                                    g.put32(sp + 0x190u, object);
                                    g.put(sp + 0x150u, endSkin);
                                    o.call(kClipDB, 0x0112F6F0u, {s5, start, sp + 0x150u, sp + 0x190u});
                                    if (o.v0() != 0u)
                                    {
                                        skinHit = true;
                                        const uint32_t lo = g.u32(sp + 0x190u), hi = g.u32(sp + 0x194u);
                                        endSkin = g.q(s5 + 0x40u);
                                        g.put32(s5, lo);
                                        g.put32(s5 + 4u, hi);
                                    }
                                }
                                if (skinHit)
                                    partHit = true;
                            }
                        }
                        if (partHit)
                        {
                            objHit = true;
                            endCur = g.q(s5 + 0x40u);
                        }
                    }
                    if (objHit)
                        g.put(s5 + 0x40u, endCur);
                }
            }
            if (objHit)
            {
                anyHit = true;
                endObj = g.q(s5 + 0x40u);
            }
            uint32_t next;
            vu.flush(ctx);
            if (nextHost && groupNext(g, entry, next))
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(next));
            else
            {
                o.call(kNext, 0x0112F7ECu, {entry, 0u});
                next = GPR_U32(ctx, 2);
            }
            entry = next;
        }
        if (last != 0u)
            o.call(kExit, 0x0112F810u, {last, 0u});
        return finish(anyHit ? 1u : 0u);
    }
}

namespace z1coll
{
    bool oneObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return decomp_collision::z1OneObj(rdram, ctx, runtime); }
    bool multiDep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return decomp_collision::z1MultiDepCall(rdram, ctx, runtime); }
    bool direct(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return decomp_collision::z1Direct(rdram, ctx, runtime); }
}
