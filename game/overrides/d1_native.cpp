// Round D1 (d1_native.h): the run counts of the D1 bodies.
#include "d1_native.h"

#include <cstdio>

namespace d1
{
    static Counter *g_counters = nullptr;
    Counter::Counter(const char *nm) : name(nm), next(g_counters) { g_counters = this; }

    void report(unsigned long long frame)
    {
        uint64_t total = 0;
        int n = 0;
        for (const Counter *c = g_counters; c; c = c->next)
            total += c->n, ++n;
        std::fprintf(stderr, "[d1] frame %llu: d1native %d; %d bodies, %llu calls\n", frame, on() ? 1 : 0, n, (unsigned long long)total);
        for (const Counter *c = g_counters; c; c = c->next)
            if (c->n)
                std::fprintf(stderr, "[d1]   %s %llu\n", c->name, (unsigned long long)c->n);
    }
} // namespace d1

#include "d1_alias.h"

namespace d1
{
    void syncTable(PS2Runtime *runtime, bool on)
    {
        static int state = -1;
        if (!runtime || state == (on ? 1 : 0))
            return;
        state = on ? 1 : 0;
        int swapped = 0, other = 0;
        for (uint32_t i = 0; i < g_aliasCount; ++i)
        {
            const AliasEntry &e = g_aliasList[i];
            const GuestFn from = on ? e.old : e.sw, to = on ? e.sw : e.old;
            const GuestFn now = runtime->lookupFunction(e.address);
            if (now == from)
            {
                runtime->registerFunction(e.address, to);
                ++swapped;
            }
            else if (now != to)
                ++other;
        }
        static uint64_t changes = 0;
        if (changes++ < 4 || changes % 1000 == 0)
            std::fprintf(stderr, "[d1] table: d1native %s, %d slots swapped, %d held by something else (change %llu)\n", on ? "on" : "off", swapped, other,
                         (unsigned long long)changes);
    }
} // namespace d1
