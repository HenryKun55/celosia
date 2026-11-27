# Round A3 (cpu500): the CPU at 500 MHz, a player option (2026-11-27)

(Not the 2026-11-21 round A3, typed loops, in `docs/decomp_a3.md`: this round reused the letter, so its files are
`decomp_a3cpu.md`, "Decomp A3 (cpu500)" in decomp.md and the rounds.json row "A3cpu".)

Item 3 of `docs/arch_plan.md`: run the Vita's ARM cores at 500 MHz instead of 444 MHz. The user approved it as an
**option**: off by default, turned on by the player in the settings menu (Select twice, System tab, "CPU at 500 MHz"),
or by `SOTC_CPU500=1` in env.txt for test runs. The expected saving (arch_plan §4) is -7% .. -10% of the game thread
(heavy scene 46.6 -> ~42 ms, light 32 -> ~29 ms) and the render thread ~-10%. Nothing about the computation changes.

## Where the clocks were set

- `platform/vita/raylib/raylib_vita.cpp` `InitWindow`: `scePowerSetArmClockFrequency(444)`, bus 222, GPU 222, xbar 166.
- The same four in `resume_watch::setClocks`, after every resume (round V2's watcher thread, which wakes every 250 ms).

Retail ScePower stops at 444 MHz for the ARM. 500 MHz needs a kernel-side route.

## The route (vita/k2/celosia_a3.c)

K2's resident kernel module (`celosia_k2.skprx`, round K2) gets a fourth syscall library, `CelosiaK2A3` (NID
0xC2C2C2A3, function `celosiaK2Clock` 0xC2A3C001). The game reaches it through a new user bridge,
`celosia_a3u.suprx`, built the same way as the C1 and P2 bridges. The route is the one PSVshell uses. I read its source
(`src/oc.c`, `src/main.c`) and wrote the code again here; nothing of PSVshell is downloaded or loaded:

1. `kscePowerSetArmClockFrequency(444)`: the retail top, through ScePower's own path (PLL, divider and its state words).
2. `ScePervasiveForDriver` NID 0xE9D95643, exported by `SceLowio` (library 0xE692C727). This is the ARM clock
   generator's multiplier:divider. It is called with **15:16**, after a Thumb NOP (`00 BF`) is put over the 16-bit
   branch at the function's +0x1E (PSVshell writes at "export address + 0x1D", where the export address carries the Thumb
   bit). Without the NOP the function keeps the multiplier at 444's.
3. ScePower's data segment words +0x41C8 (the ARM frequency `kscePowerGetArmClockFrequency` returns) and +0x41CC (its
   multiplier) are set to 500 / 15. This way the system's own read-back answers 500, and ScePower's next set starts from
   the truth.

### Checks before anything is written

The firmware may not be PSVshell's 3.60 / 3.65 layout. The module reads first and writes only if every check passes:

- `taiGetModuleInfoForKernel("ScePower")` and `module_get_offset(seg 1, 0x41C8)`: the word must be writable for the
  kernel (ATS PL1-write PAR) and must read **444** right after step 1. Any other value means this firmware's layout is
  unknown: `K2_ERR_SITE`, nothing written.
- `module_get_export_func(SceLowio, 0xE692C727, 0xE9D95643)`: the address must be Thumb (bit 0 set). The patch site and
  the half-word before it must translate for a PL1 read. The site's half-word must be one of:
  - a 16-bit conditional branch (B<cond> T1, or CBZ / CBNZ);
  - the NOP already (another plugin, for example PSVshell, did it; then no inject).

  The half-word before it must not be the first half of a 32-bit instruction. Otherwise: `K2_ERR_SITE`, nothing written.
- The NOP goes in with `taiInjectAbsForKernel`. It is taken out again with `taiInjectReleaseForKernel` whenever 444 is
  restored.

### Proof by the CPU itself

`celosiaK2Clock` times a fixed busy loop in the kernel: 400 000 taken `subs/bne` iterations, the shortest of 9 runs,
`ksceKernelGetSystemTimeLow`. It times the loop three times: before the change, right after the 444 set (the
calibration), and after the change. The clock is `est = 444 x t444 / t`. A 500 request whose loop did not get at least
6% faster (500/444 should give 11.2% less time) is undone, rc `K2_ERR_CLOCK`. So the log never says 500 unless the CPU
really runs faster. The ScePower word only says what was written.

### Restoring 444

The module does each restore in the same way: 444 through ScePower; ScePower's words put back if they still say 500;
the NOP taken out. Restores happen:

