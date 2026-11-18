// Round S12 (SOTC_S12DRAW, A/B `s12draw`): the display's immediate quads as host vertex records.
//
// An immediate quad (glBegin, four glColor4f / glTexCoord2f / glVertex3fv, glEnd: the heat-haze strips behind
// Wander, Agro and the colossi, 01142350's quads) was, after rounds 117 / 128 / 140, one pass (quad117) that still did
// what the PS2's gl layer does per vertex: the colour and ST written into the gl block and read back, the vertex
// copied into the gl ring and read back, the state words read-modified-written, and the result encoded as GS A+D
// pairs (ST / UV, RGBAQ, XYZF2 / XYZF3) in a host array, which immFeed128 then fed one by one through feed2D's
// register switch into the 2D assembler. Part `strips` (bit 0): one host pass with everything in registers (the
// colour clamp, the ST, the state words, the CLIP history, the pending count), the gl block written once at the end
// (the same words, the same values: what the next gl call or glEnd's software clip reads), and the vertices handed to
// the 2D assembler as a record (Quad12) instead of pairs. SOTC_S12DRAW_CHECK=1: every quad the new way dry, then the
// old way (the picture's), the records (the 2D assembler's input), the gl block and VU0 compared.
#pragma once

#include <cstdint>
#include <vector>

namespace s12draw
{
    // One immediate quad's input to the 2D assembler (what its pairs carried). No alignment: plain words.
    struct Quad12
    {
        uint64_t test = 0, testReg = 0; // glBegin's TEST pair (register 0x47 / 0x48: no effect on the assembler)
        uint64_t prim = 0;              // its PRIM value
        uint32_t layer = 0;             // the display list's layer glBegin would push
        bool haze = false;              // the strips' screen copy (0x10f37c8) as the texture
        int n = 0;
        uint8_t stReg[4] = {};    // 2 ST, 3 UV, 0 none (untextured)
        uint8_t xyzReg[4] = {};   // 4 XYZF2 (a drawing kick), 0xC XYZF3
        uint64_t st[4] = {};      // the ST / UV pair's value
        uint64_t rgbaq[4] = {};   // RGBAQ
        uint64_t xyz[4] = {};     // XYZF2 / XYZF3
    };

    bool stripsOn();  // part `strips` in force (the switch, the part, the native renderer drawing)
    bool reuseOn();   // part `reuse` (bit 1): a vertex the quad before transformed, its products taken again
    bool checkOn();   // SOTC_S12DRAW_CHECK
    void feed(uint8_t *rdram, const Quad12 &q);                       // into the 2D assembler (sotc_native.cpp)
    void record(std::vector<uint64_t> &rec, const Quad12 &q);         // the check's view: as immRecord128's
    void armOld(std::vector<uint64_t> *rec);                          // the check: the old way's feed recorded
    extern uint64_t g_reused, g_vertices;
    void noteQuad(bool fast, bool clipped, bool checked, bool differ); // the [s12draw] counts
}
