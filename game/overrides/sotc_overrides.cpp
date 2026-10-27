#include <cstring>
// Shadow of the Colossus (SCUS-97472) runtime overrides.
#include <cstdio>
#include <utility>
#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "game_overrides.h"
#include "lean_hot.h"
#include "runtime/ee_scheduler.h"
#include "ps2x/iop/iop_subsystem.h"
#include "runtime/ps2_stats.h"

#include <array>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

void installSotcNative(PS2Runtime &runtime); // sotc_native.cpp
#include "sotc_decomp.h"
#include "sotc_prof.h"

namespace
{
    // Entry point of GAMECORE.XFF as laid out by the game's own loader.
    constexpr uint32_t kGamecoreEntry = 0x01344788u;

    PS2Runtime::RecompiledFunction g_gamecoreEntry = nullptr;
    bool g_gamecoreEntered = false;

    // Module placement depends on the guest heap, which is only reproducible
    // while emulated time is driven purely by executed cycles. Once all code
    // modules are loaded, switch to real-time pacing.
    void gamecoreEntryHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static bool enabled = false;
        if (!enabled)
        {
            enabled = true;
            g_gamecoreEntered = true;
            runtime->eeScheduler().enableHostClockSync();
            std::cerr << "[sotc] GAMECORE entered; host clock sync enabled" << std::endl;
            // With SOTC_NATIVE the IOP runs no code from the start (the boot's
            // layout made deterministic by heapHook); PS2X_NATIVE_IOP_BOOT=0
            // keeps the IRXs for the boot, the emulated IOP stopping here.
            if (ps2x::iop::IopSubsystem *iop = runtime->iopSubsystem())
                iop->enterNativeMode();
        }
        g_gamecoreEntry(rdram, ctx, runtime);
    }

    // SOTC_TRACE_CALLS="0xaddr,0xaddr": count calls that go through the function
    // table (indirect calls, callbacks) and report them periodically.
    constexpr int kMaxTraced = 48;
    std::array<uint32_t, kMaxTraced> g_tracedAddr{};
    std::array<PS2Runtime::RecompiledFunction, kMaxTraced> g_tracedOriginal{};
    std::array<std::atomic<uint64_t>, kMaxTraced> g_tracedCount{};

    template <int Slot>
    void tracedCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t n = g_tracedCount[Slot].fetch_add(1, std::memory_order_relaxed) + 1;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), ra = GPR_U32(ctx, 31);
        g_tracedOriginal[Slot](rdram, ctx, runtime);
        static const uint64_t every = std::getenv("SOTC_TRACE_EVERY") ? std::strtoull(std::getenv("SOTC_TRACE_EVERY"), nullptr, 0) : 1000;
        if (n <= 3 || n % every == 0)
        {
            std::cerr << "[sotc:call] 0x" << std::hex << g_tracedAddr[Slot] << std::dec << " count=" << n
                      << " a0=0x" << std::hex << a0 << " a1=0x" << a1 << " ra=0x" << ra << " -> v0=0x" << GPR_U32(ctx, 2) << std::dec << std::endl;
        }
    }

    template <int... Slots>
    constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeTracers(std::integer_sequence<int, Slots...>)
    {
        return {&tracedCall<Slots>...};
    }

    void installCallTracing(PS2Runtime &runtime)
    {
        const char *env = std::getenv("SOTC_TRACE_CALLS");
        if (!env)
            return;
        static const auto tracers = makeTracers(std::make_integer_sequence<int, kMaxTraced>{});
        std::stringstream list(env);
        std::string item;
        int slot = 0;
        while (std::getline(list, item, ',') && slot < kMaxTraced)
        {
            const uint32_t addr = static_cast<uint32_t>(std::stoul(item, nullptr, 0));
            if (!runtime.hasFunction(addr))
                continue;
            g_tracedAddr[slot] = addr;
            g_tracedOriginal[slot] = runtime.lookupFunction(addr);
            runtime.registerFunction(addr, tracers[slot]);
            ++slot;
        }
    }

    // SOTC_CENSUS=1: calls per frame of the game's drawing functions
    // (docs/native_renderer_map.md), printed every 120 game frames; for
    // modelDisp also the distinct models and their draw packets.
    struct CensusEntry
    {
        uint32_t addr;
        const char *name;
    };
    constexpr CensusEntry kCensus[] = {
        {0x01097118u, "modelDisp"}, {0x01098450u, "modelClipSkin"}, {0x01098840u, "modelClipSolid"}, {0x01098cc0u, "skinDisp?"},
        {0x0109ab80u, "meshDisp"}, {0x0109a4d8u, "meshDispInstance"}, {0x0109e0e8u, "shadowDisp"}, {0x0109d320u, "shadowDraw"},
        {0x0109a228u, "ptclmanEnd"}, {0x0109a170u, "ptclDisplayListPut"}, {0x0109c108u, "waterRefraction"}, {0x01085258u, "bgDraw"},
        {0x01090a88u, "kPrintf"}, {0x001c5548u, "spriteTexFast"}, {0x010a32b8u, "spriteDrawSprite"}, {0x01114b50u, "transMicroCode"},
        {0x010aad48u, "texTransRequest"}, {0x010aae70u, "texTransRequestMulti"}, {0x01092f50u, "lineObjectDisp"},
        {0x01348468u, "dispVuColiDispWork"}, {0x01385128u, "dischargeMain"},
        // 2D: sprite objects, the immediate gl* API, text
        {0x010a24c0u, "spriteDrawSpriteTexNoSet"}, {0x010a45f0u, "spriteDrawSpriteUV"}, {0x010a4890u, "spriteDrawSpriteST"},
        {0x010a4b38u, "spriteDrawSpriteUV2"}, {0x010a4ed0u, "spriteDrawSpriteST2"}, {0x001c5110u, "spriteNoTexFast"},
        {0x0108e530u, "glBegin"}, {0x0108bb38u, "glBeginWithDlLayerNo"}, {0x0108d060u, "glEnd"}, {0x0108ea20u, "glVertex2f"},
        {0x0108eb28u, "glVertex3f"}, {0x0108ebc0u, "glVertex3fv"}, {0x0108ec08u, "glVertex4f"}, {0x01090bd8u, "kPrintfScale"},
        {0x01090d28u, "kPrintfAsSprite"}, {0x01090ff8u, "kprintfContext"}, {0x010910b0u, "kprintfContextCentering"},
        // model system siblings that chain packets
        {0x01097510u, "m97510"}, {0x01097640u, "m97640"}, {0x01097688u, "m97688"}, {0x01097798u, "m97798"},
        {0x01097928u, "m97928"}, {0x01097b70u, "m97b70"}, {0x01097d88u, "m97d88"}, {0x01097f88u, "m97f88"},
    };
    constexpr int kCensusCount = static_cast<int>(sizeof(kCensus) / sizeof(kCensus[0]));
    std::array<PS2Runtime::RecompiledFunction, kCensusCount> g_censusOriginal{};
    std::array<uint64_t, kCensusCount> g_censusCalls{};
    uint64_t g_censusModelPackets = 0;
    std::array<uint32_t, 4096> g_censusModels{}; // distinct model headers this window (open addressing)
    uint64_t g_censusDistinct = 0, g_censusFrame0 = 0;

    void censusReport()
    {
        const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        if (g_censusFrame0 == 0)
            g_censusFrame0 = frame;
        if (frame < g_censusFrame0 + 120u)
            return;
        const double frames = static_cast<double>(frame - g_censusFrame0);
        char line[128];
        std::string out = "[census] frame=" + std::to_string(frame);
        for (int i = 0; i < kCensusCount; ++i)
            if (g_censusCalls[i])
            {
                std::snprintf(line, sizeof(line), " %s=%.1f", kCensus[i].name, g_censusCalls[i] / frames);
                out += line;
                g_censusCalls[i] = 0;
            }
        std::snprintf(line, sizeof(line), " | distinctModels=%llu modelPackets/frame=%.1f", (unsigned long long)g_censusDistinct,
                      g_censusModelPackets / frames);
        out += line;
        std::cerr << out << std::endl;
        g_censusModelPackets = 0;
        g_censusDistinct = 0;
        g_censusModels.fill(0);
        g_censusFrame0 = frame;
    }

    template <int Slot>
    void censusCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++g_censusCalls[Slot];
        if (Slot == 0) // modelDisp(instance, matrix): header = *(*instance + 0x20)
        {
            auto word = [rdram](uint32_t addr) { uint32_t v = 0; std::memcpy(&v, rdram + (addr & 0x1FFFFFCu), 4); return v; };
            const uint32_t header = word(word(GPR_U32(ctx, 4)) + 0x20u);
            g_censusModelPackets += word(header + 0x64u);
            for (uint32_t probe = (header >> 4) & 4095u;; probe = (probe + 1u) & 4095u)
            {
                if (g_censusModels[probe] == header)
                    break;
                if (g_censusModels[probe] == 0u)
                {
                    g_censusModels[probe] = header;
                    ++g_censusDistinct;
                    break;
                }
            }
            censusReport();
        }
        g_censusOriginal[Slot](rdram, ctx, runtime);
    }

    template <int... Slots>
    constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeCensus(std::integer_sequence<int, Slots...>)
    {
        return {&censusCall<Slots>...};
    }

    void installCensus(PS2Runtime &runtime)
    {
        if (!std::getenv("SOTC_CENSUS"))
            return;
        static const auto hooks = makeCensus(std::make_integer_sequence<int, kCensusCount>{});
        for (int i = 0; i < kCensusCount; ++i)
            if (runtime.hasFunction(kCensus[i].addr))
            {
                g_censusOriginal[i] = runtime.lookupFunction(kCensus[i].addr);
                runtime.registerFunction(kCensus[i].addr, hooks[i]);
            }
            else
                std::cerr << "[census] no function at 0x" << std::hex << kCensus[i].addr << std::dec << " (" << kCensus[i].name << ")" << std::endl;
    }

    // SOTC_MODEL_DUMP=<game frame>: in that frame, each modelDisp call
    // (instance, model header, model matrix, camera context and its matrix at
    // +0x5a0) goes to logs/model_dump/calls.txt, then guest RAM to
    // logs/model_dump/ram.bin: input for tools/model_view.py.
    constexpr uint32_t kModelDisp = 0x01097118u, kGetCameraContext = 0x0108F518u;
    PS2Runtime::RecompiledFunction g_modelDispOriginal = nullptr, g_getCameraOriginal = nullptr;
    uint32_t g_lastCameraContext = 0;
    std::string g_modelDumpCalls;
    int g_modelDumpState = 0; // 0 waiting, 1 recording, 2 done

    void getCameraContextHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_getCameraOriginal(rdram, ctx, runtime);
        g_lastCameraContext = GPR_U32(ctx, 2);
    }

    void modelDispDumpHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const uint64_t at = std::strtoull(std::getenv("SOTC_MODEL_DUMP"), nullptr, 10);
        const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        auto word = [rdram](uint32_t addr) { uint32_t v = 0; std::memcpy(&v, rdram + (addr & 0x1FFFFFCu), 4); return v; };
        auto floats = [rdram](uint32_t addr, int n)
        {
            std::string out;
            for (int i = 0; i < n; ++i)
            {
                float f = 0.0f;
                std::memcpy(&f, rdram + ((addr + i * 4u) & 0x1FFFFFCu), 4);
                char text[32];
                std::snprintf(text, sizeof(text), " %.9g", f);
                out += text;
            }
            return out;
        };
        if (g_modelDumpState == 0 && frame >= at)
            g_modelDumpState = 1;
        if (g_modelDumpState == 1 && frame > at + 1u)
        {
            std::system("mkdir -p logs/model_dump");
            if (FILE *f = std::fopen("logs/model_dump/calls.txt", "w"))
            {
                std::fputs(g_modelDumpCalls.c_str(), f);
                std::fclose(f);
            }
            if (FILE *f = std::fopen("logs/model_dump/ram.bin", "wb"))
            {
                std::fwrite(rdram, 1, 32u << 20, f);
                std::fclose(f);
            }
            std::cerr << "[sotc] model dump written (frame " << at << ")" << std::endl;
            g_modelDumpState = 2;
        }
        const uint32_t instance = GPR_U32(ctx, 4), matrix = GPR_U32(ctx, 5);
        g_modelDispOriginal(rdram, ctx, runtime);
        if (g_modelDumpState == 1 && frame == at + 1u)
        {
            const uint32_t header = word(word(instance) + 0x20u);
            char head[96];
            std::snprintf(head, sizeof(head), "call instance=%08x header=%08x camera=%08x", instance, header, g_lastCameraContext);
            g_modelDumpCalls += std::string(head) + " model" + floats(matrix, 16) + " camera" + floats(g_lastCameraContext + 0x5a0u, 16) + "\n";
        }
    }

    // Cloth and other grid meshes (meshCreate): meshDispInstance(instance)
    // and meshDisp(mesh) calls of the dumped frame, as "mesh" lines.
    constexpr uint32_t kMeshDispInstance = 0x0109A4D8u, kMeshDisp = 0x0109AB80u;
    PS2Runtime::RecompiledFunction g_meshDispInstanceOriginal = nullptr, g_meshDispOriginal = nullptr;

    template <PS2Runtime::RecompiledFunction *Original, char Kind>
    void meshDumpHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t arg = GPR_U32(ctx, 4);
        (*Original)(rdram, ctx, runtime);
        static const uint64_t at = std::strtoull(std::getenv("SOTC_MODEL_DUMP"), nullptr, 10);
        if (g_modelDumpState == 1 && ps2_stats::vif1Chains.load(std::memory_order_relaxed) == at + 1u)
        {
            char line[96];
            std::snprintf(line, sizeof(line), "mesh %s=%08x camera=%08x\n", Kind == 'i' ? "instance" : "mesh", arg, g_lastCameraContext);
            g_modelDumpCalls += line;
        }
    }

    // SOTC_MATH_CHECK=1: the game's math functions checked against the host's
    // on every call; mismatches (|error| > 1e-3) are counted by function and
    // caller and listed every 600 calls of the worst one.
    struct MathCheck
    {
        uint32_t addr;
        const char *name;
        int args;      // float arguments (f12, f13)
        double (*ref)(double, double);
        PS2Runtime::RecompiledFunction original;
    };
    MathCheck g_math[] = {
        {0x001BDF00u, "iosGetSinf", 1, [](double a, double) { return std::sin(a); }, nullptr},
        {0x001BE020u, "iosGetCosf", 1, [](double a, double) { return std::cos(a); }, nullptr},
        {0x001BE138u, "iosGetTanf", 1, [](double a, double) { return std::tan(a); }, nullptr},
        {0x001BE260u, "iosGetACosf", 1, [](double a, double) { return std::acos(std::clamp(a, -1.0, 1.0)); }, nullptr},
        {0x001BE388u, "iosGetACosfExact", 1, [](double a, double) { return std::acos(std::clamp(a, -1.0, 1.0)); }, nullptr},
        {0x001BE3E0u, "iosGetASinfExact", 1, [](double a, double) { return std::asin(std::clamp(a, -1.0, 1.0)); }, nullptr},
        {0x001BE440u, "iosGetATanfExact", 1, [](double a, double) { return std::atan(a); }, nullptr},
        {0x001DD538u, "atanf", 1, [](double a, double) { return std::atan(a); }, nullptr},
        {0x001DD7C8u, "cosf", 1, [](double a, double) { return std::cos(a); }, nullptr},
        {0x001DD8C8u, "sinf", 1, [](double a, double) { return std::sin(a); }, nullptr},
        {0x001DD9C8u, "acosf", 1, [](double a, double) { return std::acos(std::clamp(a, -1.0, 1.0)); }, nullptr},
        {0x001DD9E0u, "asinf", 1, [](double a, double) { return std::asin(std::clamp(a, -1.0, 1.0)); }, nullptr},
        {0x001DD9F8u, "atan2f", 2, [](double y, double x) { return std::atan2(y, x); }, nullptr},
        {0x001DDA40u, "sqrtf", 1, [](double a, double) { return a < 0 ? 0.0 : std::sqrt(a); }, nullptr},
    };
    constexpr int kMath = static_cast<int>(sizeof(g_math) / sizeof(g_math[0]));
    std::map<std::pair<int, uint32_t>, std::pair<uint64_t, double>> g_mathBad; // (fn, ra) -> (count, worst)
    std::array<uint64_t, 32> g_mathCalls{};

    template <int I>
    void mathCheckHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        MathCheck &m = g_math[I];
        const float a = ctx->f[12], b = ctx->f[13];
        const uint32_t ra = GPR_U32(ctx, 31);
        m.original(rdram, ctx, runtime);
        const double want = m.ref(a, b), got = ctx->f[0];
        ++g_mathCalls[I];
        if (std::isfinite(want) && !(std::fabs(got - want) <= 1e-3))
        {
            auto &e = g_mathBad[{I, ra}];
            ++e.first;
            if (!(std::fabs(got - want) <= std::fabs(e.second)))
            {
                e.second = got - want;
                std::fprintf(stderr, "[math] %s(%g%s%g) = %g, want %g (ra %08x)\n", m.name, a, m.args > 1 ? ", " : "", m.args > 1 ? b : 0.0f, got, want, ra);
            }
        }
        if (g_mathCalls[I] % 20000u == 0u)
        {
            std::string line = "[math] calls:";
            for (int k = 0; k < kMath; ++k)
                line += std::string(" ") + g_math[k].name + "=" + std::to_string(g_mathCalls[k]);
            line += " | bad:";
            for (auto &[key, v] : g_mathBad)
            {
                char item[96];
                std::snprintf(item, sizeof(item), " %s@%08x x%llu (worst %+.3g)", g_math[key.first].name, key.second, (unsigned long long)v.first, v.second);
                line += item;
            }
            std::fprintf(stderr, "%s\n", line.c_str());
        }
    }

    // GAMEUTIL_get_yaw_pitch_by_direction(yaw*, pitch*, dir*) (0x10816c0): the
    // angles it stores, checked against atan2 of the direction (the sword aims
    // through it). Its inner calls tail-jump, so only the stored result counts.
    constexpr uint32_t kYawPitchEnd = 0x010817C8u;
    PS2Runtime::RecompiledFunction g_yawPitch = nullptr, g_yawPitchEnd = nullptr;
    struct PendingYawPitch { uint32_t yaw, pitch; float x, y, z; uint32_t ra; };
    std::map<uint32_t, PendingYawPitch> g_pendingYawPitch; // by direction pointer

    void yawPitchHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto rf = [rdram](uint32_t a) { float v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
        const uint32_t pdir = GPR_U32(ctx, 6);
        g_pendingYawPitch[pdir] = {GPR_U32(ctx, 4), GPR_U32(ctx, 5), rf(pdir), rf(pdir + 4u), rf(pdir + 8u), GPR_U32(ctx, 31)};
        g_yawPitch(rdram, ctx, runtime);
    }

    // Its last call: by then the angles are stored.
    void yawPitchEndHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto rf = [rdram](uint32_t a) { float v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
        auto it = g_pendingYawPitch.find(GPR_U32(ctx, 4));
        if (it != g_pendingYawPitch.end())
        {
            const PendingYawPitch p = it->second;
            g_pendingYawPitch.erase(it);
            const double yaw = rf(p.yaw), pitch = rf(p.pitch);
            const double hyp = std::sqrt(double(p.x) * p.x + double(p.z) * p.z), len = std::sqrt(hyp * hyp + double(p.y) * p.y);
            if (len > 1e-6 && hyp > 1e-6)
            {
                const double wantYaw = std::atan2(double(p.x), double(p.z)), wantPitch = std::atan2(double(p.y), hyp);
                static uint64_t calls = 0, badYaw = 0, badPitch = 0;
                ++calls;
                const bool by = std::fabs(std::remainder(yaw - wantYaw, 2 * M_PI)) > 2e-3;
                const bool bp = std::fabs(pitch - wantPitch) > 2e-3;
                badYaw += by;
                badPitch += bp;
                static int printed = 0;
                if ((by || bp) && printed++ < 40)
                    std::fprintf(stderr, "[yawpitch] dir (%g %g %g) -> yaw %g (want %g) pitch %g (want %g) ra %08x\n", p.x, p.y, p.z, yaw, wantYaw, pitch, wantPitch, p.ra);
                if (calls % 5000u == 0u)
                    std::fprintf(stderr, "[yawpitch] calls %llu bad yaw %llu bad pitch %llu\n", (unsigned long long)calls, (unsigned long long)badYaw, (unsigned long long)badPitch);
            }
        }
        g_yawPitchEnd(rdram, ctx, runtime);
    }

    // doit_aim_angle_interp_cntrl (0x10848b8): its arguments, to see the
    // time step and rates the aim interpolation runs with.
    PS2Runtime::RecompiledFunction g_aimInterp = nullptr;
    void aimInterpHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static uint64_t n = 0;
        if (n++ % 15u == 0u && n < 4000u)
        {
            auto rf = [rdram](uint32_t a) { float v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
            auto rw = [rdram](uint32_t a) { uint32_t v; std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4); return v; };
            const uint32_t st = GPR_U32(ctx, 4);
            std::fprintf(stderr, "[aim] a0 %08x a1 %08x a2 %08x a3 %08x f12 %g f13 %g f14 %g f15 %g | st[0]=%u rate %g max %g cur %g ra %08x\n",
                         GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15],
                         rw(st), rf(st + 4u), rf(st + 8u), rf(st + 12u), GPR_U32(ctx, 31));
        }
        g_aimInterp(rdram, ctx, runtime);
    }

    template <int... I>
    void installMathCheck(PS2Runtime &runtime, std::integer_sequence<int, I...>)
    {
        const PS2Runtime::RecompiledFunction hooks[] = {mathCheckHook<I>...};
        for (int i = 0; i < kMath; ++i)
            if (runtime.hasFunction(g_math[i].addr))
            {
                g_math[i].original = runtime.lookupFunction(g_math[i].addr);
                runtime.registerFunction(g_math[i].addr, hooks[i]);
            }
    }

    // SOTC_DUMP_SPRITE_PACKETS=1: dump the display-list bytes spriteDrawSprite emits.
    constexpr uint32_t kSpriteDrawSprite = 0x010A32B8u;
    constexpr uint32_t kDlAllocPtr = 0x002132E8u; // iosAllocPacketPtrPath1UnCachedA cursor
    PS2Runtime::RecompiledFunction g_spriteDrawSprite = nullptr;

    void spriteDrawSpriteHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        uint32_t before = 0;
        std::memcpy(&before, rdram + kDlAllocPtr, 4);
        const uint32_t obj = GPR_U32(ctx, 4);
        static int logged = 0;
        const bool logWrites = logged++ < 2;
        if (logWrites)
        {
            g_ps2WatchWriteEnabled = true;
            g_ps2LogAllWrites = true;
            std::fprintf(stderr, "[spritepkt] begin obj=0x%08x\n", obj);
        }
        g_spriteDrawSprite(rdram, ctx, runtime);
        if (logWrites)
        {
            g_ps2LogAllWrites = false;
            g_ps2WatchWriteEnabled = std::getenv("PS2X_WATCH_WRITE") != nullptr;
            std::fprintf(stderr, "[spritepkt] end\n");
        }
        uint32_t after = 0;
        std::memcpy(&after, rdram + kDlAllocPtr, 4);
        static int dumped = 0;
        if (dumped < 12)
        {
            ++dumped;
            std::fprintf(stderr, "[spritepkt] obj=0x%08x alloc 0x%08x -> 0x%08x (%d bytes)\n", obj, before, after, static_cast<int>(after - before));
            for (uint32_t a = before; a < after && a < before + 0x200u; a += 16)
            {
                uint32_t w[4];
                std::memcpy(w, rdram + (a & 0x1FFFFFFu), 16);
                std::fprintf(stderr, "   %08x: %08x %08x %08x %08x\n", a, w[0], w[1], w[2], w[3]);
            }
        }
    }

    // KERNEL 0x1ad270 is SotC's idle thread: it loops iosRecvMsg(0x1fc870,
    // 0, 0) (a poll whose result it drops; iosWaitIdleThread's message) and
    // reads the CD clock every 1e6 turns (the queue has no buffer: the poll
    // returns 0 either way). After each poll, when no other thread can run
    // (a sender it woke is ready), wait on the host for an event instead of
    // spinning: on the Vita the spin took a core from VU1 and GS.
    constexpr uint32_t kIosRecvMsg = 0x001B1138u;
    constexpr uint32_t kIdleQueue = 0x001FC870u;
    PS2Runtime::RecompiledFunction g_iosRecvMsg = nullptr;

    void iosRecvMsgHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool idlePoll = GPR_U32(ctx, 4) == kIdleQueue && GPR_U32(ctx, 6) == 0u;
        g_iosRecvMsg(rdram, ctx, runtime);
        if (idlePoll)
            runtime->eeScheduler().idleWait();
    }

    // SotC's frame loop: the main thread blocks sending to queue 0x26d9a8;
    // its GP thread, at each VBlank once the GS has finished the previous
    // frame, calls 0x1c8078, which receives that message and lets the next
    // frame start. Nothing caps the rate (60, 30, 20... as drawing allows).
    // SOTC_FRAME_INTERVAL=<n>: release a frame only n VBlanks after the
    // previous one at the earliest (2 = steady 30 fps).
    constexpr uint32_t kGpReleaseFrame = 0x001C8078u;
    PS2Runtime::RecompiledFunction g_gpReleaseFrame = nullptr;

    void gpReleaseFrameHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const uint64_t interval = std::strtoull(std::getenv("SOTC_FRAME_INTERVAL"), nullptr, 10);
        static uint64_t lastRelease = 0;
        const uint64_t tick = runtime->eeScheduler().currentVSyncTick();
        if (tick < lastRelease + interval)
            return; // the GP thread calls again at the next VBlank
        lastRelease = tick;
        g_gpReleaseFrame(rdram, ctx, runtime);
    }

    // SOTC_LOG_SG2=1: which commands the EE's sound engine sends its IOP
    // driver: Sg2RemoteSend (0x1d2010) puts cmd, a, b, c in a ring (head
    // 0x287500, tail 0x287504, 512 x 16 bytes at 0x287540) that the tick
    // (Sg2RemoteTickProc 0x1d21e8, called through a pointer) sends by RPC
    // 0x11223344; counted here before it goes.
    PS2Runtime::RecompiledFunction g_sg2RemoteTick = nullptr;
    void sg2RemoteTickHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static std::map<uint32_t, uint64_t> counts;
        static std::map<uint32_t, std::array<uint32_t, 3>> example;
        static uint64_t total = 0, ticks = 0;
        auto w = [&](uint32_t a) { uint32_t v; std::memcpy(&v, rdram + a, 4); return v; };
        const uint32_t head = w(0x287500u), tail = w(0x287504u);
        for (uint32_t i = tail; i != head; i = (i + 1u) & 0x1FFu)
        {
            const uint32_t a = 0x287540u + i * 16u;
            ++counts[w(a)];
            example[w(a)] = {w(a + 4u), w(a + 8u), w(a + 12u)};
            ++total;
        }
        if (++ticks % 1500u == 0u)
        {
            std::fprintf(stderr, "[sg2] ticks %llu commands %llu:", (unsigned long long)ticks, (unsigned long long)total);
            for (const auto &[c, n] : counts)
                std::fprintf(stderr, " %x:%llu(%x,%x,%x)", c, (unsigned long long)n, example[c][0], example[c][1], example[c][2]);
            std::fprintf(stderr, "\n");
        }
        g_sg2RemoteTick(rdram, ctx, runtime);
    }

    // SOTC_LOG_MALLOC=1: the boot's heap calls (iosMallocAlign, ...NoCheck,
    // iosFree) until GAMECORE runs, in order: where module placement comes
    // from. GAMECORE's image follows the main thread's letter post (0x14
    // bytes from init_letter_manager, ra 0x1c8828) or not depending on when
    // that thread first runs: boot timing moves the module by 0x40.
    //
    // So the layout never depends on timing, the letter post waits (a VBlank
    // at a time, the call made again) until GAMECORE's image is allocated:
    // the order of a boot with the IOP's timing, which the recompiled modules
    // were captured with. Without it the boot needs no IOP code at all.
    PS2Runtime::RecompiledFunction g_mallocAlign = nullptr, g_mallocNoCheck = nullptr, g_free = nullptr;
    bool g_gamecoreImageAllocated = false;
    template <int Kind>
    void heapHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6), ra = GPR_U32(ctx, 31);
        if (Kind == 0 && !g_gamecoreEntered && ra == 0x001C8828u && !g_gamecoreImageAllocated)
            runtime->eeWaitVSyncTicks(1u, 0x001AFFD8u); // the letter post: after GAMECORE's image
        (Kind == 0 ? g_mallocAlign : Kind == 1 ? g_mallocNoCheck : g_free)(rdram, ctx, runtime);
        if (Kind == 0 && ra == 0x001B5558u && a1 == 0x0020AEA8u)
            g_gamecoreImageAllocated = true;
        static const bool log = std::getenv("SOTC_LOG_MALLOC") != nullptr;
        if (log && !g_gamecoreEntered)
            std::fprintf(stderr, "[heap] %s heap=%x size=%x align=%x ra=%x\n", Kind == 0 ? "malloc" : Kind == 1 ? "mallocNC" : "free",
                         a0, a1, a2, ra);
    }

    // ---- Native system (SOTC_NATIVE): the game library's hardware access
    // done on the host. The game no longer reads or writes a PS2 register
    // once GAMECORE runs (PS2X_MMIO_CENSUS=1 counts what is left):
    //  - T0 / T1: iosInitIntrSystem arms T0 (bus clock, overflow interrupt
    //    every 65536 ticks, ~2300 a second, only to extend the count) and T1
    //    (profiling, never read). Both are stopped after it; the 64-bit count
    //    iosGetTCountExtension / iosGetTCount return comes from the EE clock
    //    (bus = EE / 2, as the timer model counted it).
    //  - Scratchpad DMA (iosMemToSprDmaAdrCopy, iosSprToMemDmaAdrCopy and
    //    the chunked copy 0x1ad958): plain copies; the DMA was synchronous.
    //  - iosDmaSendSChainPath1 (the display list, VIF1 chain) and
    //    iosCopyFrameBuffer (a GIF packet): straight into the host DMA engine
    //    once the paths are idle (otherwise the game's own wait runs).
    //  - iosCheckDrawFinish: the host's "all paths idle".
    //  - libkernel's timer system on T2 (StartTimerSystemTime, the system
    //    time, its interrupt) and DelayThread, its one user: the EE clock
    //    and a timed wait on the host scheduler; T2 stays stopped.
    namespace sysnative
    {
        PS2Runtime::RecompiledFunction g_initIntr = nullptr, g_dmaSendPath1 = nullptr, g_copyFrameBuffer = nullptr;

        void returnTo(R5900Context *ctx) { ctx->pc = GPR_U32(ctx, 31); }
        uint64_t busTicks(PS2Runtime *runtime) { return runtime->eeScheduler().eeCycle() / 2u; }
        uint32_t u32(uint8_t *rdram, uint32_t address)
        {
            uint32_t v;
            std::memcpy(&v, getMemPtr(rdram, address), 4);
            return v;
        }
        void put32(uint8_t *rdram, uint32_t address, uint32_t v) { std::memcpy(getMemPtr(rdram, address), &v, 4); }
        void put64(uint8_t *rdram, uint32_t address, uint64_t v) { std::memcpy(getMemPtr(rdram, address), &v, 8); }

        void initIntrSystem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            g_initIntr(rdram, ctx, runtime);
            runtime->memory().hostStopEeTimer(0);
            runtime->memory().hostStopEeTimer(1);
        }
        void tcountOverflow(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            // T0's overflow handler (0x1ad850): T0 is stopped, nothing to extend or re-arm.
            runtime->memory().hostStopEeTimer(0);
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
        }
        void delayThread(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            // libkernel's DelayThread (an alarm on T2, then SleepThread): a timed wait on the host.
            runtime->eeScheduler().delayCurrent(GPR_U32(ctx, 4), [](R5900Context &resume)
            {
                R5900Context *r = &resume;
                SET_GPR_U64(r, 2, 0u);
                r->pc = GPR_U32(r, 31);
            });
        }
        void timerSystemTime(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            SET_GPR_U64(ctx, 2, busTicks(runtime)); // (i)GetTimerSystemTime: bus clock ticks
            returnTo(ctx);
        }
        void startTimerSystemTime(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            runtime->memory().hostStopEeTimer(2); // the system time comes from the EE clock
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
        }
        void timerInterrupt(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            runtime->memory().hostStopEeTimer(2); // T2's handler (0x1109d0): no alarms on T2 any more
            SET_GPR_U64(ctx, 2, 0u);
            returnTo(ctx);
        }
        void getTCountExtension(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            SET_GPR_U64(ctx, 2, busTicks(runtime));
            returnTo(ctx);
        }
        void getTCount(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            SET_GPR_U64(ctx, 2, busTicks(runtime) & ~0xFFFFull);
            returnTo(ctx);
        }
        // T1, the "CPU rap counter" (profiling): counted from the EE clock
        // since its last reset, at the clock its mode selects, 16 bits.
        uint64_t g_rapBase = 0;
        void resetCpuRapCounter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            put32(rdram, 0x001E0BD4u, GPR_U32(ctx, 4)); // the mode its readers scale by
            g_rapBase = runtime->eeScheduler().eeCycle();
            returnTo(ctx);
        }
        void getCpuRapCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            static constexpr double kClockHz[4] = {147456000.0, 9216000.0, 576000.0, 15734.0};
            const uint32_t mode = u32(rdram, 0x001E0BD4u);
            const double seconds = static_cast<double>(runtime->eeScheduler().eeCycle() - g_rapBase) / 294912000.0;
            const uint32_t count = static_cast<uint32_t>(seconds * kClockHz[mode & 3u]) & 0xFFFFu;
            float scale;
            std::memcpy(&scale, getMemPtr(rdram, 0x001E0BD8u + mode * 4u), 4);
            ctx->f[0] = static_cast<float>(count) * 60.0f / scale; // iosGetCpuRapCountPar1Int
            returnTo(ctx);
        }
        void chkGpDlTransExec(uint8_t *, R5900Context *ctx, PS2Runtime *runtime)
        {
            const bool busy = runtime->memory().hostChannelBusy(0x10009000u) || runtime->memory().hostChannelBusy(0x1000A000u);
            SET_GPR_U64(ctx, 2, busy ? 1u : 0u);
            returnTo(ctx);
        }
        void memToSpr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const uint32_t madr = GPR_U32(ctx, 4), bytes = (GPR_U32(ctx, 5) & 0xFFFFu) * 16u, sadr = GPR_U32(ctx, 6) & 0x3FF0u;
            std::memcpy(runtime->memory().getScratchpad() + sadr, getMemPtr(rdram, madr), std::min(bytes, 0x4000u - sadr));
            returnTo(ctx);
        }
        void sprToMem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const uint32_t madr = GPR_U32(ctx, 4), bytes = (GPR_U32(ctx, 5) & 0xFFFFu) * 16u, sadr = GPR_U32(ctx, 6) & 0x3FF0u;
            std::memcpy(getMemPtr(rdram, madr), runtime->memory().getScratchpad() + sadr, std::min(bytes, 0x4000u - sadr));
            returnTo(ctx);
        }
        void copyViaSpr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            // (dst, src, bytes): 16 KB at a time through the scratchpad.
            uint32_t dst = GPR_U32(ctx, 4), src = GPR_U32(ctx, 5), qwc = GPR_U32(ctx, 6) >> 4;
            ++sotc_decomp::g_guestMoveSerial; // the native hooks' memos sample again
            uint8_t *spr = runtime->memory().getScratchpad();
            // The heap's garbage pass moves blocks down with it (several MB a
            // frame at an area change): one memmove when the chunks come to
            // the same (the destination below the source, or apart), the
            // scratchpad left as the chunks leave it (the last chunk, the one
            // before it past that). The source is read before any write: a
            // chunk's source is never one an earlier chunk's write reached
            // then. SOTC_SPR_COPY_CHUNKED=1: the chunks (checking).
            static const bool chunked = std::getenv("SOTC_SPR_COPY_CHUNKED") != nullptr;
            const uint32_t total = qwc * 16u, srcOff = src & PS2_RAM_MASK, dstOff = dst & PS2_RAM_MASK;
            if (!chunked && qwc > 0u && qwc < (PS2_RAM_SIZE >> 4) && srcOff + total <= PS2_RAM_SIZE && dstOff + total <= PS2_RAM_SIZE &&
                (dstOff <= srcOff || dstOff >= srcOff + total))
            {
                const uint32_t chunks = (qwc + 0x3FFu) / 0x400u, last = (qwc - (chunks - 1u) * 0x400u) * 16u;
                // SOTC_SPR_COPY_CHECK=1 (checking): the chunks on a copy of the range and the scratchpad, compared.
                static const bool check = std::getenv("SOTC_SPR_COPY_CHECK") != nullptr;
                const uint32_t lo = std::min(srcOff, dstOff), hi = std::max(srcOff, dstOff) + total;
                std::vector<uint8_t> ram, pad;
                if (check)
                {
                    ram.assign(rdram + lo, rdram + hi);
                    pad.assign(spr, spr + 0x4000u);
                    for (uint32_t q = qwc, s = srcOff - lo, d = dstOff - lo;; q -= 0x400u, s += 0x4000u, d += 0x4000u)
                    {
                        const uint32_t n = std::min(q, 0x400u) * 16u;
                        std::memcpy(pad.data(), ram.data() + s, n);
                        std::memcpy(ram.data() + d, pad.data(), n);
                        if (q <= 0x400u)
                            break;
                    }
                }
                const uint8_t *from = rdram + srcOff;
                if (chunks > 1u)
                    std::memcpy(spr + last, from + (chunks - 2u) * 0x4000u + last, 0x4000u - last);
                std::memcpy(spr, from + (chunks - 1u) * 0x4000u, last);
                std::memmove(rdram + dstOff, from, total);
                sotc_decomp::noteGuestMove(srcOff, dstOff, total);
                if (check)
                {
                    static uint64_t checks = 0, bad = 0;
                    ++checks;
                    if (std::memcmp(ram.data(), rdram + lo, hi - lo) != 0 || std::memcmp(pad.data(), spr, 0x4000u) != 0)
                        ++bad;
                    if ((checks & 1023u) == 0u || bad == 1u)
                        std::fprintf(stderr, "[spr-copy] %llu checks, %llu differ\n", (unsigned long long)checks, (unsigned long long)bad);
                }
                return returnTo(ctx);
            }
            while (true)
            {
                const uint32_t n = std::min(qwc, 0x400u);
                std::memcpy(spr, getMemPtr(rdram, src), n * 16u);
                std::memcpy(getMemPtr(rdram, dst), spr, n * 16u);
                if (qwc <= 0x400u)
                    break;
                qwc -= 0x400u;
                src += 0x4000u;
                dst += 0x4000u;
            }
            returnTo(ctx);
        }
        void dmaSendPath1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (!runtime->memory().hostDrawPathsIdle())
            {
                g_dmaSendPath1(rdram, ctx, runtime); // the game's own wait
                return;
            }
            // Completions already pending go to their handlers first, as the
            // game's stores before the kick would have delivered them.
            runtime->drainCompletedDmacHandlers(rdram);
            runtime->memory().hostDmaStart(0x10009000u, 0x145u, 0u, 0u, GPR_U32(ctx, 4) & 0x7FFFFFFFu);
            runtime->drainCompletedDmacHandlers(rdram);
            returnTo(ctx);
        }
        void checkDrawFinish(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            put32(rdram, 0x001E10ECu, u32(rdram, 0x001E10ECu) + 1u);
            SET_GPR_U64(ctx, 2, runtime->memory().hostDrawPathsIdle() ? 1u : 0u);
            returnTo(ctx);
        }
        void copyFrameBuffer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (!runtime->memory().hostDrawPathsIdle())
            {
                g_copyFrameBuffer(rdram, ctx, runtime);
                return;
            }
            // The packet iosCopyFrameBuffer (0x1c19c0) builds at 0x213710: the
            // draw buffer copied (stretched when the sizes differ) to the display one.
            const int32_t dw = static_cast<int32_t>(u32(rdram, 0x00213810u)), dh = static_cast<int32_t>(u32(rdram, 0x00213814u));
            const int32_t sw = static_cast<int32_t>(u32(rdram, 0x00213818u)), sh = static_cast<int32_t>(u32(rdram, 0x0021381Cu));
            auto log2Up = [](int32_t v) -> int64_t
            {
                for (int64_t k = 0; k < 11; ++k)
                    if (v <= (1 << k))
                        return k;
                return -1;
            };
            const int64_t th = log2Up(sh), tw = log2Up(sw);
            const bool same = dw == sw && dh == sh;
            put64(rdram, 0x00213710u, 0x100000000000800Full);
            put64(rdram, 0x00213718u, 0xEu);
            put64(rdram, 0x00213720u, ((static_cast<uint64_t>(dw >> 6) & 0x3Fu) << 16) | 0x1000000u);
            put64(rdram, 0x00213728u, 0x4Cu);
            put64(rdram, 0x00213730u, static_cast<uint64_t>(static_cast<int64_t>(dw * dh >> 11)) | 0x131000000ull);
            put64(rdram, 0x00213738u, 0x4Eu);
            put64(rdram, 0x00213740u, 0x30000u);
            put64(rdram, 0x00213748u, 0x47u);
            put64(rdram, 0x00213750u, 0u);
            put64(rdram, 0x00213758u, 8u);
            put64(rdram, 0x00213760u, (static_cast<uint64_t>(dw - 1) << 16) | (static_cast<uint64_t>(dh - 1) << 48));
            put64(rdram, 0x00213768u, 0x40u);
            put64(rdram, 0x00213770u, static_cast<uint64_t>(static_cast<int64_t>((0x800 - dw / 2) * 0x10)) |
                                          (static_cast<uint64_t>(static_cast<int64_t>((0x800 - dh / 2) * 0x10)) << 32));
            put64(rdram, 0x00213778u, 0x18u);
            put64(rdram, 0x00213780u, 1u);
            put64(rdram, 0x00213788u, 0x3Fu);
            put64(rdram, 0x00213790u, static_cast<uint64_t>(static_cast<int64_t>((dw * dh >> 6) + (sw * sh >> 6))) | (static_cast<uint64_t>(th) << 30) |
                                          (static_cast<uint64_t>(sw >> 6) << 14) | 0x100000u | (static_cast<uint64_t>(tw) << 26));
            put64(rdram, 0x00213798u, 6u);
            put64(rdram, 0x002137A0u, same ? 0u : 0x60u);
            put64(rdram, 0x002137A8u, 0x14u);
            put64(rdram, 0x002137B0u, 0x3F80000080808080ull);
            put64(rdram, 0x002137B8u, 1u);
            put64(rdram, 0x002137C0u, 0x116u);
            put64(rdram, 0x002137C8u, 0u);
            const int32_t hh = dh >> 1, hw = dw >> 1;
            uint64_t a, b;
            if (same)
            {
                a = static_cast<uint64_t>((0x800 - hw) * 0x10) | (static_cast<uint64_t>((0x800 - hh) * 0x10) << 16);
                b = static_cast<uint64_t>((hw + 0x800) * 0x10) | (static_cast<uint64_t>((hh + 0x800) * 0x10) << 16);
                put64(rdram, 0x002137F0u, static_cast<uint64_t>(dw << 4) | (static_cast<uint64_t>(dh << 4) << 16));
                put64(rdram, 0x002137D0u, 0u);
            }
            else
            {
                a = static_cast<uint64_t>((0x800 - hw) * 0x10 - 8) | (static_cast<uint64_t>((0x800 - hh) * 0x10 - 8) << 16);
                b = static_cast<uint64_t>(hw * 0x10 + 0x7FF8) | (static_cast<uint64_t>(hh * 0x10 + 0x7FF8) << 16);
                put64(rdram, 0x002137F0u, static_cast<uint64_t>(sw * 0x10 - 0x18) | (static_cast<uint64_t>(sh * 0x10 - 0x18) << 16));
                put64(rdram, 0x002137D0u, 0x80008u);
            }
            put64(rdram, 0x00213800u, b | 0xFFFFFFFF00000000ull);
            put64(rdram, 0x002137E0u, a | 0xFFFFFFFF00000000ull);
            put64(rdram, 0x00213808u, 4u);
            put64(rdram, 0x002137F8u, 3u);
            put64(rdram, 0x002137E8u, 4u);
            put64(rdram, 0x002137D8u, 3u);
            runtime->drainCompletedDmacHandlers(rdram);
            runtime->memory().hostDmaStart(0x1000A000u, 0x141u, 0x00213710u, 0x10u, 0u);
            runtime->drainCompletedDmacHandlers(rdram);
            returnTo(ctx);
        }

        void install(PS2Runtime &runtime)
        {
            const char *native = std::getenv("SOTC_NATIVE");
            if (!native || std::atoi(native) == 0 || std::getenv("SOTC_SYSTEM_EMULATED"))
                return;
            struct Hook
            {
                uint32_t address;
                PS2Runtime::RecompiledFunction fn;
                PS2Runtime::RecompiledFunction *original;
            };
            const Hook hooks[] = {
                {0x001AD460u, initIntrSystem, &g_initIntr},
                {0x001AD850u, tcountOverflow, nullptr},
                {0x00107A08u, delayThread, nullptr},
                {0x00110670u, startTimerSystemTime, nullptr},
                {0x00110C78u, timerSystemTime, nullptr},
                {0x00110CC8u, timerSystemTime, nullptr},
                {0x001109D0u, timerInterrupt, nullptr},
                {0x001AD528u, getTCountExtension, nullptr},
                {0x001AD510u, getTCount, nullptr},
                {0x001AD628u, resetCpuRapCounter, nullptr},
                {0x001AD658u, getCpuRapCount, nullptr},
                {0x001C35E8u, chkGpDlTransExec, nullptr},
                {0x001B2A18u, memToSpr, nullptr},
                {0x001B2A80u, sprToMem, nullptr},
                {0x001AD958u, copyViaSpr, nullptr},
                {0x001C2570u, dmaSendPath1, &g_dmaSendPath1},
                {0x001C2A78u, checkDrawFinish, nullptr},
                {0x001C19C0u, copyFrameBuffer, &g_copyFrameBuffer},
            };
            const char *skip = std::getenv("SOTC_SYSTEM_SKIP"); // debugging: "0x1c2570,0x1c2a78" left to the game
            for (const Hook &h : hooks)
            {
                if (!runtime.hasFunction(h.address))
                    continue;
                char tag[16];
                std::snprintf(tag, sizeof(tag), "0x%x", h.address);
                if (skip && std::strstr(skip, tag))
                    continue;
                if (h.original)
                    *h.original = runtime.lookupFunction(h.address);
                runtime.registerFunction(h.address, h.fn);
            }
            std::cerr << "[sotc] system library on the host (timers, scratchpad DMA, display list kicks)" << std::endl;
        }
    }

    void applySotcOverrides(PS2Runtime &runtime)
    {
        // First: the hooks below wrap whatever is in the table, lean or not.
        ps2_lean::install(runtime);
        if (runtime.hasFunction(0x001AFFD8u) && runtime.hasFunction(0x001B00F0u) && runtime.hasFunction(0x001B0328u))
        {
            g_mallocAlign = runtime.lookupFunction(0x001AFFD8u);
            runtime.registerFunction(0x001AFFD8u, heapHook<0>);
            g_mallocNoCheck = runtime.lookupFunction(0x001B00F0u);
            runtime.registerFunction(0x001B00F0u, heapHook<1>);
            g_free = runtime.lookupFunction(0x001B0328u);
            runtime.registerFunction(0x001B0328u, heapHook<2>);
        }
        if (std::getenv("SOTC_LOG_SG2") && runtime.hasFunction(0x001D21E8u))
        {
            g_sg2RemoteTick = runtime.lookupFunction(0x001D21E8u);
            runtime.registerFunction(0x001D21E8u, sg2RemoteTickHook);
        }
        if (std::getenv("SOTC_FRAME_INTERVAL") && runtime.hasFunction(kGpReleaseFrame))
        {
            g_gpReleaseFrame = runtime.lookupFunction(kGpReleaseFrame);
            runtime.registerFunction(kGpReleaseFrame, gpReleaseFrameHook);
        }
        if (!std::getenv("SOTC_IDLE_SPIN") && runtime.hasFunction(kIosRecvMsg))
        {
            g_iosRecvMsg = runtime.lookupFunction(kIosRecvMsg);
            runtime.registerFunction(kIosRecvMsg, iosRecvMsgHook);
        }
        if (std::getenv("SOTC_DUMP_SPRITE_PACKETS") && runtime.hasFunction(kSpriteDrawSprite))
        {
            g_spriteDrawSprite = runtime.lookupFunction(kSpriteDrawSprite);
            runtime.registerFunction(kSpriteDrawSprite, spriteDrawSpriteHook);
        }
        installCallTracing(runtime);
        installCensus(runtime);
        installSotcNative(runtime);
        sysnative::install(runtime);
        sotc_decomp::install(runtime);
        if (std::getenv("SOTC_MATH_CHECK") && runtime.hasFunction(0x010816C0u))
        {
            g_yawPitch = runtime.lookupFunction(0x010816C0u);
            runtime.registerFunction(0x010816C0u, yawPitchHook);
            g_aimInterp = runtime.lookupFunction(0x010848B8u);
            runtime.registerFunction(0x010848B8u, aimInterpHook);
            g_yawPitchEnd = runtime.lookupFunction(kYawPitchEnd);
            runtime.registerFunction(kYawPitchEnd, yawPitchEndHook);
        }
        if (std::getenv("SOTC_MODEL_DUMP") && runtime.hasFunction(kModelDisp) && runtime.hasFunction(kGetCameraContext))
        {
            g_modelDispOriginal = runtime.lookupFunction(kModelDisp);
            runtime.registerFunction(kModelDisp, modelDispDumpHook);
            g_getCameraOriginal = runtime.lookupFunction(kGetCameraContext);
            runtime.registerFunction(kGetCameraContext, getCameraContextHook);
            if (runtime.hasFunction(kMeshDispInstance) && runtime.hasFunction(kMeshDisp))
            {
                g_meshDispInstanceOriginal = runtime.lookupFunction(kMeshDispInstance);
                runtime.registerFunction(kMeshDispInstance, meshDumpHook<&g_meshDispInstanceOriginal, 'i'>);
                g_meshDispOriginal = runtime.lookupFunction(kMeshDisp);
                runtime.registerFunction(kMeshDisp, meshDumpHook<&g_meshDispOriginal, 'm'>);
            }
        }
        if (runtime.hasFunction(kGamecoreEntry))
        {
            g_gamecoreEntry = runtime.lookupFunction(kGamecoreEntry);
            runtime.registerFunction(kGamecoreEntry, gamecoreEntryHook);
        }
        ps2_lean::afterOverrides(runtime);
        // Last: it wraps whatever is in the table by now (SOTC_SUBSYS_PROF).
        sotc_prof::install(runtime);
    }
}

PS2_REGISTER_GAME_OVERRIDE("Shadow of the Colossus (USA)", "SCUS_974.72", 0u, 0u, applySotcOverrides)
