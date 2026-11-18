# Leaving the game and coming back on the Vita (round V2)

The console report (2026-11-18, build b26cbd7): after leaving the game by accident (the PS button to
LiveArea, or the console going to sleep / the screen turning off) and coming back, a load never finished
or the game stopped moving.

## What breaks across a suspend

The Vita stops the whole process while it is away (no thread runs). Coming back:

| What | Before V2 | Effect |
| --- | --- | --- |
| Disc image handles (`ux0:data/celosia/disc/...`) | opened once and cached forever (`Support.h` readHostRangeTimed, `CD.cpp` aheadPread) | the system invalidates ux0: handles at a suspend (the reason re3-vita needed the FdFix plugin): every read through the cache failed from then on, so every emulated CD read failed again and again: **the load never ends** |
| Memory card image (`memcards/Mcd001.ps2`) | one `std::fstream` for the whole session (`MemoryCard2.cpp`) | writes fail: no save after a resume |
| Logs (`err.txt`, `log.txt`) | one handle each | nothing logged after the suspend (the console log of the report simply ends) |
| Display pacing (round 110, `raylib_vita.cpp`) | vcount unwrapped from 16 bits with a +-0x8000 window | a jump of 0x8000+ refreshes (9 min away, or the counter reset by the resume) read as a step back: the next picture's slot up to 9 minutes in the future, the display callback waits for it, the render thread waits behind it, the game waits for the render thread: **the game stops moving** |
| CPU / GPU clocks | set once at boot (444 / 222 / 222 / 166) | the system's defaults after a resume |

Unchanged and fine: the emulated EE clock follows the host clock with an 8-VBlank cap (no catch-up storm), the
audio output (SDL on sceAudioOut blocks per buffer and goes on), controls, vitaGL.

## What V2 does (`include/runtime/ps2_resume.h`)

- **Disc reads**: a read on a cached handle that fails reopens the file by its path and reads again (every
  100 ms, for up to 10 s while the card comes back). Normal path: the same `sceIoPread` and one compare.
- **Resume detection** (`raylib_vita.cpp`, thread `celosia_resume`, asleep in `sceKernelDelayThreadCB`):
  ScePower's callback (`SCE_POWER_CB_SYSTEM_RESUME`, `SCE_POWER_CB_APP_RESUME`) or a wall-clock gap of
  3 s+ between two of its 250 ms wake-ups. (SceAppMgr's `ON_RESUME` event is not polled: Vita3K answers
  every `sceAppMgrReceiveSystemEvent` with it, which made a "resume" every 250 ms in the first test build.)
  On a resume: the clocks set again and `ps2_resume::noteResume` (count + `[resume]` log line).
- On the resume count changing: the display pacing's chain starts again; the memory card image is opened
  again before its next write (and any failed write is retried on a new handle for up to 5 s); the log files
  are opened again (appending).
- The EE scheduler is left alone. Its host clock sync already caps a gap at 8 VBlanks, so a resume is one
  133 ms step, not a catch-up storm. A first version moved its VBlank host deadlines to now on a resume: in
  the walk route's test (suspend mid-load at frame 400) the game never produced another frame (VBlanks went
  on, the game thread idle), most likely because every other pending deadline (alarms, CD timings) moved seconds on with
  them; without it the same test goes on. Its VBlank deadlines simply stay behind real time after a resume,
  as they already do after any slow stretch (in Vita3K they lag by seconds all the time).
- **vcount**: only a small step back (another thread's newer read) is one; anything else is forward. A
  picture's slot is never more than 30 refreshes away.

The normal path's cost: a relaxed atomic load per picture and one per vcount
read; the watcher's wake-up every 250 ms.

## The test (Vita3K and the Mac cannot suspend)

`SOTC_TEST_SUSPEND=<frame>:<ms>[:q][:l][,...]`: at that game frame the game thread stops for `<ms>`, disc
reads and the audio callback are held as long, every cached disc handle is closed behind its cache's back
(what a real resume leaves them as), the log's handle too, the card image's, the vcount jumps back 30 000
refreshes, then the resume is announced (`q`: not announced, only the failed reads and writes find it;
`l`: at the first frame from `<frame>` on with disc reads under way, i.e. mid-load). After it the slot-1
card's first page is written back unchanged (the card write path after a resume).

Lines to look for: `[resume-test] ... suspended`, `[resume] read of ... failed ... reopening`,
`[resume] ... reopened, the read went through`,
`[resume] memory card ...`, `[resume] log file opened again`, then `[frame]` / `[stats]` lines going on.

## Results (Vita3K 222 MHz, 3 cores, muted)

| Run | Build | Route | Suspends | Outcome |
| --- | --- | --- | --- | --- |
| tA1 | p1 | attract | 3 s, 5 s (q), 30 s, all mid-load | every NICO.DAT read on a closed handle failed (0x80010051) and went through on the reopen (try 1); card write ok; on to 9 014 frames (t=360 s) |
| tW3 | p2 (EE deadlines moved on a resume) | walk (console card, Load Game) | 5 s at frame 410, mid-load | **stuck at frame 410 for good** (VBlanks on, game idle): the EE change, now removed |
| tW4 | p2 | walk | 5 s (q: no resume notice, so no EE change) | fine: 2 401 frames at t=120 s |
| tW5 | p3 | walk | 5 s at frame 405, mid-load | reads reopened, 2 239 frames at t=80 s |
| tW6 | p3 | walk, SPU2 on, `SOTC_LOG_STDERR=0` (the console's logging) | 5 s at 413 mid-load, 8 s (q) at 2637 mid-load, **60 s** at 4200 | after each: reads reopened, card write ok (the quiet one through the failed-write path: reopened, try 2), game back within ~5 s and on to 9 138 frames (t=570 s), the sound engine's 5 s lines carry on right after the 60 s stop, pictures: Wander walking the shrine stairs before and after |
| cW1 | p3 | walk, no suspend | - | 2 943 frames at t=150 s (the normal path) |

`spalign.py p3.elf`: 868 NEON :128/:256 accesses, 0 sp-based.

## What to try on the console

See the V2 row of `docs/rounds.json`; in short: press PS mid-gallop and come back after 10 s and after 2 min;
press PS during a loading fade (area change / Load Game) and come back; put the console to sleep (power button)
for 1 min and for 10+ min, during gameplay and during a load; then save at a shrine. Each should continue (a
short hitch on return is expected). `err.txt` then shows `[resume] #n: power: resumed ...` and, at the first
disc read after it, `[resume] read of ... failed ... reopening it` / `... reopened, the read went through`.
