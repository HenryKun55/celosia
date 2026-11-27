// Round Z2 (docs/decomp_z2.md): the rest of the cloth library as native C++ (SOTC_Z2=1, off by default; it acts inside
// round Y2's getClothDynamics driver, so SOTC_Y2=1 and SOTC_D1NATIVE=1 too).
//
// Y2 left getClothDynamics' guest calls for every whole-set callback but Wander's mantle and for a type-1 cloth's
// velocities: on ride7 ~18.5 of ~20 calls a frame still handed the context to a callback and ~15 to the type-1 velocity
// path (the reins' strings of layout objects). Z2 makes them C++ calls from the driver (dc2_chara.cpp, z2_dynamics.inc):
//   - gamecore_sub_013978E8 (the reins against the horse's three cylinders) and gamecore_sub_013976B8 (the bridle against
//     the head: its frame made on the host) on round D2's NEON walks (decomp_bridle.cpp);
//   - gamecore_sub_01397110 (a hanging strip's pass: Agro's cloth) and gamecore_sub_013BA790 (a string point's length
//     keep: Wander's fringe) as new typed functions;
//   - a type-1 cloth's velocities (manager_sub_01137640 from 0x1137710): the velocity rows, then along the string each
//     layout object turned to its segment and placed (GetLayoutClass's plain cases, the direction, the quaternion,
//     UpdateLayoutClassSimpleGeometry, isysGroupForNext as cores), isysGroupForFirst / isysGroupForExit (the group lock)
//     the only guest calls left.
#pragma once

#include <cstdint>

class PS2Runtime;
struct R5900Context;

namespace z2
{
    // The segments Z2 runs (counters, checks, brackets).
    enum Kind
    {
        kThree = 0,  // 013978E8
        kHead = 1,   // 013976B8
        kPass = 2,   // 01397110
        kKeep = 3,   // 013BA790
        kType1 = 4,  // a type-1 cloth's velocities
        kKinds = 5
    };
    constexpr uint32_t kCbThree = 0x013978E8u, kCbHead = 0x013976B8u, kCbPass = 0x01397110u, kCbKeep = 0x013BA790u;

    // SOTC_Z2=1 (read once) with Y2's preconditions.
    bool on();
    // SOTC_Z2_PARTS=<mask> (testing): bit per Kind (default all).
    bool partOn(Kind k);

    struct Stats
    {
        uint64_t calls[kKinds] = {}, host[kKinds] = {}, fallback[kKinds] = {}, guestCalls[kKinds] = {};
        uint64_t otherCallback = 0, objects = 0, objectGuest = 0;
        uint64_t checks[kKinds] = {}, bad[kKinds] = {}, voided[kKinds] = {}, notQuiet[kKinds] = {}, deadStack[kKinds] = {};
        uint64_t regChecks = 0, regBad = 0;
    };
    extern Stats g_st;
    // SOTC_Z2_LOG=<frames>: "[z2]" / "[z2chk]" lines.
    void tick();
    // Direct brackets (SOTC_P2BRACKET >= 1): characters:z2Three / z2Head / z2Pass / z2Keep / z2Type1.
    int bracket(Kind k);

    // ---- the typed functions (z2_cloth.cpp) -----------------------------------------------------------------------
    // gamecore_sub_013BA790(record, layout class): the string point kept at its length and out of the class's 0.15
    // cylinder (its frame's three vectors, dead stack, are not written). false: outside the domain (nothing written).
    bool stringKeep(uint8_t *rdram, uint32_t record, uint32_t cls);

    // ---- the check (SOTC_Z2_CHECK=1, same thread) -----------------------------------------------------------------
    // checkNow(k): this call is checked (from SOTC_Z2_CHECK_FROM, one in SOTC_Z2_CHECK_EVERY per kind, only while the EE
    // clock is quiet). Then: begin() (RAM / scratchpad / context copied), Z2's leg, afterMine() (Z2's words listed and
    // only those put back, the context back), the old leg, finish() (every word either leg changed compared; void when
    // the VBlank handler ran in a leg). The game goes on from the old leg.
    bool checkNow(Kind k, PS2Runtime *runtime);
    bool begin(Kind k, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void afterMine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void finish(Kind k, uint8_t *rdram, PS2Runtime *runtime, uint32_t entrySp, uint32_t a0);
    // A begun check whose Z2 leg ran nothing (outside its domain): the copies dropped, nothing compared.
    void abandon();
    bool checking();
    void noteRegisters(bool same);
    // SOTC_Z2_CHECK_RECOMP=1: the old leg is the recompiled function (the MIPS as generated, its callees through the
    // table) run to its return instead of the table's entry (round D2's natives for the bridle walks / the string keep).
    bool recompRef();
    void runRecompiled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address);
}
