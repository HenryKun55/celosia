#pragma once

// Player settings menu drawn over the game (not the developer debugger).
// Opens with Select pressed twice (F1 on a keyboard); L/R: tab, D-pad: item,
// Cross: change, Triangle: help, Circle: close. Settings apply at once and
// are saved to a small text file.

#include <atomic>

namespace hostmenu
{
    enum ScreenMode
    {
        kScreenFit = 0,     // keep the 4:3 picture, black bars
        kScreenStretch = 1, // fill the screen
        kScreenInteger = 2, // largest whole multiple (sharp pixels)
    };

    struct Settings
    {
        std::atomic<bool> exactAlpha{true}; // GPU renderer: alpha-fail pass
        std::atomic<bool> showFps{true};
        std::atomic<int> screenMode{kScreenFit};
        std::atomic<int> volume{10};   // 0..10
        std::atomic<int> language{0};  // 0 auto, 1 Portuguese, 2 English
    };

    Settings &settings();

    // Loads the saved settings (call once at startup).
    void load();
    // Writes them (and the renderer adjustments, native_tuning.h) now.
    void saveSettings();
    // Reads the controller and keyboard; call once per host frame.
    void update();
    // Draws the menu when open; call inside BeginDrawing/EndDrawing.
    void draw(int screenWidth, int screenHeight);
    // The performance overlay (Debug "Performance overlay", round 20): with
    // the menu closed; nothing when off.
    void drawPerfOverlay(int screenWidth, int screenHeight);
    bool isOpen();
    // True while the menu owns the controller (open, or a button pressed in
    // it is still held): the game sees no input then.
    bool inputBlocked();
    // The player chose "Quit game".
    bool quitRequested();
}
