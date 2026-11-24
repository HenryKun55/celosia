# Round B8: the dispatcher's deliveries without invocation records, the gate re-set at timer reads (switch b8sched) (2026-11-23)

Plan: docs/decomp_100_plan.md, phase B (layers 6 and 7), step B8. Base: main 0394454 (B7 on A12). Switch `SOTC_B8SCHED` (A/B
name `b8sched`, sotc_decomp `kB8Sched`, **off by default**; it takes effect only while b7irq is in effect, so with b3threads and
the other carrier switches once GAMECORE runs). Code: PS2Recomp/ps2xRuntime/src/lib/Kernel/EeScheduler.cpp +
include/runtime/ee_scheduler.h (`b8Push`, `b8Dispatch`, `b8Materialize`, the hooks in run() / carrierNest / s17Nest, `b8Step` /
`b8ShadowPush` / `b8ShadowRecord` (the check), `b8Regate` / `b8Compare`, `gateNow` split out of `updateGate`, `EeB8Stats`),
ps2_memory.cpp / .h (the regate hook after a timer read's flush), game/overrides/b8_sched.cpp (the switch, the `[b8sched]` line),
b7_irq.cpp (each host handler's charge formula for the check), sotc_native.cpp / sotc_decomp.h (install, frame call, the switch
tables). Scripts, runs and logs: `build/agent-scratch/b8/` (NOTES.md). Vita3K trees ../vita3k-celosia-b1 / -b1b / -b1c (pointed
at b8; 222 MHz, 3 cores, audio-volume 0; every run muted).

__RESULTS__
