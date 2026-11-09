#pragma once

// The input recorder and its replay (SOTC_INPUT_RECORD / SOTC_INPUT_REPLAY,
// sotc_replay.cpp): the game's logic frames keyed by its frame-time call,
// the pads by FlushPadHistory, so a session recorded on one platform plays
// the same on another.

class PS2Runtime;

namespace sotc_replay
{
    // After the other overrides: wraps what the table holds by then.
    void install(PS2Runtime &runtime);
    // Round 135: Agro's object as its step (0x01391E30) last saw it (0: none yet); kept with SOTC_POS_LOG too, for
    // the [pos] line's horse position (Agro's gait from its speed).
    unsigned horseObject();
}
