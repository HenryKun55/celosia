# Round B9: a delivery in one turn of the dispatcher's loop, the ready queues as a host bitmap (switch b9loop) (2026-11-24)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B9 (decomp_b8.md section 6: "the loop itself is the next thing
to make host-shaped"). Base: main 3061496 (B8 + A13). Switch `SOTC_B9LOOP` (A/B name `b9loop`, sotc_decomp `kB9Loop`, **off by
default**; it takes effect only while b8sched is in effect, so with b7irq and the carrier switches once GAMECORE runs). Code:
PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp + include/runtime/ee_scheduler.h (`b9AfterPush`, `b9Dispatch`,
`b9SkipTop`, `b9Quiet`, the bitmap `m_b9Ready` with `b9MaskSet` / `b9MaskClear` / `b9MaskRebuild` / `b9FirstReady`, the check
`b9Digest` / `b9TopCheck`, `EeB9Stats`; the three loops' push points and slot dispatches), game/overrides/b9_loop.cpp (the
switch, the `[b9loop]` line), sotc_native.cpp / sotc_decomp.h (install, frame call, the switch tables). Scripts, runs and logs:
`build/agent-scratch/b9/` (NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed at b9; 222 MHz, 3 cores,
audio-volume 0; every run muted).

RESULTS_PLACEHOLDER
