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
    // Round S7: the left stick held at (x, y) (-1..1, y down) by game code while on.
    static void setSteerStick(bool on, float x, float y);
    static void setSteerButtons(uint16_t mask); // held down while set (PS2 masks)
    // After the window: gamepad mappings raylib's GLFW lacks (built in, then an SDL-format
    // gamecontrollerdb.txt in the working directory or at PS2X_GAMEPAD_DB), and a log line
    // for a controller GLFW sees with no mapping (its buttons would never reach the game).
    static void installGamepadMappings();
    // On the main thread, now and then (the host loop): a [pad] line for a
    // controller connected since the last look (its mapping applies already).
    static void pollControllers();
};

#endif
