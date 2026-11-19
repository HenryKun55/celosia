# Round S20: the A/B of default-off switches, re-checked (2026-11-19)

Round S18 found that S17's A/Bs were A/A: `abSwitches` turns a listed switch off in its "off" windows and, in its "on" windows,
restores the switch's value at the start. A switch that is off by default therefore ran off in both windows unless the run
also set `SOTC_<NAME>=1`.

## Audit: was the switch really on in the "on" windows?

| switch | round | decision A/B (off - on, 2 SE) | env / default on during the A/B? |
|---|---|---|---|
| s17iop | S17 | +0.156 +- 0.168 etc. | **no: A/A** (S18 re-measured: +0.13 ms, z 3.0, now default on) |
| s16plan | S16 | R7 -0.085 +- 0.371, R11 +0.058 +- 0.247, L7 +0.106 +- 0.258 | yes (abs16.sh set SOTC_S16PLAN=1) |
| s14hook | S14 | R7 800 s +0.058 +- 0.20, R11 +0.137 +- 0.21 | yes (p1 had it on by default; readChainPaletteLean fell 0.57% -> 0.28-0.33%) |
| s13mot | S13 | R7 +0.123 +- 0.192, R11 -0.099 +- 0.228 | yes (p2/p3 on by default) |
| s12draw | S12 | p3 +0.303 +- 0.289 / +0.149 +- 0.163; p4 +0.111 +- 0.238 / -0.077 +- 0.299; lake +0.018 / +0.053 | yes (p2-p4 on by default; quad12Body in the sampler) |
| s7wind | S7 | R7 +0.146 / -0.101, lake +0.124 / +0.222, attract +0.262 / +0.063; pooled +0.12 +- 0.06 | yes (time per call by state 13.4 -> 10.8 us) |
| pose136 | r136 | sampler only, no Vita3K frame A/B | n/a (the console env turns it on) |
| texr128 | r128 | sampler + Mac rotating A/B (+- 1 ms) | yes |
| flush/walk/part/view119 | r119 | rotating A/B, r119all -0.2..+0.14 | likely yes (per-state times) |
| oan106 | r106 | rotating, +0.94 / -0.62 / -0.49 / -0.01 | yes |
| ptcl91 | r91 | with SOTC_PTCL91=1, +0.9..+1.9 | yes (off for the console's sake) |
| early95 | r95 | run with SOTC_EARLY95=1 | yes |
| obj96, ctl126, gaze122, seg123, charlod78, ptcl79 | | off for correctness / gameplay, not by an A/B | n/a |
| find84 | r84 | numbers not found | unknown |

Only S17 was A/A. The other off switches were measured on; their problem was power (400 s on a loaded machine), not A/A.

## Re-measured (Vita3K 222 MHz, per-frame interleaved, env = 1)

| run | off - on (2 SE) | note |
|---|---|---|
| pose136, ride7 800 s seed 7 | -0.036 +- 0.325 ms (26 blocks) | mounted late (frame 6075); loaded machine (~36 ms/frame) |
| s14hook, ride7 800 s seed 7 | -0.064 +- 0.210 ms (44 blocks); busy +0.064 +- 0.204 | on confirmed: [s14hook] 41 424 takes |
| s7wind, ride7 800 s seed 13 | +0.219 +- 0.385 ms (29 blocks); busy +0.168 +- 0.331 | 8 418 frames, 178 >= 200 ms left out (Mac screen locked) |

**s7wind default on**: exact (S7: 0 differ), and its A/Bs pooled with this one read about +0.12 +- 0.06 ms (2 SE), z about 4,
the bar S18 used for s17iop. `SOTC_S7WIND=0` turns it off (decomp_wind.cpp). pose136 and s14hook stay as they are (even).
s12draw, s16plan were not re-run (the earlier runs had them on).

## The A/B fixed

`abSwitches` (sotc_native.cpp): a switch listed by name in SOTC_AB_FLAG that is off at the start is now turned ON in its on
windows, with a loud `[ab] WARNING: <name> (<ENV>) is off at the start ...` line. `SOTC_AB_FORCE_ON=0` keeps the old rule
(and still warns). s7wind (not a sotc_decomp switch) got the same: `decomp_wind::g_s7AbOn`. The `rNNNall` groups are not
covered (they only turn off; their on windows still restore the start value).
Checked on the Vita build (bootR20, SOTC_AB_FLAG=s12draw without SOTC_S12DRAW): the warning printed once and [s12draw]
counted 131 291 fast quads by frame 10 801; 600 s, SOTC_VITA_EAT_USER_MB=89, no fault, max game 53.5 ms.

The console env (build/agent-scratch/console-env-latest.txt) changes nothing: every switch it lists is on by default, or its
env turns it on (SOTC_S7WIND=1, now the default anyway).
