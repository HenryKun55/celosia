// Shadow of the Colossus: native version of the VU1 world-geometry loop.
//
// Microcode image f40e22b1ef541d18, program 0x05b0 (half of all VU1 work):
// 0x05b0 BAL vi14, 0x1140 runs a software-pipelined loop over a vertex batch
// (position, 12.4 texture coordinate, colour per vertex) that writes
// GIF-ready vertices [STQ, RGBA, XYZF2], marks triangles that are off screen
// (ADC) and counts those that cross the guard band. This code does what the
// microcode does from 0x05b0 up to 0x13b8, pair by pair with VU arithmetic
// (operands and results normalised, round toward zero: the caller has set
// it), and the microcode continues from 0x13b8: it stores the counters, runs
// its clipper when needed, XGKICKs and returns.
//
// The microcode relies on the VU interlocks, so in program order each read
// sees the latest write; the two lanes of a pair read before either writes.
// PS2X_VU1_NATIVE=verify compares this with the interpreter.

#include "runtime/ps2_stats.h"
#include "ps2_runtime.h"
#include "runtime/ps2_vu1.h"
#include "runtime/ps2_vu1_native.h"
#include "runtime/gs/gs_frontend.h"

#include <atomic>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

namespace
{
    constexpr uint64_t kWorldImage = 0xf40e22b1ef541d18ull;
    constexpr uint32_t kWorldResume = 0x13b8; // loop 0x1140 done
    constexpr uint32_t kLitResume = 0x1ad0;   // loop 0x1818 done

    __attribute__((always_inline)) inline float norm(float value)
    {
#if defined(PS2X_VU1_TRUST_FPU)
        // The VU1 threads run with round-toward-zero and flush-to-zero
        // (ps2SetEeFloatingPointMode): the FPU itself gives denormals as
        // signed zero and overflow as +-FLT_MAX. Only Inf/NaN inputs, absent
        // from game data, would differ (PS2X_VU1_NATIVE=verify checks).
        return value;
#endif
        uint32_t bits;
        std::memcpy(&bits, &value, sizeof(bits));
        const uint32_t exponent = bits & 0x7F800000u;
        if (exponent == 0u)
            bits &= 0x80000000u;
        else if (exponent == 0x7F800000u)
            bits = (bits & 0x80000000u) | 0x7F7FFFFFu;
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    }

    inline int32_t ftoi(float value, float scale)
    {
        const double scaled = static_cast<double>(value) * static_cast<double>(scale);
        if (scaled >= static_cast<double>(std::numeric_limits<int32_t>::max()))
            return std::numeric_limits<int32_t>::max();
        if (scaled <= static_cast<double>(std::numeric_limits<int32_t>::min()))
            return std::numeric_limits<int32_t>::min();
        return static_cast<int32_t>(scaled);
    }

    struct Machine
    {
        VU1State &S;
        uint8_t *data;
        uint32_t mask; // data size - 1

        float *vf(int r) { return S.vf[r]; }

        uint8_t *qword(int32_t index) { return data + ((static_cast<uint32_t>(index) * 16u) & mask); }
        void lq(float *dst, int32_t index) { std::memcpy(dst, qword(index), 16); }
        void sq(const float *src, int32_t index) { std::memcpy(qword(index), src, 16); }
        // Lanes in `lanes` (bit 0 = x ... bit 3 = w).
        void lqLanes(float *dst, int32_t index, unsigned lanes)
        {
            const uint8_t *q = qword(index);
            for (int c = 0; c < 4; ++c)
                if (lanes & (1u << c))
                    std::memcpy(&dst[c], q + c * 4, 4);
        }
        void sqLanes(const float *src, int32_t index, unsigned lanes)
        {
            uint8_t *q = qword(index);
            for (int c = 0; c < 4; ++c)
                if (lanes & (1u << c))
                    std::memcpy(q + c * 4, &src[c], 4);
        }
        int32_t ilw(int32_t index, int lane)
        {
            uint32_t v;
            std::memcpy(&v, qword(index) + lane * 4, 4);
            return static_cast<int16_t>(v & 0xFFFFu);
        }
        void isw(int32_t index, int lane, int32_t value)
        {
            const uint32_t v = static_cast<uint16_t>(value & 0xFFFF);
            std::memcpy(qword(index) + lane * 4, &v, 4);
        }

        // ACC = vs * bc (all lanes)
        void mulaBc(int vs, float bc)
        {
            const float b = norm(bc);
            for (int c = 0; c < 4; ++c)
            {
                const float s = norm(S.vf[vs][c]);
                S.acc[c] = norm(s * b);
            }
        }
        // ACC += vs * bc
        void maddaBc(int vs, float bc)
        {
            const float b = norm(bc);
            for (int c = 0; c < 4; ++c)
            {
                const float a = norm(S.acc[c]);
                const float s = norm(S.vf[vs][c]);
                S.acc[c] = norm(a + s * b);
            }
        }
        // vd = ACC + vs * bc
        void maddBc(int vd, int vs, float bc, float out[4])
        {
            const float b = norm(bc);
            for (int c = 0; c < 4; ++c)
            {
                const float a = norm(S.acc[c]);
                const float s = norm(S.vf[vs][c]);
                out[c] = norm(a + s * b);
            }
            (void)vd;
        }
        void itof(float out[4], const float *in, float div)
        {
            for (int c = 0; c < 4; ++c)
            {
                int32_t iv;
                std::memcpy(&iv, &in[c], 4);
                out[c] = div == 1.0f ? static_cast<float>(iv) : static_cast<float>(iv) / div;
            }
        }
        void div(float den)
        {
            const float num = norm(S.vf[0][3]);
            const float d = norm(den);
            float result;
            if (d == 0.0f)
                result = std::signbit(num) != std::signbit(d) ? -FLT_MAX : FLT_MAX;
            else
                result = num / d;
            S.q = norm(result);
        }
        void clip(const float *v)
        {
            uint32_t wBits;
            std::memcpy(&wBits, &v[3], 4);
            const int32_t limit = (wBits & 0x7F800000u) != 0u ? static_cast<int32_t>(wBits & 0x7FFFFFFFu) : 0x007FFFFF;
            auto exceeds = [limit](float value, uint32_t signMask)
            {
                uint32_t bits;
                std::memcpy(&bits, &value, 4);
                bits ^= signMask;
                int32_t ordered;
                std::memcpy(&ordered, &bits, 4);
                return ordered > limit;
            };
            uint32_t flags = 0;
            if (exceeds(v[0], 0u)) flags |= 0x01u;
            if (exceeds(v[0], 0x80000000u)) flags |= 0x02u;
            if (exceeds(v[1], 0u)) flags |= 0x04u;
            if (exceeds(v[1], 0x80000000u)) flags |= 0x08u;
            if (exceeds(v[2], 0u)) flags |= 0x10u;
            if (exceeds(v[2], 0x80000000u)) flags |= 0x20u;
            S.clip = ((S.clip << 6) | flags) & 0xFFFFFFu;
        }
    };

