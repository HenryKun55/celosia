// Round S4 (SOTC_S4DISP, A/B s4disp): display / draw prep without the PS2 packets nobody reads.
//
// With SOTC_NATIVE=1 the GS rasterises nothing and no VU1 program runs (docs/display_map.md 5a): the native renderer draws
// from host records made at its hooks and redoes every screen effect as its own pass from guest globals read at endFrame.
// The game's screen passes (graphFrameFinish's water reflection set-up, zFog and the background blend, the shadow
// reduce, the depth of field, the refraction copies, the bloom chain ...) still ran in full each frame, building GS
// register packets (gsReg A+D pairs, spriteNoTexFast / spriteTexFast sprites, layer pushes, gsRegBegin / gsRegEnd) whose
// only reader was the emulated GS; each guest call goes through the function table and its hook (callGuest, the
// return-address classification), ~40-140 calls a pass.
//
// Here (part "passes", SOTC_S4DISP_PASSES, default on) each such pass is replaced, while the native renderer draws, by
// a host-only variant that keeps what the rest of the game and the renderer read of it (the guest words outside the
// display list's packet memory: the ripple clock, the VRAM requests, the root draw context; the host notes: the camera
// context, the bloom flag, zFog's parameters) and builds no packet. Which words those are was found, and is checked, by
// the check mode below; docs/display_s4.md lists them per pass.
//
// SOTC_S4DISP_CHECK=<n> (default off): every n-th call of each pass (n = 1: all) runs the original as before, with the
// guest RAM snapshotted at entry; afterwards every word the original changed is classified: the stack below the entry
// sp (its frames), the display list's packet memory (the work buffer's new packets, the layers' chain tails, gsReg's
// packet state: dead with the native renderer), a word the variant reproduces (compared with the value the variant
// computes from the entry state), or anything else (a difference: the variant misses a write). The host side: the
// renderer's emissions during the original (draw jobs, 2D writes, notes) must be the ones the variant makes, and the
// host state (layer mirror, requested texture, camera note, bloom flag, fog) the one the variant leaves. [s4check] lines
// every 600 frames; unknown ranges listed. The other calls run the original (the check mode draws as the off state).
// SOTC_S4DISP_PASSES=<mask>: which passes (bit i = kPasses[i]; default all). SOTC_S4DISP_LOG=1: calls per pass.
#include "sotc_decomp.h"
#include "disp_types.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_diag.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace sotc_native
{
    bool s4NativeDrawing();
    uint64_t s4Emits();
    uint64_t s4HostDigest();
    void s4NoteCamera(uint32_t context);
    void s4BloomRan();
    void s4SkipZFog(bool on);
    extern uint64_t g_frame;
}

