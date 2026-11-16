// Round A9 sweep (SOTC_A9SWEEP, default on; A/B a9sweep): the hoisted values' initialisation (a9sweep.h).
#include "a9sweep.h"
#include "runtime/ps2_a9.h"
#include "runtime/ps2_diag.h"
#include <vector>

namespace a9sweep
{
    void init()
    {
        if (g_ready)
            return;
        g_log = ps2_diag::env("SOTC_A9SWEEP_LOG") != nullptr;
        // In registration order (a value may read one hoisted before it in the same file); the list is newest first.
        int n = 0;
        for (const Hoist *h = g_head; h; h = h->next)
            ++n;
        std::vector<const Hoist *> order(static_cast<size_t>(n));
        int i = n;
        for (const Hoist *h = g_head; h; h = h->next)
            order[static_cast<size_t>(--i)] = h;
        for (const Hoist *h : order)
            h->fn();
        g_ready = true;
        // The switch reads its variable now (0, off, until here: nothing hoisted is read before its value is set).
        sotc_decomp::g_switch[sotc_decomp::kA9Sweep] = -1;
        const bool sw = on();
        ps2_a9::g_flag = &sotc_decomp::g_switch[sotc_decomp::kA9Sweep];
        std::fprintf(stderr, "[a9sweep] %d hoisted initialisers run; SOTC_A9SWEEP %s\n", n, sw ? "on" : "off");
    }
}
