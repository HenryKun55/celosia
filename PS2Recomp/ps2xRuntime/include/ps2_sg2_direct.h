#pragma once

#include <cstddef>
#include <cstdint>

class PS2Runtime;
struct R5900Context;

// Round N2 (n2snd): the native sound driver's status request called straight from the game's native remote tick
// (game/overrides/sound_n2.cpp), as the RPC would run it (the IOP's clock caught up first), without SIF / RPC /
// the IOP request: commands of 4 words each (cmd, a, b, c) applied in order, the 400-byte reply put in reply.
// False when the native driver is not loaded (the caller sends the RPC then). reply null: only asks whether it is.
bool ps2xSg2Serve(PS2Runtime *runtime, uint8_t *rdram, R5900Context *ctx, const uint32_t *command, size_t words, uint8_t *reply);
