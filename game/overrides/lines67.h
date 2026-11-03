// Round 67: a Mac-side cache-miss proxy for the per-object walks
// (SOTC_LINES67=1; nothing in the Vita build). The walks' guest reads and
// writes on the game thread (tagged by walk) are counted as 64-byte lines:
// per frame, the distinct lines each walk touched, and of those the
// "exposed" ones: lines whose first touch that frame was not asked for
// (prefetched) in an earlier object call of the frame or at least 16 of the
// walks' memory accesses before, i.e. the A9's demand misses if every line
// were cold and every prefetch that far ahead arrived in time. Every
// SOTC_LINES67_EVERY frames (300) a `[lines67]` line per walk: calls,
// distinct lines, exposed lines, lines asked for, asked for but unused.
#pragma once

#include <cstdint>

namespace lines67
{
    enum Walk : int
    {
        kNone,
        kGroup, // DispDispObjGroupModel's own reads (the group, its passes over the sort lists)
        kClip,  // ClipLayoutObjModel / modelClipSolid
        kDisp,  // DispLayoutObjModel / 010F3A08 / the gl kick
        kHook,  // modelDispHook / meshDispHook's game-thread part
        kPtcl,  // a particle system's draw (manager_sub_0113E940)
        kColl,  // the collision tree walk and its leaves
        kCount
    };
#if defined(__vita__)
    constexpr bool g_on = false;
    inline void touch(uint32_t) {}
    inline void pref(uint32_t) {}
    inline void call() {}
    inline void frame(uint64_t) {}
    inline void touchAs(int, uint32_t, uint32_t) {}
    inline void prefAs(int, uint32_t) {}
    inline void callAs(int) {}
    inline void base(uint32_t) {}
    struct Scope
    {
        explicit Scope(int, bool = false, uint32_t = 0u) {}
    };
#else
    extern bool g_on;
    extern thread_local int t_walk;
    extern thread_local uint32_t t_stack;
    extern thread_local uint32_t t_base; // SOTC_LINES67_REL=1: exposed lines by offset from this record // the walk's guest stack (+-8 KB around it not counted: always in the cache)
    void touchSlow(uint32_t addr);
    void prefSlow(uint32_t addr);
    void callSlow();
    void frameSlow(uint64_t frame);
    void touchAsSlow(int walk, uint32_t addr, uint32_t sp);
    void prefAsSlow(int walk, uint32_t addr);
    void callAsSlow(int walk);
    // For a walk whose natives have no scope of their own (the collision's
    // G): the game thread's only (no other thread runs guest code).
    inline void touchAs(int walk, uint32_t addr, uint32_t sp)
    {
        if (__builtin_expect(g_on, 0))
            touchAsSlow(walk, addr, sp);
    }
    inline void prefAs(int walk, uint32_t addr)
    {
        if (__builtin_expect(g_on, 0))
            prefAsSlow(walk, addr);
    }
    inline void callAs(int walk)
    {
        if (__builtin_expect(g_on, 0))
            callAsSlow(walk);
    }
    inline void touch(uint32_t addr)
    {
        if (__builtin_expect(g_on, 0) && t_walk != kNone)
            touchSlow(addr);
    }
    inline void pref(uint32_t addr)
    {
        if (__builtin_expect(g_on, 0) && t_walk != kNone)
            prefSlow(addr);
    }
    // A new object's call (the epoch a prefetch must come before to count).
    inline void call()
    {
        if (__builtin_expect(g_on, 0) && t_walk != kNone)
            callSlow();
    }
    inline void frame(uint64_t f)
    {
        if (__builtin_expect(g_on, 0))
            frameSlow(f);
    }
    inline void base(uint32_t b) { t_base = b; }
    struct Scope
    {
        int prev;
        uint32_t prevStack;
        explicit Scope(int walk, bool newCall = false, uint32_t sp = 0u) : prev(t_walk), prevStack(t_stack)
        {
            t_walk = walk;
            if (sp)
                t_stack = sp;
            if (newCall)
                call();
        }
        ~Scope()
        {
            t_walk = prev;
            t_stack = prevStack;
        }
    };
#endif
}