    template <uint32_t Entry>
    uint32_t worldLoopCpu(VU1State &S, uint8_t *data, uint32_t dataSize, uint64_t &cycles);

    // ---- The world batch on the GPU (docs/vu1_gpu_path.md, stage 1) --------
    //
    // PS2X_VU1_DIRECT=1 (always on the Vita): the batch is drawn by the GS backend's vertex stage
    // (GSVuBatch) instead of being kicked as the packet the loop writes. The
    // loop still runs (other programs read its output); the packet's XGKICK
    // and the guard-band clipper are skipped (the GPU clips).
    struct DirectWorld
    {
        bool active = false;
        uint64_t tagLo = 0;
        std::vector<GSVuVertex> vertices;
        float m[16]{};
        float fogBase = 0.0f, fogScale = 0.0f;
        float nearPlane[4]{};
    };

    bool directEnabled()
    {
#if defined(__vita__)
        return true; // no environment there; the GPU path is the point of the port
#else
        // 2: every other game frame only, to compare the two paths frame to
        // frame on a still scene.
        static const int mode = std::getenv("PS2X_VU1_DIRECT") ? std::atoi(std::getenv("PS2X_VU1_DIRECT")) : 0;
        if (mode == 2)
            return (ps2_stats::vif1Chains.load(std::memory_order_relaxed) & 1u) == 0u;
        return mode != 0;
#endif
    }

    // The microcode's clipper (0x1410) cuts the triangles its CLIPw marks
    // against the box of the clipping matrix C (vf5-vf8; the same as its
    // copy at 1004-1007, and C's row w is M's). The box's sides lie on or
    // outside the scissor; its near plane (C z >= -C w, at GS Z = 2^24 for
    // SotC's camera) is what the GPU must cut too. On the Vita (no free
    // clip plane: z carries the depth there) a batch reaching behind it
    // stays on VU1.
    bool nearPlane(const VU1State &S, const GSVuVertex *v, size_t count, float plane[4])
    {
        for (int col = 0; col < 4; ++col)
            plane[col] = S.vf[5 + col][2] + S.vf[5 + col][3];
#if defined(__vita__)
        for (size_t i = 0; i < count; ++i)
            if (plane[0] * v[i].x + plane[1] * v[i].y + plane[2] * v[i].z + plane[3] < 0.0f)
                return false;
#else
        (void)v;
        (void)count;
#endif
        return true;
    }

