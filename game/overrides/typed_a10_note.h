// typed_a10_note.h: round A10's camera note (docs/decomp_a10.md section 2). iosgluGetCameraContext's slot holds sotc_native's
// hook (getCameraContextHook): the native, then a note of the camera context (v0) the renderer reads (g_cameraContext and
// sotc_decomp::g_cameraContextAddr). The typed iosgluGetCameraContext (typed_a8.inc, tools/typed_lift2.py NOTE10) returns
// through A10_CAMNOTE, which makes the same note through the same core (sotc_native::s4NoteCamera, which the hook calls too)
// while g_a10CamHook says the hook holds the slot over decomp_leaves::gluCameraContext and the switch a10typed is on; the
// slot tests then take the hook as the native (a6Is). Elsewhere (the hook not there, a10typed off) the note is nothing, as the
// slot then holds the native itself or the test fails.
#pragma once

#include <cstdint>

#if defined(A3_OFFLINE)
namespace a10
{
    inline uint32_t g_offNotes = 0; // the offline harness: notes counted, nothing written
    inline uint32_t camNote(uint32_t v) { return ++g_offNotes, v; }
}
#else
extern bool g_a10CamHook; // sotc_native.cpp, once a frame: a10typed, the hook in the slot, gluCameraContext behind it
namespace sotc_native
{
    void s4NoteCamera(uint32_t context); // the note's core (the hook and the typed path)
}
namespace a10
{
    inline uint32_t camNote(uint32_t v)
    {
        if (__builtin_expect(g_a10CamHook, 1))
            ::sotc_native::s4NoteCamera(v);
        return v;
    }
}
#endif

#ifndef A10_CAMNOTE
#define A10_CAMNOTE(v) ::a10::camNote(v)
#endif
