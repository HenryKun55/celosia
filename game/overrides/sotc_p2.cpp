// Round P2 (docs/decomp_p2.md): counter brackets (SOTC_P2BRACKET=1, off by default; the Vita with SOTC_K2=1 only).
// Top-level guest functions of each subsystem wrapped in the function table (calls through it: jal / jalr / callGuest;
// natives calling natives directly are inside their caller's bracket). On entry and exit the game thread's six K2
// counters + cycles are read (src/lib/ps2_vita_p2.cpp: ps2x_p2b_enter / leave, a K2 syscall each); inclusive and self
// (minus the brackets opened inside) per frame, every window, as "[p2b] w ..." lines. SOTC_P2BRACKET_EXTRA=0xaddr:name,...
// adds functions. With the switch off nothing is wrapped (one getenv at install). Round P3: SOTC_P2BRACKET=2 adds kTargetsP3
// (one level down); SOTC_P2BRACKET_CLOCK=1 runs them without K2 (Vita3K: call counts and the tree, microseconds).
#include "sotc_p2.h"

#include "ps2_runtime.h"
#include "runtime/ps2_thread_load.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#if defined(__vita__)
extern "C" bool ps2x_p2b_wanted();
extern "C" int ps2x_p2b_level();
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
        constexpr int kMax = 128; // (ps2_vita_p2.cpp's p2b::kMax)
        std::array<Wrapped, kMax> g_w;
        bool g_zoneMark = false; // round X2: SOTC_X2_ZONE=1 (read at install)

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
            if (__builtin_expect(g_zoneMark, 0))
            {
                // Round X2 (SOTC_X2_ZONE=1 with SOTC_PROF129=1): prof129's samples inside this bracket carry zone
                // 0x5000 | slot, so a script body's zone (decomp_w3) marks only its own self.
                const ps2_load::GameZone z(0x5000u | static_cast<uint32_t>(Slot));
                const Bracket b(w.id);
                w.inner(rdram, ctx, runtime);
                return;
            }
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
        // Round P3 (SOTC_P2BRACKET=2; docs/decomp_p3.md): one level down, where the P2 console run's self cycles were
        // (execScript 4.1M, ExecCharaGeometry 2.76M, DispDispObjGroupModel 2.76M a frame). Chosen from the console's own
        // prof129 (pc, ra) pairs of that run (each function's caller chain: what runs inside those brackets' self) and
        // kept only if Vita3K's clock brackets (SOTC_P2BRACKET_CLOCK) count fewer than ~200 calls a frame on the ride.
        // Only functions entered through the table (a guest call / callGuest / callNative) can be bracketed: a native's
        // direct C++ callees ("...Sw" helpers) stay in their caller's self. Left out after the Vita3K count (ride7, calls a
        // frame): isysGroupForNext 323, iosDlLockObj 171, ClipLayoutObjModel 168 (no guest callees: it is most of
        // DispDispObjGroupModel's remaining self), isysGroupForExit 122, clip_particle_geodisp 75, isysLetterObjDiscardLetterAll
        // 72, the particle-display gate 0113ACB8 71 (its emitter is kept); never entered through the table on the ride:
        // manager_sub_010BC4F0, manager_sub_010F3FB0 (DispLayoutObjModel's recompiled callee), DispCloth.
        constexpr Target kTargetsP3[] = {
            // execScript's self: the script pass, the sub-thread wakes (the script bodies run inside), the frame end's work
            {0x01102638u, "scripts:scriptPass"},
            {0x001C9A40u, "scripts:isysWakeupSubThread"},
            {0x013CFBE0u, "scripts:frameEndCharaGeometry"},
            {0x0110A130u, "scripts:ExecObjectAnimation"},
            {0x01365450u, "scripts:StageManagerCheckAreaExist"},
            {0x010CABA0u, "characters:charaCtrl"},
            {0x010D37C8u, "characters:GetMainAnimation"},
            {0x010D38B0u, "characters:GetSubAnimation"},
            {0x010C71D8u, "characters:ctrlTurn"},
            {0x010CA230u, "characters:ctrlWalk"},
            {0x01391E30u, "characters:horseStep"},
            {0x010C9B10u, "characters:ctrlReactions"},
            {0x013B0EB8u, "characters:playerStep_013B0EB8"},
            {0x0108FBE8u, "display:graphFrameInit"},
            {0x0108F8D8u, "display:graphFrameFinish"},
            {0x010F5820u, "display:InitLayoutObjMatrix"},
            {0x010EC2E8u, "display:ExecFogColMan"},
            {0x01164710u, "camera:update_clip_screen"},
            {0x01160570u, "camera:update_camera_context"},
            {0x0135EF18u, "particles:updateSpecialEffectManager"},
            // ExecCharaGeometry's self
            {0x010D9FC0u, "characters:GetCharaRootDynamics"},
            {0x010D9AA0u, "characters:norootdynamics"},
            {0x010D43E8u, "collision:charrootdyn_getCollisionDB"},
            {0x01124C88u, "collision:GetCollisionDBCallBack"},
            {0x010DC7C0u, "collision:charrootdyn_getLandingSurface"},
            {0x010DC020u, "collision:charrootdyn_slidecollision"},
            {0x01116958u, "collision:finder"},
            {0x01137A68u, "characters:GetClothDynamics"},
            {0x013BA2D8u, "characters:nico_execPlayerFringe"},
            {0x011399E0u, "characters:getClothDynamics"},
            {0x01137640u, "characters:clothVelocity_01137640"},
            {0x010BBA60u, "characters:CalcNodeQuaternionWithIK"},
            {0x010E7610u, "characters:xz"},
            {0x010E1808u, "characters:sub_010E1808"},
            {0x010E0558u, "characters:sub_010E0558"},
            {0x010E1068u, "characters:aimIk"},
            {0x01169020u, "characters:ExecAnimeTableInfo"},
            {0x011688A8u, "characters:sub_011688A8"},
            {0x011696A8u, "particles:SequenceKickParticle"},
            {0x010C4CA8u, "characters:AddCharacterClassSubGeometries"},
            {0x010C4220u, "characters:rootBlend"},
            {0x013BC278u, "characters:nico_playerExpression"},
            {0x010C6C80u, "characters:GazeAddMotions"},
            {0x010C6238u, "characters:gaze"},
            // DispDispObjGroupModel's self
            {0x010F5920u, "display:DispLayoutObjModel"},
            {0x010F3A08u, "display:solidDisp"},
            {0x010F3CF0u, "display:skinned"},
            {0x010F4FB8u, "display:lightPass"},
            {0x010F4D48u, "display:CalcLightLayoutObj"},
            {0x010F64C0u, "display:SetGeoDispClassLightColMan"},
            {0x010F8728u, "display:ExecGlobalLightMan"},
            {0x010F1A20u, "display:GetLayoutClass"},
            {0x0113D988u, "particles:disp_particle_emitter"},
            {0x013FDD68u, "display:DispWithSwordBlur"},
            {0x0108D730u, "display:glVertexKick"},
            {0x0108FCC8u, "display:graphFsEffectReset"},
            {0x010998B8u, "display:modelSetEnviroment"},
        };
        // Round X1 (SOTC_P2BRACKET=2; docs/decomp_x1.md): GetCharaRootDynamics' recompiled helpers under xz (0x10e7610,
        // already in kTargetsP3), each in its own bracket, so the console's parts run splits root dynamics' 1.1 M cycles
        // a frame (the ones of more than ~100 bytes; the tiny getters stay in their caller's self).
        constexpr Target kTargetsX1[] = {
            {0x010DF218u, "characters:charrootdyn_updateCharacterClassParentGeometry"},
            {0x010DCDF8u, "characters:charrootdyn_addWindDynamicsToRoot"},
            {0x010DCF10u, "characters:charrootdyn_getPlaneYFittingVector"},
            {0x010DCB10u, "characters:charrootdyn_addSlideElement"},
            {0x010DE1A0u, "characters:charrootdyn_getRegularColFinderPosition"},
            {0x010D3D20u, "characters:charrootdyn_acceptMotionRootOffset"},
            {0x010D6BD8u, "characters:charrootdyn_setkeepcolhitstatus"},
            {0x010DDFB0u, "characters:charrootdyn_restoreposition"},
            {0x010DC270u, "characters:charrootdyn_getGPPlaneInfo"},
            {0x010DEFD0u, "characters:charrootdyn_detectCharEdge"},
            {0x010D5BF0u, "characters:charrootdyn_getNipPlaneStatus"},
            {0x010D5A40u, "characters:charrootdyn_getChainStatus"},
            {0x010DC990u, "characters:charrootdyn_getWaterStatus"},
            {0x010D5D70u, "collision:GetCharaVCollisionStatus"},
            {0x01129C10u, "collision:GetCollisionEdgeLinkDBVWallFinder"},
            {0x010F1EE0u, "display:ParentLayoutObj"},
            {0x001BCA40u, "characters:StabilizeToYRotQuaternion"},
        };
        // Round W3 (SOTC_P2BRACKET=3; docs/decomp_w3.md): the script walk's callees and the camera's steps (lifted code
        // calls them as known natives; wrapped, the slot no longer holds the known one, so the call goes through the table).
        constexpr Target kTargetsW3[] = {
            {0x01102438u, "scripts:GetScriptClass"},
            {0x001C7110u, "scripts:isysGroupForFirst"},
            {0x001C76D0u, "scripts:isysGroupForNext"},
            {0x001C77B8u, "scripts:isysGroupForExit"},
            {0x001C82D8u, "scripts:LetterObjDiscardAll"},
            {0x013CF4E0u, "scripts:charaTail"},
            {0x0141CC90u, "camera:doit_cameraman"},
            {0x01163C70u, "camera:region_manager"},
            {0x01162498u, "camera:lws_anim_manager"},
            {0x01418948u, "camera:obstacle_avoid"},
            {0x01418738u, "camera:view_cut_avoid"},
            {0x014137E8u, "camera:slope"},
            {0x01416DC0u, "camera:subjective_vis"},
            {0x0141A988u, "camera:objective_vis"},
            {0x0141A370u, "camera:CAM_doit_target"},
            {0x0141B380u, "camera:follow_rot"},
            {0x01418350u, "camera:post_update"},
            {0x01411C18u, "camera:final_update"},
            {0x0140FF98u, "camera:default_angle"},
            {0x0141C718u, "camera:view_angle_cntrl"},
            {0x014167D0u, "camera:aim_angle_rot"},
            {0x01417550u, "camera:target_offset"},
        };
    }

    void install(PS2Runtime &runtime)
    {
        if (!ps2x_p2b_wanted())
            return;
        if (const char *z = std::getenv("SOTC_X2_ZONE"))
            g_zoneMark = std::atoi(z) != 0;
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
        if (ps2x_p2b_level() >= 2)
        {
            for (const Target &t : kTargetsP3)
                wrap(t.address, t.name);
            for (const Target &t : kTargetsX1)
                wrap(t.address, t.name);
        }
        if (ps2x_p2b_level() >= 3)
            for (const Target &t : kTargetsW3)
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
        // Round W1: the direct brackets (natives' parts; both halves of the W1 A/B).
        static const char *const kDirectNames[kDirectCount] = {"display:w1GroupPass", "display:w1CopyList", "display:w1LightPass",
                                                               "display:w1DrawLoop", "display:w1Palette", "display:w1CalcLight"};
        int direct = 0;
        for (int i = 0; i < kDirectCount; ++i)
            if ((g_direct[i] = ps2x_p2b_register(kDirectNames[i])) >= 0)
                ++direct;
        std::fprintf(stderr, "[p2b] %d functions wrapped, level %d, %d direct brackets (counters read when K2's counters run on the game thread)\n", slot,
                     ps2x_p2b_level(), direct);
    }

    // Round W3: a bracket opened by native code (not a table entry), at SOTC_P2BRACKET >= level only.
    int named(const char *name, int level)
    {
        if (!ps2x_p2b_wanted() || ps2x_p2b_level() < level)
            return -1;
        return ps2x_p2b_register(name);
    }
    uint32_t open(int id) { return id < 0 ? 0u : ps2x_p2b_enter(id); }
    void close(uint32_t token)
    {
        if (token)
            ps2x_p2b_leave(token);
    }
    int g_direct[kDirectCount] = {-1, -1, -1, -1, -1, -1};
    uint32_t directEnter(int id) { return ps2x_p2b_enter(id); }
    void directLeave(uint32_t token) { ps2x_p2b_leave(token); }
}
#else
namespace sotc_p2
{
    void install(PS2Runtime &) {}
    int named(const char *, int) { return -1; }
    uint32_t open(int) { return 0u; }
    void close(uint32_t) {}
    int g_direct[kDirectCount] = {-1, -1, -1, -1, -1, -1};
    uint32_t directEnter(int) { return 0u; }
    void directLeave(uint32_t) {}
}
#endif