    // Before the loop: the input batch at TOP as the game sent it.
    void captureWorld(const VU1State &S, const uint8_t *data, uint32_t dataSize, DirectWorld &w)
    {
        GS *gs = vu1native::gs();
        if (!directEnabled() || !gs || !gs->supportsVuBatch())
            return;
        const uint32_t mask = dataSize - 1u;
        const uint32_t top = S.top & 0x3FFu;
        uint64_t tagLo;
        std::memcpy(&tagLo, data + ((top * 16u) & mask), 8);
        const uint32_t count = static_cast<uint32_t>(tagLo & 0x7FFFu);
        const uint32_t prim = static_cast<uint32_t>((tagLo >> 47) & 0x7FFu);
        const uint32_t type = prim & 7u;
        const bool pre = ((tagLo >> 46) & 1u) != 0u;
        // Triangles, Gouraud, STQ: what the vertex stage reproduces.
        if (!pre || type < 3u || type > 5u || !(prim & 0x8u) || (prim & 0x100u) || count == 0u || count > 1024u)
        {
            static std::atomic<uint64_t> s_rejects{0};
            const uint64_t n = ++s_rejects;
            if ((n & (n - 1)) == 0)
                std::fprintf(stderr, "[vu1:direct] %llu batches kept on the CPU (tag %016llx)\n",
                             static_cast<unsigned long long>(n), static_cast<unsigned long long>(tagLo));
            return;
        }
        w.tagLo = tagLo;
        w.vertices.resize(count);
        for (uint32_t i = 0; i < count; ++i)
        {
            const uint8_t *q = data + (((top + 1u + i * 3u) * 16u) & mask);
            GSVuVertex &v = w.vertices[i];
            std::memcpy(&v.x, q + 0, 4);
            std::memcpy(&v.y, q + 4, 4);
            std::memcpy(&v.z, q + 8, 4);
            const uint8_t *uv = data + (((top + 2u + i * 3u) * 16u) & mask);
            std::memcpy(&v.s, uv + 0, 4);
            std::memcpy(&v.t, uv + 4, 4);
            const uint8_t *c = data + (((top + 3u + i * 3u) * 16u) & mask);
            int32_t rgba[4];
            std::memcpy(rgba, c, 16);
            v.r = static_cast<uint8_t>(rgba[0]);
            v.g = static_cast<uint8_t>(rgba[1]);
            v.b = static_cast<uint8_t>(rgba[2]);
            v.a = static_cast<uint8_t>(rgba[3]);
        }
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row)
                w.m[col * 4 + row] = S.vf[1 + col][row];
        w.fogBase = S.vf[17][3];  // MULA.w ACC, vf17, vf0
        w.fogScale = S.vf[17][0]; // MR32.w vf29, vf17 -> MADDq.w vf26, vf29, q
        w.active = nearPlane(S, w.vertices.data(), w.vertices.size(), w.nearPlane);
    }

    // After the loop (pc 0x13b8): the microcode's stores before its XGKICK,
    // then the batch to the GS; the microcode resumes after the XGKICK.
    uint32_t finishWorldDirect(VU1State &S, uint8_t *data, uint32_t dataSize, DirectWorld &w)
    {
        const uint32_t mask = dataSize - 1u;
        auto isw = [&](uint32_t qword, int lane, int32_t value)
        {
            const uint32_t v = static_cast<uint16_t>(value & 0xFFFF);
            std::memcpy(data + ((qword * 16u) & mask) + lane * 4, &v, 4);
        };
        isw(1017, 1, S.vi[6]);  // ISW.y vi6, 1017(vi0)
        isw(1017, 2, S.vi[8]);  // ISW.z vi8, 1017(vi0)
        isw(1017, 3, S.vi[14]); // ISW.w vi14, 1017(vi0)  (clipper skipped: the GPU clips)
        vu1native::gs()->submitVuBatch(w.tagLo, std::move(w.vertices), w.m, w.fogBase, w.fogScale, w.nearPlane);
        static std::atomic<uint64_t> s_batches{0};
        const uint64_t n = ++s_batches;
        if ((n & (n - 1)) == 0 && n >= 1024)
            std::fprintf(stderr, "[vu1:direct] %llu world batches on the GPU\n", static_cast<unsigned long long>(n));
        return 0x13f8; // after XGKICK vi6: NOP, then JR vi14
    }

    // ---- The world batch without VU1 (VIF1 -> GPU) ----------------------
    //
    // The game sends each world batch as
    //   UNPACK V4-32 x1 -> TOP+0      GIF tag (NLOOP = vertex count)
    //   UNPACK V3-32 xN -> TOP+1      positions, STCYCL CL=3 WL=1
    //   STMOD 1, UNPACK V2-16 xN -> TOP+2   ST (12-bit fraction) + ROW
    //   STMOD 0, UNPACK V4-8 xN (unsigned) -> TOP+3   colour
    //   MSCNT                          the program resumes at 0x0538/0x05b0
    // and the program draws it and stops again at the same pc, with nothing
    // that later programs read. So the batch goes from the VIF data to the
    // GPU; VU1 data memory and registers are not touched.
    bool worldBatch(const VU1State &S, const vu1native::VifBatch &b)
    {
        if (!directEnabled() || !b.gs || !b.gs->supportsVuBatch() || (b.cycle & 0xFFFFu) != 0x0103u)
            return false;
        const uint8_t *p = b.codes;
        const uint8_t *const end = b.codes + b.size;
        auto code = [&](uint32_t expect, uint32_t mask) -> bool
        {
            if (p + 4 > end)
                return false;
            uint32_t c;
            std::memcpy(&c, p, 4);
            if ((c & mask) != expect)
                return false;
            p += 4;
            return true;
        };
        if (!code(0x6C018000u, 0xFFFFFFFFu))
            return false;
        uint64_t tagLo;
        std::memcpy(&tagLo, p, 8);
        p += 16;
        const uint32_t count = static_cast<uint32_t>(tagLo & 0x7FFFu);
        const uint32_t prim = static_cast<uint32_t>((tagLo >> 47) & 0x7FFu);
        const uint32_t type = prim & 7u;
        if (count == 0u || count > 255u || !((tagLo >> 46) & 1u) || type < 3u || type > 5u || !(prim & 0x8u) || (prim & 0x100u))
            return false;
        const uint32_t n = count << 16;
        if (!code(0x68008001u | n, 0xFFFFFFFFu))
            return false;
        const uint8_t *pos = p;
        p += count * 12u;
        if (!code(0x05000001u, 0xFFFFFFFFu) || !code(0x65008002u | n, 0xFFFFFFFFu))
            return false;
        const uint8_t *st = p;
        p += (count * 4u + 3u) & ~3u;
        if (!code(0x05000000u, 0xFFFFFFFFu) || !code(0x6E00C003u | n, 0xFFFFFFFFu))
            return false;
        const uint8_t *rgba = p;
        p += count * 4u;
        if (!code(0x17000000u, 0xFFFFFFFFu) || p != end)
            return false;

        std::vector<GSVuVertex> vertices(count);
        for (uint32_t i = 0; i < count; ++i)
        {
            GSVuVertex &v = vertices[i];
            std::memcpy(&v.x, pos + i * 12u, 12);
            int16_t uv[2];
            std::memcpy(uv, st + i * 4u, 4);
            v.s = static_cast<int32_t>(uv[0] + b.row[0]); // STMOD 1: + ROW
            v.t = static_cast<int32_t>(uv[1] + b.row[1]);
            v.r = rgba[i * 4u + 0];
            v.g = rgba[i * 4u + 1];
            v.b = rgba[i * 4u + 2];
            v.a = rgba[i * 4u + 3];
        }
        float m[16];
        for (int col = 0; col < 4; ++col)
            for (int row = 0; row < 4; ++row)
                m[col * 4 + row] = S.vf[1 + col][row];
        float plane[4];
        if (!nearPlane(S, vertices.data(), vertices.size(), plane))
            return false;
        b.gs->submitVuBatch(tagLo, std::move(vertices), m, S.vf[17][3], S.vf[17][0], plane);
        return true;
    }

    // Entry: the program's BAL vi14, 0x1140 (its delay slot is a NOP).
    // SOTC_BLOOM_LOG (round 65e): each distinct clipping matrix C (vf5-vf8)
    // the world loop runs with in the logged frames, and how many batches.
    void logClipMatrix(const VU1State &S)
    {
        static const bool on = std::getenv("SOTC_BLOOM_LOG") != nullptr;
        if (!on)
            return;
        static uint64_t frame = ~0ull;
        static std::vector<std::pair<std::vector<float>, uint32_t>> seen;
        const uint64_t f = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        if (f != frame)
        {
            for (const auto &[m, n] : seen)
            {
                std::fprintf(stderr, "[bloom:vu1] f=%llu C (vf5-vf8 columns x y z w) used by %u batches:", (unsigned long long)frame, n);
                for (float x : m)
                    std::fprintf(stderr, " %g", x);
                std::fprintf(stderr, "\n");
            }
            seen.clear();
            frame = f;
        }
        if (!ps2_stats::bloomLogFrame(f))
            return;
        std::vector<float> m;
        for (int r = 5; r <= 8; ++r)
            for (int c = 0; c < 4; ++c)
                m.push_back(S.vf[r][c]);
        for (auto &e : seen)
            if (e.first == m)
            {
                ++e.second;
                return;
            }
        seen.push_back({m, 1u});
    }

    template <uint32_t Entry>
    uint32_t worldLoop(VU1State &S, uint8_t *data, uint32_t dataSize, uint64_t &cycles)
    {
        logClipMatrix(S);
        DirectWorld direct;
        captureWorld(S, data, dataSize, direct);
        const uint32_t pc = worldLoopCpu<Entry>(S, data, dataSize, cycles);
        if (pc == kWorldResume && direct.active)
            return finishWorldDirect(S, data, dataSize, direct);
        return pc;
    }

    template <uint32_t Entry>
    uint32_t worldLoopCpu(VU1State &S, uint8_t *data, uint32_t dataSize, uint64_t &cycles)
    {
        constexpr uint32_t kWorldEntry = Entry;
        if (dataSize == 0 || (dataSize & (dataSize - 1)) != 0)
            return 0;
        Machine m{S, data, dataSize - 1u};
        int32_t *vi = S.vi;
        float *vf0 = S.vf[0];
        float t[4];

        // 0x05b0 BAL vi14, 0x1140 (delay slot 0x05b8: NOP)
        vi[14] = static_cast<int16_t>((kWorldEntry + 16u) / 8u);

        // Prologue 0x1140-0x1240
        vi[4] = static_cast<int32_t>(S.top & 0x3FFu);              // XTOP vi4
        vi[9] = 32767;                                             // IADDIU vi9, vi0, 32767
        vi[11] = static_cast<int16_t>(vi[4]);                      // IADDIU vi11, vi4, 0
        vi[3] = m.ilw(static_cast<uint16_t>(vi[4]), 0);            // ILWR.x vi3, (vi4)
        vi[6] = m.ilw(static_cast<uint16_t>(vi[4]), 3);            // ILWR.w vi6, (vi4)
        vi[2] = static_cast<int16_t>(vi[9] + 1);                   // IADDIU vi2, vi9, 1
        m.lq(S.vf[26], vi[4]);                                     // LQI vf26, (vi4++)
        vi[4] = static_cast<int16_t>(vi[4] + 1);
        vi[3] = vi[9] & vi[3];                                     // IAND vi3, vi9, vi3
        vi[5] = static_cast<int16_t>(vi[6]);                       // IADDIU vi5, vi6, 0
        vi[8] = 0;                                                 // IADDIU vi8, vi0, 0
        S.vf[29][3] = S.vf[17][0];                                 // MR32.w vf29, vf17
        vi[10] = 2;                                                // IADDI vi10, vi0, 2
        m.sq(S.vf[26], vi[5]);                                     // SQI vf26, (vi5++)
        vi[5] = static_cast<int16_t>(vi[5] + 1);
        m.lq(S.vf[18], vi[4]);                                     // LQ vf18, 0(vi4)
        m.lq(S.vf[21], vi[4] + 1);                                 // LQ vf21, 1(vi4)
        m.lq(S.vf[20], vi[4] + 2);                                 // LQ vf20, 2(vi4)
        m.mulaBc(1, S.vf[18][0]);                                  // MULAx ACC, vf1, vf18x
        m.maddaBc(2, S.vf[18][1]);                                 // MADDAy
        m.maddaBc(3, S.vf[18][2]);                                 // MADDAz
        m.maddBc(23, 4, vf0[3], t);                                // MADDw vf23, vf4, vf0w
        std::memcpy(S.vf[23], t, 16);
        m.itof(S.vf[25], S.vf[21], 4096.0f);                       // ITOF12 vf25, vf21
        m.mulaBc(5, S.vf[18][0]);                                  // MULAx ACC, vf5, vf18x
        m.maddaBc(6, S.vf[18][1]);                                 // MADDAy
        {
            const float den = S.vf[23][3];                         // DIV Q, vf0w, vf23w (same pair)
            m.maddaBc(7, S.vf[18][2]);                             // MADDAz
            m.div(den);
        }
        m.maddBc(28, 8, vf0[3], t);                                // MADDw vf28, vf8, vf0w | SQ vf25, 1(vi4)
        m.sq(S.vf[25], vi[4] + 1);
        std::memcpy(S.vf[28], t, 16);
        m.itof(S.vf[24], S.vf[20], 1.0f);                          // ITOF0 vf24, vf20 | LOI 255
        S.i = 255.0f;
        vi[4] = static_cast<int16_t>(vi[4] + 3);                   // IADDIU vi4, vi4, 3
        S.vf[25][2] = vf0[3];                                      // MR32.zw vf25, vf0
        S.vf[25][3] = vf0[0];
        m.sq(S.vf[24], vi[4] - 1);                                 // SQ vf24, -1(vi4)
        {
            const float q = norm(S.q);                             // MULq.xyz vf26, vf23, q | WAITQ
            for (int c = 0; c < 3; ++c)
                S.vf[26][c] = norm(norm(S.vf[23][c]) * q);
        }
        {
            const float q = norm(S.q);                             // MULq.xyz vf25, vf25, q | LQ vf18, 0(vi4)
            for (int c = 0; c < 3; ++c)
                S.vf[25][c] = norm(norm(S.vf[25][c]) * q);
            m.lq(S.vf[18], vi[4]);
        }
        S.acc[3] = norm(norm(S.vf[17][3]) * norm(vf0[3]));         // MULA.w ACC, vf17, vf0
        m.clip(S.vf[28]);                                          // CLIPw.xyz vf28, vf28w

        uint64_t vertices = 0;
        for (;;)
        {
            ++vertices;
            // 0x1248 MADDq.w vf26, vf29, q | LQ vf21, 1(vi4)
            S.vf[26][3] = norm(norm(S.acc[3]) + norm(S.vf[29][3]) * norm(S.q));
            m.lq(S.vf[21], vi[4] + 1);
            // 0x1250 MULAx ACC, vf1, vf18x | SQ vf20, 1(vi5)
            m.sq(S.vf[20], vi[5] + 1);
            m.mulaBc(1, S.vf[18][0]);
            // 0x1258 MADDAy | IADDI vi10, vi10, -1
            m.maddaBc(2, S.vf[18][1]);
            vi[10] = static_cast<int16_t>(vi[10] - 1);
            // 0x1260 MADDAz | FCAND vi1, 0x3ffff
            vi[1] = (S.clip & 0x3FFFFu) != 0u ? 1 : 0;
            m.maddaBc(3, S.vf[18][2]);
            // 0x1268 MADDw vf23, vf4, vf0w | LQ vf20, 2(vi4)
            m.maddBc(23, 4, vf0[3], t);
            m.lq(S.vf[20], vi[4] + 2);
            std::memcpy(S.vf[23], t, 16);
            // 0x1270 MINIi.w vf26, vf26, i | SQ vf25, 0(vi5)
            m.sq(S.vf[25], vi[5]);
            {
                const float iv = norm(S.i), sw = norm(S.vf[26][3]);
                S.vf[26][3] = sw < iv ? sw : iv;
            }
            // 0x1278 ITOF12 vf25, vf21 | IADDIU vi5, vi5, 3
            m.itof(S.vf[25], S.vf[21], 4096.0f);
            vi[5] = static_cast<int16_t>(vi[5] + 3);
            // 0x1280 MULAx ACC, vf5, vf18x | IADDIU vi4, vi4, 3
            m.mulaBc(5, S.vf[18][0]);
            vi[4] = static_cast<int16_t>(vi[4] + 3);
            // 0x1288 ITOF0 vf24, vf20 | DIV Q, vf0w, vf23w
            m.itof(S.vf[24], S.vf[20], 1.0f);
            m.div(S.vf[23][3]);
            // 0x1290 MAXx.w vf26, vf26, vf0x | IADDI vi3, vi3, -1
            {
                const float bc = norm(vf0[0]), sw = norm(S.vf[26][3]);
                S.vf[26][3] = sw > bc ? sw : bc;
            }
            vi[3] = static_cast<int16_t>(vi[3] - 1);
            // 0x1298 MADDAy ACC, vf6, vf18y | SQ vf25, -2(vi4)
            m.sq(S.vf[25], vi[4] - 2);
            m.maddaBc(6, S.vf[18][1]);
            // 0x12a0 MADDAz ACC, vf7, vf18z | MR32.zw vf25, vf0
            m.maddaBc(7, S.vf[18][2]);
            S.vf[25][2] = vf0[3];
            S.vf[25][3] = vf0[0];
            // 0x12a8 MADDw vf28, vf8, vf0w | SQ vf24, -1(vi4)
            m.maddBc(28, 8, vf0[3], t);
            m.sq(S.vf[24], vi[4] - 1);
            std::memcpy(S.vf[28], t, 16);
            // 0x12b0 FTOI4 vf27, vf26 | IBNE vi1, vi0, 0x12e8
            for (int c = 0; c < 4; ++c)
            {
                const int32_t iv = ftoi(norm(S.vf[26][c]), 16.0f);
                std::memcpy(&S.vf[27][c], &iv, 4);
            }
            const bool offScreen = vi[1] != 0;
            // 0x12b8 (delay slot) MULA.w ACC, vf17, vf0 | LQ vf18, 0(vi4)
            S.acc[3] = norm(norm(S.vf[17][3]) * norm(vf0[3]));
            m.lq(S.vf[18], vi[4]);

            if (!offScreen)
            {
                // 0x12c0 MULq.xyz vf26, vf23, q | WAITQ
                {
                    const float q = norm(S.q);
                    for (int c = 0; c < 3; ++c)
                        S.vf[26][c] = norm(norm(S.vf[23][c]) * q);
                }
                // 0x12c8 MULq.xyz vf25, vf25, q | IBNE vi3, vi0, 0x1248
                {
                    const float q = norm(S.q);
                    for (int c = 0; c < 3; ++c)
                        S.vf[25][c] = norm(norm(S.vf[25][c]) * q);
                }
                const bool more = vi[3] != 0;
                // 0x12d0 (delay slot) CLIPw.xyz vf28, vf28w | SQ vf27, -1(vi5)
                m.sq(S.vf[27], vi[5] - 1);
                m.clip(S.vf[28]);
                if (more)
                    continue;
                break; // 0x12d8 B 0x13b8
            }

            // 0x12e8 MTIR vi9, vf27w
            {
                uint32_t bits;
                std::memcpy(&bits, &S.vf[27][3], 4);
                vi[9] = static_cast<int16_t>(bits & 0xFFFFu);
            }
            // 0x12f0 FCAND vi1, 0x20820
            vi[1] = (S.clip & 0x20820u) != 0u ? 1 : 0;
            // 0x1300 MULq.xyz vf26, vf23, q | ISW.w vi1, -6(vi4)
            m.isw(vi[4] - 6, 3, vi[1]);
            {
                const float q = norm(S.q);
                for (int c = 0; c < 3; ++c)
                    S.vf[26][c] = norm(norm(S.vf[23][c]) * q);
            }
            // 0x1308 MULq.xyz vf25, vf25, q | IOR vi9, vi9, vi2
            {
                const float q = norm(S.q);
                for (int c = 0; c < 3; ++c)
                    S.vf[25][c] = norm(norm(S.vf[25][c]) * q);
            }
            vi[9] = vi[9] | vi[2];
            // 0x1310 IBGEZ vi10, 0x13a8 ; 0x1318 (delay slot) MFIR.w vf27, vi9
            const bool stripStart = static_cast<int16_t>(vi[10]) >= 0;
            {
                const int32_t v = static_cast<int16_t>(vi[9] & 0xFFFF);
                std::memcpy(&S.vf[27][3], &v, 4);
            }
            if (!stripStart)
            {
                // Triangle wholly outside one plane: stays ADC. Otherwise it
                // crosses the guard band: flag it for the clipper.
                static const uint32_t kPlanes[5] = {0xff7df7u, 0xffbefbu, 0xffdf7du, 0xffefbeu, 0xfdf7dfu};
                bool outside = false;
                for (uint32_t mask : kPlanes)
                {
                    vi[1] = ((S.clip | mask) == 0xFFFFFFu) ? 1 : 0; // FCOR vi1, mask
                    if (vi[1] != 0)
                    {
                        outside = true;
                        break;
                    }
                }
                if (!outside)
                {
                    m.isw(vi[5] - 3, 3, vi[2]);                     // ISW.w vi2, -3(vi5)
                    vi[8] = static_cast<int16_t>(vi[8] + 1);        // IADDIU vi8, vi8, 1
                }
            }
            // 0x13a8 IBNE vi3, vi0, 0x1248 ; 0x13b0 (delay slot) CLIPw | SQ vf27, -1(vi5)
            const bool more = vi[3] != 0;
            m.sq(S.vf[27], vi[5] - 1);
            m.clip(S.vf[28]);
            if (!more)
                break;
        }
        cycles = 24u + vertices * 13u;
        return kWorldResume;
    }

    // ---- Lit geometry: loop 0x1818 ------------------------------------------
    //
    // As 0x1140 with a normal per vertex (4 qwords: position, normal, 12.4
    // texture coordinate, colour): the normal goes through vf13-vf16 (w forced
    // to 0/1), is clamped at 0, through the light matrix vf9-vf12, and
    // modulates the vertex colour (clamped to 255). Output [STQ, RGBA, XYZF2].
    template <uint32_t Entry>
    uint32_t litLoop(VU1State &S, uint8_t *data, uint32_t dataSize, uint64_t &cycles)
    {
        if (dataSize == 0 || (dataSize & (dataSize - 1)) != 0)
            return 0;
        Machine m{S, data, dataSize - 1u};
        int32_t *vi = S.vi;
        float *vf0 = S.vf[0];
        float t[4];
        auto mulq = [&](int vd, int vs)
        {
            const float q = norm(S.q);
            for (int c = 0; c < 3; ++c)
                S.vf[vd][c] = norm(norm(S.vf[vs][c]) * q);
        };

        // BAL vi14, 0x1818 (delay slot: NOP)
        vi[14] = static_cast<int16_t>((Entry + 16u) / 8u);

        // Prologue 0x1818-0x18f8
        vi[4] = static_cast<int32_t>(S.top & 0x3FFu);              // XTOP vi4
        S.vf[13][3] = norm(norm(vf0[3]) * norm(vf0[0]));           // MULx.w vf13, vf0, vf0x | IADDIU vi9, vi0, 32767
        vi[9] = 32767;
        S.vf[14][3] = norm(norm(vf0[3]) * norm(vf0[0]));           // MULx.w vf14 | IADDIU vi11, vi4, 0
        vi[11] = static_cast<int16_t>(vi[4]);
        S.vf[15][3] = norm(norm(vf0[3]) * norm(vf0[0]));           // MULx.w vf15 | ILWR.x vi3, (vi4)
        vi[3] = m.ilw(static_cast<uint16_t>(vi[4]), 0);
        S.vf[16][3] = norm(norm(vf0[3]) + norm(vf0[0]));           // ADDx.w vf16, vf0, vf0x | IADDIU vi2, vi9, 1
        vi[2] = static_cast<int16_t>(vi[9] + 1);
        vi[6] = m.ilw(static_cast<uint16_t>(vi[4]), 3);            // ILWR.w vi6, (vi4)
        m.lq(S.vf[26], vi[4]);                                     // LQI vf26, (vi4++)
        vi[4] = static_cast<int16_t>(vi[4] + 1);
        vi[3] = vi[9] & vi[3];                                     // IAND vi3, vi9, vi3
        vi[8] = 0;                                                 // IADDIU vi8, vi0, 0
        vi[5] = static_cast<int16_t>(vi[6]);                       // IADDIU vi5, vi6, 0
        m.sq(S.vf[26], vi[5]);                                     // SQI vf26, (vi5++)
        vi[5] = static_cast<int16_t>(vi[5] + 1);
        S.vf[29][3] = S.vf[17][0];                                 // MR32.w vf29, vf17
        vi[10] = 2;                                                // IADDI vi10, vi0, 2
        m.lq(S.vf[18], vi[4]);                                     // LQ vf18, 0(vi4)
        m.lq(S.vf[19], vi[4] + 1);                                 // LQ vf19, 1(vi4)
        m.lq(S.vf[21], vi[4] + 2);                                 // LQ vf21, 2(vi4)
        m.lq(S.vf[20], vi[4] + 3);                                 // LQ vf20, 3(vi4)
        m.mulaBc(1, S.vf[18][0]);                                  // MULAx ACC, vf1, vf18x
        m.maddaBc(2, S.vf[18][1]);                                 // MADDAy
        m.maddaBc(3, S.vf[18][2]);                                 // MADDAz
        m.maddBc(31, 4, vf0[3], t);                                // MADDw vf31, vf4, vf0w
        std::memcpy(S.vf[31], t, 16);
        m.itof(S.vf[25], S.vf[21], 4096.0f);                       // ITOF12 vf25, vf21
        m.mulaBc(5, S.vf[18][0]);                                  // MULAx ACC, vf5, vf18x
        m.maddaBc(6, S.vf[18][1]);                                 // MADDAy
        {
            const float den = S.vf[31][3];                         // MADDAz | DIV Q, vf0w, vf31w
            m.maddaBc(7, S.vf[18][2]);
            m.div(den);
        }
        {
            float moved[4];                                        // MADDw vf28, vf8, vf0w | MOVE vf23, vf31
            std::memcpy(moved, S.vf[31], 16);
            m.maddBc(28, 8, vf0[3], t);
            std::memcpy(S.vf[28], t, 16);
            std::memcpy(S.vf[23], moved, 16);
        }
        m.mulaBc(13, S.vf[19][0]);                                 // MULAx ACC, vf13, vf19x
        m.maddaBc(14, S.vf[19][1]);                                // MADDAy ACC, vf14, vf19y
        m.maddaBc(15, S.vf[19][2]);                                // MADDAz ACC, vf15, vf19z | LOI 255
        S.i = 255.0f;

        uint64_t vertices = 0;
        for (;;)
        {
            ++vertices;
            // 0x1900 MADDw vf24, vf16, vf0w | SQ vf25, 2(vi4)
            m.sq(S.vf[25], vi[4] + 2);
            m.maddBc(24, 16, vf0[3], t);
            std::memcpy(S.vf[24], t, 16);
            // 0x1908 ITOF0 vf20, vf20 | MR32.zw vf25, vf0
            m.itof(S.vf[20], S.vf[20], 1.0f);
            S.vf[25][2] = vf0[3];
            S.vf[25][3] = vf0[0];
            // 0x1910 MULq.xyz vf23, vf23, q | WAITQ
            mulq(23, 23);
            // 0x1918 MULA.w ACC, vf17, vf0 | IADDIU vi5, vi5, 3
            S.acc[3] = norm(norm(S.vf[17][3]) * norm(vf0[3]));
            vi[5] = static_cast<int16_t>(vi[5] + 3);
            // 0x1920 MAXx vf24, vf24, vf0x | IADDIU vi4, vi4, 4
            {
                const float bc = norm(vf0[0]);
                for (int c = 0; c < 4; ++c)
                {
                    const float v = norm(S.vf[24][c]);
                    S.vf[24][c] = v > bc ? v : bc;
                }
            }
            vi[4] = static_cast<int16_t>(vi[4] + 4);
            // 0x1928 MULq.xyz vf25, vf25, q | IADDI vi3, vi3, -1
            mulq(25, 25);
            vi[3] = static_cast<int16_t>(vi[3] - 1);
            // 0x1930 MADDq.w vf23, vf29, q | IADDI vi10, vi10, -1
            S.vf[23][3] = norm(norm(S.acc[3]) + norm(S.vf[29][3]) * norm(S.q));
            vi[10] = static_cast<int16_t>(vi[10] - 1);
            // 0x1938 CLIPw.xyz vf28, vf28w | LQ vf18, 0(vi4)
            m.clip(S.vf[28]);
            m.lq(S.vf[18], vi[4]);
            // 0x1940 MULAx ACC, vf9, vf24x | LQ vf19, 1(vi4)
            m.mulaBc(9, S.vf[24][0]);
            m.lq(S.vf[19], vi[4] + 1);
            // 0x1948 MADDAy ACC, vf10, vf24y | LQ vf21, 2(vi4)
            m.maddaBc(10, S.vf[24][1]);
            m.lq(S.vf[21], vi[4] + 2);
            // 0x1950 MADDAz ACC, vf11, vf24z | SQ vf25, -3(vi5)
            m.maddaBc(11, S.vf[24][2]);
            m.sq(S.vf[25], vi[5] - 3);
            // 0x1958 MADDw vf24, vf12, vf0w | FCAND vi1, 0x3ffff
            vi[1] = (S.clip & 0x3FFFFu) != 0u ? 1 : 0;
            m.maddBc(24, 12, vf0[3], t);
            std::memcpy(S.vf[24], t, 16);
            // 0x1960 MINIi.w vf23, vf23, i
            {
                const float iv = norm(S.i), sw = norm(S.vf[23][3]);
                S.vf[23][3] = sw < iv ? sw : iv;
            }
            // 0x1968 MULAx ACC, vf1, vf18x ; 0x1970 MADDAy ACC, vf2, vf18y
            m.mulaBc(1, S.vf[18][0]);
            m.maddaBc(2, S.vf[18][1]);
            // 0x1978 MUL.xyz vf20, vf24, vf20
            for (int c = 0; c < 3; ++c)
                S.vf[20][c] = norm(norm(S.vf[24][c]) * norm(S.vf[20][c]));
            // 0x1980 MAXx.w vf23, vf23, vf0x
            {
                const float bc = norm(vf0[0]), sw = norm(S.vf[23][3]);
                S.vf[23][3] = sw > bc ? sw : bc;
            }
            // 0x1988 MADDAz ACC, vf3, vf18z ; 0x1990 MADDw vf31, vf4, vf0w
            m.maddaBc(3, S.vf[18][2]);
            m.maddBc(31, 4, vf0[3], t);
            std::memcpy(S.vf[31], t, 16);
            // 0x1998 MINIi vf20, vf20, i
            {
                const float iv = norm(S.i);
                for (int c = 0; c < 4; ++c)
                {
                    const float v = norm(S.vf[20][c]);
                    S.vf[20][c] = v < iv ? v : iv;
                }
            }
            // 0x19a0 FTOI4 vf30, vf23
            for (int c = 0; c < 4; ++c)
            {
                const int32_t iv = ftoi(norm(S.vf[23][c]), 16.0f);
                std::memcpy(&S.vf[30][c], &iv, 4);
            }
            // 0x19a8 MULAx ACC, vf5, vf18x
            m.mulaBc(5, S.vf[18][0]);
            // 0x19b0 MADDAy ACC, vf6, vf18y | DIV Q, vf0w, vf31w
            {
                const float den = S.vf[31][3];
                m.maddaBc(6, S.vf[18][1]);
                m.div(den);
            }
            // 0x19b8 FTOI0 vf24, vf20 | SQ vf20, -1(vi4)
            m.sq(S.vf[20], vi[4] - 1);
            for (int c = 0; c < 4; ++c)
            {
                const int32_t iv = ftoi(norm(S.vf[20][c]), 1.0f);
                std::memcpy(&S.vf[24][c], &iv, 4);
            }
            // 0x19c0 ITOF12 vf25, vf21 | LQ vf20, 3(vi4)
            m.itof(S.vf[25], S.vf[21], 4096.0f);
            m.lq(S.vf[20], vi[4] + 3);
            // 0x19c8 MADDAz ACC, vf7, vf18z | SQ vf30, -1(vi5)
            m.maddaBc(7, S.vf[18][2]);
            m.sq(S.vf[30], vi[5] - 1);
            // 0x19d0 MADDw vf28, vf8, vf0w | IBNE vi1, vi0, 0x1a00
            m.maddBc(28, 8, vf0[3], t);
            std::memcpy(S.vf[28], t, 16);
            const bool offScreen = vi[1] != 0;
            // 0x19d8 (delay slot) MULAx ACC, vf13, vf19x | SQ vf24, -2(vi5)
            m.mulaBc(13, S.vf[19][0]);
            m.sq(S.vf[24], vi[5] - 2);

            if (offScreen)
            {
                // 0x1a00 ILW.w vi9, -1(vi5)
                vi[9] = m.ilw(vi[5] - 1, 3);
                // 0x1a08 FCAND vi1, 0x20820
                vi[1] = (S.clip & 0x20820u) != 0u ? 1 : 0;
                // 0x1a18 ISW.w vi1, -4(vi4)
                m.isw(vi[4] - 4, 3, vi[1]);
                // 0x1a20 IOR vi9, vi9, vi2
                vi[9] = vi[9] | vi[2];
                // 0x1a28 IBGEZ vi10, 0x1ac0 ; 0x1a30 (delay slot) ISW.w vi9, -1(vi5)
                const bool stripStart = static_cast<int16_t>(vi[10]) >= 0;
                m.isw(vi[5] - 1, 3, vi[9]);
                if (!stripStart)
                {
                    static const uint32_t kPlanes[5] = {0xff7df7u, 0xffbefbu, 0xffdf7du, 0xffefbeu, 0xfdf7dfu};
                    bool outside = false;
                    for (uint32_t mask : kPlanes)
                    {
                        vi[1] = ((S.clip | mask) == 0xFFFFFFu) ? 1 : 0; // FCOR vi1, mask
                        if (vi[1] != 0)
                        {
                            outside = true;
                            break;
                        }
                    }
                    if (!outside)
                    {
                        m.isw(vi[5] - 3, 3, vi[2]);                 // ISW.w vi2, -3(vi5)
                        vi[8] = static_cast<int16_t>(vi[8] + 1);    // IADDIU vi8, vi8, 1
                    }
                }
            }
            // 0x19e0 / 0x1ac0 MADDAy ACC, vf14, vf19y | IBNE vi3, vi0, 0x1900
            m.maddaBc(14, S.vf[19][1]);
            const bool more = vi[3] != 0;
            // 0x19e8 / 0x1ac8 (delay slot) MADDAz ACC, vf15, vf19z | MOVE vf23, vf31
            m.maddaBc(15, S.vf[19][2]);
            std::memcpy(S.vf[23], S.vf[31], 16);
            if (!more)
                break; // 0x19f0 B 0x1ad0, or falls into 0x1ad0
        }
        cycles = 32u + vertices * 30u;
        return kLitResume;
    }

    // ---- Second UV set: loops 0x0f80 (after 0x1140) and 0x1060 (after 0x1818)
    //
    // Multi-texture passes redraw a batch with another texture: the output
    // the previous loop left is kept, only its ST gets the input's second
    // coordinate pair (zw of the texture-coordinate qword) times a Q.
    // Lit: 0x1060 (4-qword input, the Q factor from vf26 as loaded), else
    // 0x0f80 (3-qword input, Q from the output's STQ).
    template <uint32_t Entry, bool Lit>
    uint32_t uvPass(VU1State &S, uint8_t *data, uint32_t dataSize, uint64_t &cycles)
    {
        if (dataSize == 0 || (dataSize & (dataSize - 1)) != 0)
            return 0;
        Machine m{S, data, dataSize - 1u};
        int32_t *vi = S.vi;
        float *vf0 = S.vf[0];
        constexpr unsigned XY = 0x3u, ZW = 0xCu;
        constexpr int32_t uvOffset = Lit ? 2 : 1; // the texture-coordinate qword
        constexpr int32_t stride = Lit ? 4 : 3;

        // BAL vi14, loop (delay slot: NOP)
        vi[14] = static_cast<int16_t>((Entry + 16u) / 8u);
        vi[4] = static_cast<int32_t>(S.top & 0x3FFu);              // XTOP vi4
        vi[9] = 32767;                                             // IADDIU vi9, vi0, 32767
        vi[11] = static_cast<int16_t>(vi[4]);                      // IADDIU vi11, vi4, 0
        vi[3] = m.ilw(static_cast<uint16_t>(vi[4]), 0);            // ILWR.x vi3, (vi4)
        vi[6] = m.ilw(static_cast<uint16_t>(vi[4]), 3);            // ILWR.w vi6, (vi4)
        vi[2] = static_cast<int16_t>(vi[9] + 1);                   // IADDIU vi2, vi9, 1
        m.lq(S.vf[26], vi[4]);                                     // LQI vf26, (vi4++)
        vi[4] = static_cast<int16_t>(vi[4] + 1);
        vi[3] = vi[9] & vi[3];                                     // IAND vi3, vi9, vi3
        vi[5] = static_cast<int16_t>(vi[6]);                       // IADDIU vi5, vi6, 0
        vi[8] = m.ilw(1017, 2);                                    // ILW.z vi8, 1017(vi0)
        S.vf[29][3] = S.vf[17][0];                                 // MR32.w vf29, vf17
        vi[10] = 2;                                                // IADDI vi10, vi0, 2
        m.sq(S.vf[26], vi[5]);                                     // SQI vf26, (vi5++)
        vi[5] = static_cast<int16_t>(vi[5] + 1);
        m.lqLanes(S.vf[21], vi[4] + uvOffset, ZW);                 // LQ.zw vf21, uv(vi4)
        m.lqLanes(S.vf[25], vi[5], ZW);                            // LQ.zw vf25, 0(vi5)
        S.vf[21][0] = norm(norm(vf0[0]) + norm(S.vf[21][2]));      // ADDz.x vf21, vf0, vf21z
        S.vf[21][1] = norm(norm(vf0[1]) + norm(S.vf[21][3]));      // ADDw.y vf21, vf0, vf21w

        uint64_t vertices = 0;
        for (;;)
        {
            ++vertices;
            // MULw.xy vf25, vf21, vf0w | (unlit) LQ.zw vf26, 0(vi5) / (lit) IADDIU vi5, vi5, 3
            for (int c = 0; c < 2; ++c)
                S.vf[25][c] = norm(norm(S.vf[21][c]) * norm(vf0[3]));
            if (Lit)
                vi[5] = static_cast<int16_t>(vi[5] + 3);
            else
                m.lqLanes(S.vf[26], vi[5], ZW);
            m.sqLanes(S.vf[21], vi[4] + uvOffset, XY);             // SQ.xy vf21, uv(vi4)
            m.lqLanes(S.vf[21], vi[4] + uvOffset + stride, ZW);    // LQ.zw vf21, uv+stride(vi4)
            if (!Lit)
                vi[4] = static_cast<int16_t>(vi[4] + stride);      // IADDIU vi4, vi4, 3
            // MULz.xy vf26, vf25, vf26z | (unlit) IADDI vi3, -1 / (lit) IADDIU vi4, vi4, 4
            {
                const float z = norm(S.vf[26][2]);
                for (int c = 0; c < 2; ++c)
                    S.vf[26][c] = norm(norm(S.vf[25][c]) * z);
            }
            vi[3] = static_cast<int16_t>(vi[3] - 1);
            if (Lit)
                vi[4] = static_cast<int16_t>(vi[4] + stride);
            else
                vi[5] = static_cast<int16_t>(vi[5] + 3);           // IADDIU vi5, vi5, 3
            // ADDz.x vf21, vf0, vf21z | LQ.zw vf25, 0(vi5)
            S.vf[21][0] = norm(norm(vf0[0]) + norm(S.vf[21][2]));
            m.lqLanes(S.vf[25], vi[5], ZW);
            // ADDw.y vf21, vf0, vf21w | IBNE vi3, vi0, loop ; (delay slot) SQ.xy vf26, -3(vi5)
            S.vf[21][1] = norm(norm(vf0[1]) + norm(S.vf[21][3]));
            const bool more = vi[3] != 0;
            m.sqLanes(S.vf[26], vi[5] - 3, XY);
            if (!more)
                break; // B 0x13b8 / 0x1ad0
        }
        cycles = 16u + vertices * 8u;
        return Lit ? kLitResume : kWorldResume;
    }

    // Every program that runs one of the loops.
    vu1native::BatchRegistrar g_worldBatch0538(kWorldImage, 0x0538, worldBatch);
    vu1native::BatchRegistrar g_worldBatch05b0(kWorldImage, 0x05b0, worldBatch);
    vu1native::Registrar g_world0538(kWorldImage, 0x0538, worldLoop<0x0538>);
    vu1native::Registrar g_world05b0(kWorldImage, 0x05b0, worldLoop<0x05b0>);
    vu1native::Registrar g_world0628(kWorldImage, 0x0628, worldLoop<0x0628>);
    vu1native::Registrar g_lit0710(kWorldImage, 0x0710, litLoop<0x0710>);
    vu1native::Registrar g_lit0788(kWorldImage, 0x0788, litLoop<0x0788>);
    vu1native::Registrar g_lit0860(kWorldImage, 0x0860, litLoop<0x0860>);
    vu1native::Registrar g_lit0920(kWorldImage, 0x0920, litLoop<0x0920>);
    vu1native::Registrar g_lit09e0(kWorldImage, 0x09e0, litLoop<0x09e0>);
    vu1native::Registrar g_lit0aa0(kWorldImage, 0x0aa0, litLoop<0x0aa0>);
    vu1native::Registrar g_uv0698(kWorldImage, 0x0698, uvPass<0x0698, false>);
    vu1native::Registrar g_uv07f8(kWorldImage, 0x07f8, uvPass<0x07f8, true>);
}
