// Round 126 (ctl126, decomp_cam126.cpp): the two option-node getters made in
// place by native callers (decomp_hot47a's child / root node passes). Each
// returns false when the call has to go to the guest (the switch is off, the
// table holds something else: SOTC_DECOMP_SKIP, a difftest checker; or the
// worker lane), and the caller then makes its callGuest as before.
#pragma once

#include <cstdint>

struct R5900Context;
class PS2Runtime;

namespace cam126
{
    // GetMotionOptionNodeParentId (0x10b63c0): v0 / v1 / a1 as the original leaves them.
    bool parentIdDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // GetCharacterClassOptionNodeLocalQuaternionPos (0x10c10e0): the two qwords, v0 / a3 as the original leaves them.
    bool localQPDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
