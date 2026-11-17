#ifndef PS2_PAD_H
#define PS2_PAD_H

#include <cstddef>
#include <cstdint>

class PSPadBackend
{
public:
    PSPadBackend() = default;
    ~PSPadBackend() = default;

    bool readState(int port, int slot, uint8_t *data, size_t size);
    // Guest RAM for PS2X_INPUT_SCRIPT entries that wait on a memory word.
    static void setScriptMemory(const uint8_t *rdram);
    // Round S7: host-side flags an input script can wait on ("@f<k>=<v>", "r<n>@f<k>=<v>"), set by game code (k < 8).
    static void setScriptFlag(int k, uint32_t value);
};

#endif