- when the player turns the option off (`celosiaK2Clock(444)`);
- at the game's normal exit (`std::atexit` in the game, registered at the first successful 500);
- when the game process exits or is killed (K2's process-event handler `onGone`, next to C1's and P2's releases);
- when a new process claims the module (hello);
- at module stop.

## The game side (src/lib/ps2_vita_runtime.cpp, namespace a3)

- Nothing is loaded while the option is off.
- On (at start from settings.cfg / env.txt, in `hostmenu::load()` after InitWindow's 444; or in the menu):
  1. The K2 module is loaded **now** if the boot did not load it. This uses the same two steps as `k2::boot`
     (`taiLoadKernelModule` + `taiStartKernelModule`, P3's resident-module case included) and claims ownership through
     the K2 bridge's hello. k2's own state is left alone, so none of K2's first-frame walks or PMU probes start because
     of A3.
  2. If the boot already loaded K2 (`SOTC_K2=1`), that instance is reused.
  3. Then `app0:module/celosia_a3u.suprx`, then `celosiaK2Clock(500)`.
- The module is still **only loaded by the game at run time**, never through `ur0:tai/config.txt`, so a failure cannot
  boot-loop the console.
- **Fallback**: any failure leaves the CPU at 444 and calls `scePowerSetArmClockFrequency(444)` again. Failures include:
  - no module answer (Vita3K, unsafe homebrew off, module not found);
  - an older resident K2 without the A3 library (the A3 bridge does not bind; reboot once);
  - a site check failing;
  - a loop that did not get faster.

  One `[a3]` line names the reason. The menu then shows "On (refused: 444 MHz)".
- **Suspend / resume / LiveArea**: the resume watcher (raylib_vita.cpp) calls `ps2x_cpu500_watch` at every wake-up.
  - After a resume it calls with resumed=1, right after `setClocks()` put 444 back.
  - Otherwise, when ScePower's frequency no longer reads 500, for example after ScePower re-applied the game's 444 on
    the way back from LiveArea. The A3 path writes ScePower's word to 500, so a revert shows up as 444 there.

  Either way 500 is applied again. After three failed re-applies in a row it gives up and stays at 444. The cost is one
  `scePowerGetArmClockFrequency` call every 250 ms, and only while the option is on.
- Menu: System tab, "CPU at 500 MHz" / "CPU a 500 MHz", with English and Portuguese help. Saved as `cpu500=0/1` in
  `ux0:data/celosia/settings.cfg`. `SOTC_CPU500` overrides it for one run and is not saved unless the player changes the
  item in the menu. In the release build `ps2_diag::env` is null, so only the menu applies there.

### The log lines that prove it (console)

The success line ends like this:

    [a3] cpu 500 (option on): rc 0 (ok); kernel: busy loop 1801 us at 444 MHz, 1801 us before (= 444 MHz), 1600 us after (= 500 MHz);
    ScePower says 444 -> 500, its words 444/../.. -> 500/15 (...); ... on 1; scePowerGetArmClockFrequency now 500 MHz: CPU500 ON: the CPU runs at 500 MHz

(The microsecond numbers here are only illustrative.) The proof is the "after (= ~500 MHz)" busy-loop figure together
with "CPU500 ON". A refusal ends with "CPU500 REFUSED: the CPU stays at 444 MHz" and the reason (rc / site half-words /
ScePower NID). Turning the option off logs "CPU500 OFF: the CPU back at 444 MHz".

## Vita3K

Vita3K cannot change a kernel clock. Its `taiStartKernelModule` returns 0 without running the module, so there is no
answer. The A3 path logs "CPU500 REFUSED ... (the K2 module did not answer / K2 was tried at boot and did not answer)" and
the game runs as before. See the Vita3K result below.

## Results

Build p1 is main 0d923a6 (Z4) + A3. It has 0 errors, no "segment 1 overlaps", `sotc_vita_elf_room` = 0x1e000
(122 880), and spalign reports 0 sp-based of 3 417.

The VPK's module/ holds `celosia_k2.skprx` (changed: the A3 library), `celosia_k2u.suprx` and the new
`celosia_a3u.suprx`.

Vita3K runs, ride7, 600 s, 222 MHz, 3 cores, muted:

- **on1**: all/env-all-on.txt (SOTC_K2=1) + SOTC_CPU500=1. It booted, rode, and was mounted at frame 5 233. The log has
  `[a3] cpu 500 (option on): CPU500 REFUSED: the CPU stays at 444 MHz (K2 was tried at boot and did not answer; ...)`
  and nothing else from A3. The last frame was 47.1 ms / game 42.7, the same as before.
- **on2**: the same + SOTC_K2=0. This is the player's path: the K2 module is loaded lazily from the option. It booted,
  rode, and was mounted at frame 2 403. The log has
  `[a3] cpu 500 (option on): CPU500 REFUSED: ... (the K2 module did not answer (Vita3K / unsafe homebrew off / module not
  found); load 0x000000f7 start 0x00000000/-1 ...)`.

  The lazy `taiLoadKernelModule` + `taiStartKernelModule` after libc did not hang Vita3K.

Pictures: on1 t0300 is Wander on Agro under the bridge pillars and t0540 is the gallop on the plain past the stone
arches. on2 t0540 is the gallop on the plain. All are normal (ground, sky, Agro, cape, reins).

SOTC_CPU500 from env.txt was not written to settings.cfg in either run. Neither run had a settings-after.cfg with a
cpu500 key.

### Console (pending; nothing installed here)

The package is `build/agent-scratch/a3/`. README.txt has the steps; the install is eboot.bin + module/celosia_k2.skprx +
module/celosia_k2u.suprx + module/celosia_a3u.suprx. **Reboot once** before the first run: an old resident K2 has no A3
library, and A3 would refuse cleanly. Then two rides, env-a3-off.txt and env-a3-on.txt.

The proof of 500 is the "[a3] cpu 500 ... after (= ~500 MHz) ... CPU500 ON" line. Then compare the [m1pmu] / frame ms of
the two rides; arch_plan expects -7..-10%.

Open: does ScePower re-apply the game's 444 when it comes back from LiveArea? The watcher handles it either way, and the
console log will show it as "[a3] cpu 500 (again ...)".
