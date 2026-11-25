// Round P2 (docs/decomp_p2.md): counter brackets (SOTC_P2BRACKET=1, off by default; the Vita with SOTC_K2=1 only).
// Top-level guest functions of each subsystem wrapped in the function table (calls through it: jal / jalr / callGuest;
// natives calling natives directly are inside their caller's bracket). On entry and exit the game thread's six K2
// counters + cycles are read (src/lib/ps2_vita_p2.cpp: ps2x_p2b_enter / leave, a K2 syscall each); inclusive and self
// (minus the brackets opened inside) per frame, every window, as "[p2b] w ..." lines. SOTC_P2BRACKET_EXTRA=0xaddr:name,...
// adds functions. With the switch off nothing is wrapped (one getenv at install).
#include "sotc_p2.h"

#include "ps2_runtime.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#if defined(__vita__)
extern "C" bool ps2x_p2b_wanted();
extern "C" int ps2x_p2b_register(const char *name);
extern "C" uint32_t ps2x_p2b_enter(int id);
extern "C" void ps2x_p2b_leave(uint32_t token);

namespace sotc_p2
{
    namespace
    {
        struct Wrapped
        {
            uint32_t address;
            PS2Runtime::RecompiledFunction inner;
            int id;
        };
        constexpr int kMax = 48;
        std::array<Wrapped, kMax> g_w;

        struct Bracket
        {
            uint32_t token;
            explicit Bracket(int id) : token(ps2x_p2b_enter(id)) {}
            ~Bracket() { ps2x_p2b_leave(token); } // also when a guest unwind passes through
            Bracket(const Bracket &) = delete;
            Bracket &operator=(const Bracket &) = delete;
        };

        template <int Slot>
        void wrapped(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const Wrapped &w = g_w[Slot];
            const Bracket b(w.id);
            w.inner(rdram, ctx, runtime);
        }
        template <int... Slots>
        constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeWrappers(std::integer_sequence<int, Slots...>)
        {
            return {&wrapped<Slots>...};
        }

        struct Target
        {
            uint32_t address;
            const char *name;
        };
        // Per subsystem the entry points that run a few to a few hundred times a frame (docs/console_profile_2026-11-20.md
        // section 3; addresses from sotc_prof.cpp's list). Names carry the subsystem for p2_report.py.
        constexpr Target kTargets[] = {
            {0x010F52C8u, "display:DispDispObjGroupModel"},
            {0x01097118u, "display:modelDisp"},
            {0x0109E0E8u, "display:shadowDisp"},
            {0x010F26D0u, "display:UpdateLayoutClassGeometry"},
            {0x001C0058u, "display:iosDlTrans"},
            {0x010BF8F0u, "characters:ExecCharaGeometry"},
            {0x010B83D8u, "characters:GetBlendedMotion"},
            {0x010B1D60u, "characters:GetMotionMatrix"},
            {0x011370E8u, "characters:clothStep"},
            {0x010CE558u, "characters:charaLinks"},
            {0x010CBF78u, "characters:CharaCtrlAIControl"},
            {0x010CBF08u, "characters:CharaCtrlPlayableControl"},
            {0x0111B9F0u, "collision:ClipCollisionDB"},
            {0x0112D590u, "collision:ClipCollisionDirectCallBack"},
            {0x0112ED58u, "collision:ClipCollisionDirect"},
            {0x0141D280u, "camera:doit_cameraman_manager"},
            {0x01102930u, "scripts:execScript"},
            {0x0110D0F8u, "scripts:ExecScriptLayoutObj"},
            {0x0113A340u, "particles:doit_particle_manager"},
            {0x0113CA58u, "particles:particleUpdate"},
            {0x0113E940u, "particles:particleDraw"},
            {0x011462A0u, "particles:doit_wind_manager"},
        };
    }

    void install(PS2Runtime &runtime)
    {
        if (!ps2x_p2b_wanted())
            return;
        static const auto wrappers = makeWrappers(std::make_integer_sequence<int, kMax>{});
        int slot = 0;
        auto wrap = [&](uint32_t address, const char *name)
        {
            if (slot >= kMax || !runtime.hasFunction(address))
            {
                std::fprintf(stderr, "[p2b] not wrapped: %s @%08x\n", name, address);
                return;
            }
            for (int i = 0; i < slot; ++i)
                if (g_w[i].address == address)
                    return;
            const int id = ps2x_p2b_register(name);
            if (id < 0)
                return;
            g_w[slot] = Wrapped{address, runtime.lookupFunction(address), id};
            runtime.registerFunction(address, wrappers[slot]);
            ++slot;
        };
        for (const Target &t : kTargets)
            wrap(t.address, t.name);
        static std::vector<std::string> extraNames;
        if (const char *extra = std::getenv("SOTC_P2BRACKET_EXTRA"))
        {
            const std::string list(extra);
            size_t at = 0;
            extraNames.reserve(32);
            while (at < list.size())
            {
                size_t end = list.find(',', at);
                if (end == std::string::npos)
                    end = list.size();
                const std::string item = list.substr(at, end - at);
                at = end + 1;
                const size_t colon = item.find(':');
                const uint32_t address = static_cast<uint32_t>(std::strtoul(item.substr(0, colon).c_str(), nullptr, 16));
                if (extraNames.size() < 32)
                {
                    extraNames.push_back(colon == std::string::npos ? "extra:" + item : "extra:" + item.substr(colon + 1));
                    if (address)
                        wrap(address, extraNames.back().c_str());
                }
            }
        }
        std::fprintf(stderr, "[p2b] %d functions wrapped (counters read when K2's counters run on the game thread)\n", slot);
    }
}
#else
namespace sotc_p2
{
    void install(PS2Runtime &) {}
}
#endif