namespace s4disp
{
    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
    };

    // A word the variant writes: its address and the value it gives from the entry state.
    struct Effect
    {
        uint32_t at, value;
    };
    // What a variant does to host state beyond the guest words (applied to the check's expected host digest).
    struct HostEffects
    {
        bool camera = false; // the camera context noted (iosgluGetCameraContext inside the pass)
        bool bloom = false;  // the bloom flag (gsReg from inside the bloom chain)
        uint32_t emits = 0;  // emissions the variant makes itself (hooks it calls)
    };

    using Fn = PS2Runtime::RecompiledFunction;
    struct Pass
    {
        uint32_t address;
        const char *name;
        // The variant's guest writes from the entry state (no write: computed only); the check compares them.
        void (*effects)(const Ram &ram, R5900Context *ctx, std::vector<Effect> &out);
        // The variant itself: host-only (ctx->pc set to ra by the caller after it).
        void (*variant)(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn orig);
        HostEffects host;
    };

    // ---- the variants ----------------------------------------------------------------------------------------------
    uint32_t cameraNow(const Ram &ram) { return ram.u32(disp::camera::kTable + ram.u32(disp::camera::kIndex) * 4u); }

    void noEffects(const Ram &, R5900Context *, std::vector<Effect> &) {}
    void nothing(uint8_t *, R5900Context *, PS2Runtime *, Fn) {}
    void noteCamera(uint8_t *rdram, R5900Context *, PS2Runtime *, Fn) { sotc_native::s4NoteCamera(cameraNow(Ram{rdram})); }

    // zFog: its hook (zFogHook: the fog's parameters into the renderer) without the guest zFog (and bgDraw it calls).
    void zFogVariant(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn orig)
    {
        const uint32_t ra = GPR_U32(ctx, 31);
        sotc_native::s4SkipZFog(true);
        orig(rdram, ctx, runtime);
        sotc_native::s4SkipZFog(false);
        ctx->pc = ra;
        sotc_native::s4NoteCamera(cameraNow(Ram{rdram}));
    }
    void bloomVariant(uint8_t *, R5900Context *, PS2Runtime *, Fn) { sotc_native::s4BloomRan(); }

    // reflectionBegin: the ripple clock (0x118c030 += 0.1 x the frame step f12, the code's FPU ops: mul.s then add.s).
    constexpr uint32_t kRippleTime = 0x0118C030u;
    void reflectionEffects(const Ram &ram, R5900Context *ctx, std::vector<Effect> &out)
    {
        const float step = ctx->f[12];
        const float t = ram.f32(kRippleTime) + step * 0.1f;
        uint32_t b;
        std::memcpy(&b, &t, 4);
        out.push_back({kRippleTime, b});
    }
    void reflectionVariant(uint8_t *rdram, R5900Context *ctx, PS2Runtime *, Fn)
    {
        std::vector<Effect> e;
        const Ram ram{rdram};
        reflectionEffects(ram, ctx, e);
        for (const Effect &x : e)
            ram.put32(x.at, x.value);
    }

    // The passes. Bits of SOTC_S4DISP_PASSES in this order.
    const Pass kPasses[] = {
        {0x0109AD58u, "reflectionBegin", reflectionEffects, reflectionVariant, {}},
        {0x010AC0A8u, "zFog", noEffects, zFogVariant, {true, false, 1}},
        {0x0109D320u, "shadowDraw", noEffects, noteCamera, {true, false, 0}},
        {0x0109D158u, "shadowReset", noEffects, nothing, {}},
        {0x01084DE8u, "bgEnd", noEffects, nothing, {}},
        {0x010874E8u, "depthOfField", noEffects, noteCamera, {true, false, 0}},
        {0x0109C108u, "waterRefractionBegin", noEffects, nothing, {}},
        {0x0109E2A0u, "bloomDisplay", noEffects, bloomVariant, {false, true, 1}},
        {0x0109FB08u, "bloom2_0109FB08", noEffects, nothing, {}},
        {0x010A1518u, "solarFlareReset", noEffects, nothing, {}},
        {0x010F37C8u, "hazeCopy_010F37C8", noEffects, nothing, {}},
    };
    constexpr int kPassCount = sizeof(kPasses) / sizeof(kPasses[0]);
    Fn g_orig[kPassCount] = {};

    // ---- switches ----------------------------------------------------------------------------------------------------
    uint32_t g_mask = ~0u;
    int g_checkEvery = 0;
    bool g_log = false;
    uint64_t g_calls[kPassCount][2] = {}; // [variant, original]

    // ---- the check -------------------------------------------------------------------------------------------------
    struct CheckStats
    {
        uint64_t checks = 0, incomplete = 0, hostDiffer = 0, emitDiffer = 0, effectDiffer = 0, unknownWords = 0, stackWords = 0, packetWords = 0;
        std::map<uint32_t, std::pair<uint32_t, uint64_t>> unknown; // range start -> (end, times)
    };
    CheckStats g_check[kPassCount];
    uint8_t *g_snap = nullptr;

    // The display list's packet memory as the entry state gives it: the work buffer from its free pointer on (new packets),
    // the two packet buffers' layer packets (their chain tails), gsReg's packet state.
    struct DeadRanges
    {
        std::vector<std::pair<uint32_t, uint32_t>> r;
        bool has(uint32_t a) const
        {
            for (const auto &x : r)
                if (a >= x.first && a < x.second)
                    return true;
            return false;
        }
    };
    DeadRanges deadRanges(const Ram &ram)
    {
        DeadRanges d;
        const uint32_t list = disp::kDisplayList;
        const uint32_t work = ram.u32(list + 0x10u);
        if (work && work < PS2_RAM_SIZE)
        {
            const uint32_t free = ram.u32(work + 0x20u);
            if (free && free < PS2_RAM_SIZE)
                d.r.push_back({free & ~0xFu, std::min<uint32_t>((free & ~0xFu) + 0x100000u, PS2_RAM_SIZE)});
            d.r.push_back({work, work + 0x40u});
        }
        for (uint32_t k = 0; k < 2u; ++k)
        {
            const uint32_t packets = ram.u32(list + 8u + 4u * k), count = ram.u32(list + 0x14u);
            if (packets && packets < PS2_RAM_SIZE && count < 256u)
                d.r.push_back({packets, packets + (count + 1u) * 0x30u});
        }
        const uint32_t gs = ram.u32(disp::kGsRegState);
        if (gs && gs < PS2_RAM_SIZE)
            d.r.push_back({gs, gs + 0x20u});
        d.r.push_back({disp::kGsRegState, disp::kGsRegState + 8u});
        d.r.push_back({disp::kLayerDepth, disp::kLayerDepth + 4u});
        d.r.push_back({disp::kSavedLayers, disp::kSavedLayers + 0x400u});
        d.r.push_back({list, list + 0x20u});
        return d;
    }

    void report()
    {
        static uint64_t told = 0;
        if (sotc_native::g_frame < told + 600u)
            return;
        told = sotc_native::g_frame;
        if (g_log)
        {
            std::string calls;
            for (int i = 0; i < kPassCount; ++i)
                calls += std::string(" ") + kPasses[i].name + " " + std::to_string(g_calls[i][0]) + "/" + std::to_string(g_calls[i][1]);
            std::fprintf(stderr, "[s4disp] frame %llu calls (variant / original):%s\n", (unsigned long long)sotc_native::g_frame, calls.c_str());
        }
        for (int i = 0; i < kPassCount; ++i)
        {
            CheckStats &c = g_check[i];
            if (!c.checks && !c.incomplete)
                continue;
            std::string line;
            int shown = 0;
            for (const auto &u : c.unknown)
            {
                if (shown++ >= 12)
                    break;
                char b[64];
                std::snprintf(b, sizeof(b), " %08x-%08x x%llu", u.first, u.second.first, (unsigned long long)u.second.second);
                line += b;
            }
            std::fprintf(stderr,
                         "[s4check] frame %llu %s: %llu checked (%llu incomplete): guest words unknown %llu, effects differ %llu, host state differ %llu, "
                         "emissions differ %llu (stack %llu, packet %llu words);%s\n",
                         (unsigned long long)sotc_native::g_frame, kPasses[i].name, (unsigned long long)c.checks, (unsigned long long)c.incomplete,
                         (unsigned long long)c.unknownWords, (unsigned long long)c.effectDiffer, (unsigned long long)c.hostDiffer,
                         (unsigned long long)c.emitDiffer, (unsigned long long)c.stackWords, (unsigned long long)c.packetWords, line.c_str());
        }
    }

    void checkCall(int i, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Pass &p = kPasses[i];
        CheckStats &c = g_check[i];
        if (!g_snap)
            g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
        if (!g_snap)
            return g_orig[i](rdram, ctx, runtime);
        const Ram ram{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        std::vector<Effect> expected;
        p.effects(ram, ctx, expected);
        const DeadRanges dead = deadRanges(ram);
        // The host state the variant would leave: the entry state plus its notes.
        const uint64_t emits0 = sotc_native::s4Emits();
        std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
        // (host digest after the variant's notes: computed after the original, from the same notes)
        g_orig[i](rdram, ctx, runtime);
        if (ctx->pc != ra0 || runtime->transferPending())
        {
            ++c.incomplete;
            return;
        }
        ++c.checks;
        const uint64_t emitsOrig = sotc_native::s4Emits() - emits0;
        const uint64_t digestOrig = sotc_native::s4HostDigest();
        // The variant's host side from the original's end state: its notes again (camera: the context as the table gives
        // it now; bloom) must leave the same digest; and the original's emissions are the variant's own.
        if (p.host.camera)
            sotc_native::s4NoteCamera(cameraNow(ram));
        if (p.host.bloom)
            sotc_native::s4BloomRan();
        c.hostDiffer += sotc_native::s4HostDigest() != digestOrig;
        // (the bloom chain's gsReg writes each set the flag: one or more is the variant's one)
        c.emitDiffer += p.host.bloom ? (emitsOrig == 0u) : (emitsOrig != p.host.emits);
        // The guest words.
        for (uint32_t page = 0; page < PS2_RAM_SIZE; page += 4096u)
        {
            if (std::memcmp(rdram + page, g_snap + page, 4096u) == 0)
                continue;
            for (uint32_t a = page; a < page + 4096u; a += 4u)
            {
                uint32_t now, was;
                std::memcpy(&now, rdram + a, 4);
                std::memcpy(&was, g_snap + a, 4);
                if (now == was)
                    continue;
                if (a < sp0 && a + 0x4000u >= sp0)
                {
                    ++c.stackWords;
                    continue;
                }
                const Effect *e = nullptr;
                for (const Effect &x : expected)
                    if (x.at == a)
                        e = &x;
                if (e)
                {
                    c.effectDiffer += e->value != now;
                    continue;
                }
                if (dead.has(a))
                {
                    ++c.packetWords;
                    continue;
                }
                ++c.unknownWords;
                // Coalesce into ranges (64-byte gaps).
                auto it = c.unknown.upper_bound(a);
                if (it != c.unknown.begin())
                {
                    auto prev = std::prev(it);
                    if (a <= prev->second.first + 64u)
                    {
                        prev->second.first = std::max(prev->second.first, a + 4u);
                        ++prev->second.second;
                        continue;
                    }
                }
                if (c.unknown.size() < 64u)
                    c.unknown[a] = {a + 4u, 1u};
            }
        }
        // An expected word the original left unchanged (the variant would write it): compare with the entry value.
        for (const Effect &x : expected)
        {
            uint32_t now;
            std::memcpy(&now, rdram + (x.at & PS2_RAM_MASK), 4);
            c.effectDiffer += now != x.value;
        }
    }

    template <int I>
    void wrap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kS4Disp) || !(g_mask & (1u << I)) || !sotc_native::s4NativeDrawing())
        {
            ++g_calls[I][1];
            if (g_log)
                report();
            return g_orig[I](rdram, ctx, runtime);
        }
        if (g_log)
            report();
        if (g_checkEvery > 0)
        {
            static uint64_t n = 0;
            ++g_calls[I][1];
            if (++n % static_cast<uint64_t>(g_checkEvery) == 0u)
                checkCall(I, rdram, ctx, runtime);
            else
                g_orig[I](rdram, ctx, runtime);
            report();
            return;
        }
        ++g_calls[I][0];
        const uint32_t ra = GPR_U32(ctx, 31);
        kPasses[I].variant(rdram, ctx, runtime, g_orig[I]);
        ctx->pc = ra;
    }
    template <int... I>
    void installAll(PS2Runtime &runtime, std::integer_sequence<int, I...>)
    {
        ((runtime.hasFunction(kPasses[I].address) ? (g_orig[I] = runtime.lookupFunction(kPasses[I].address), runtime.registerFunction(kPasses[I].address, wrap<I>), 0)
                                                  : 0),
         ...);
    }
    void install(PS2Runtime &runtime)
    {
        if (!std::getenv("SOTC_NATIVE"))
            return;
        if (const char *m = std::getenv("SOTC_S4DISP_PASSES"))
            g_mask = static_cast<uint32_t>(std::strtoul(m, nullptr, 0));
        if (const char *c = std::getenv("SOTC_S4DISP_CHECK"))
            g_checkEvery = std::max(0, std::atoi(c));
        g_log = ps2_diag::env("SOTC_S4DISP_LOG") != nullptr;
        installAll(runtime, std::make_integer_sequence<int, kPassCount>{});
        std::fprintf(stderr, "[s4disp] %d passes wrapped (mask %x, check every %d)\n", kPassCount, g_mask, g_checkEvery);
    }
}
