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
