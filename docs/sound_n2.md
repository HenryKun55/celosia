# N2: the sound native, step one (2026-11-19)

Round N2's brief (the user, 2026-11-19: "nada de ps2 emulado, fakeado, nada disso! tudo nativo, tudo!"): the sound
was still PS2-shaped. The game's SG2 engine (237 `Sg2*` functions, ~42 KB of MIPS: voices / slots, sequence players,
sample banks, ADPCM streams) runs as recompiled code and talks to a host stand-in of the IOP driver (Sg2Driver,
round 2026-10-16) through a guest command ring and a SIF RPC (server 0x11223344) once a sound tick; a host mixer
(SoundMixer) plays SPU2-style voices with PS2 envelopes and reverb, on the audio thread, which took 10-25 ms of every
60 ms console frame before round 90 (round 90 cut it by a half to two thirds in Vita3K).

Switch: `SOTC_N2SND` (A/B name `n2snd`; sotc_decomp switch `kN2Snd`, the last of the tables; default on,
`SOTC_N2SND=0` off). It turns on everything below together (the sound tick copies it into the mixer's
`audio_stats::n2Mix` each tick, so an A/B window flips the mixer too). Code: game/overrides/sound_n2.cpp,
PS2Recomp/ps2xIOP/src/native/sound_mixer.cpp, iop_sg2.cpp (`Sg2Driver::serve`), ps2_sg2_direct.h.
Scripts, runs and logs: build/agent-scratch/n2/ (clone.sh / build.sh the Vita build dir cloned from S21's, chain*.sh
the detached chains, run.sh / v3k.sh the Vita3K runs in its own tree ../vita3k-celosia-n2 (222 MHz, 3 cores,
audio-volume 0), harness.cpp the Mac mixer harness, tools/wavcmp.cpp the recordings' comparison, vq.out).

## 1. The engine's remote layer on the host (no command ring, no RPC)

The original (MIPS read from the recompiled listing):

- `Sg2RemoteSend` (0x1d2010): between DIntr / EIntr, puts (cmd, a, b, c) at the ring's head (head 0x287500, 512 x 16
  bytes at 0x287540), head = (head + 1) mod 512; v0 = 0.
- `Sg2RemoteTickProc` (0x1d21e8, slot 4 of Sg2CtrlTickProc's table, run by tickProcSound when the RPC is not busy):
  the DMA ring (0x28b740, 32 entries of 0x14 bytes, index 0x28b72c): an entry at -1 becomes -2, the reply's busy word
  (0x28b6c4) 1, and the entry's four words go out as a command (0x13, a sample transfer) through Sg2RemoteSend; the
  ring's pending commands copied to 0x289540 (two memcpy for the wrap) and sent with `sceSifCallRpc(0x28b700, 0, no
  wait, 0x289540, n * 16, 0x28b540, 400)`; tail = head; the DMA entry at -2 whose transfer the reply says is done
  (busy 0) set back to 0, the index moved on (mod 32).

N2 (`sound_n2::sg2RemoteSend` / `sg2RemoteTickProc`): the four words go into a host queue; the tick does the DMA ring's
steps as above, and hands the queue to the driver straight: `ps2xSg2Serve` -> `PS2IopTransport::sg2Serve` (the IOP's
clock caught up and the call scope entered, as the RPC path does) -> `Sg2Driver::serve`, the RPC's own body split out
of `handleRpc`: the same command handlers in the same order, round 139's no-wait queueing, the same 400-byte reply,
written straight into the game's status block. No guest ring, no send-buffer copy, no SIF client / server records, no
RPC request. Commands left in the guest ring (sent while the switch was off) go first; turning the switch off spills
the host queue into the guest ring before the original tick runs. Not loaded native driver (`PS2X_NATIVE_SG2=0`): the
originals. `SOTC_N2SND_CHECK=1`: Sg2RemoteSend also runs the original and each tick compares the guest ring's pending
commands with the host queue; `SOTC_N2SND_LOG=<ticks>`: `[n2snd]` counts.

## 2. Sg2SlotTickProc on the host (the voice slots' per-tick update)

`Sg2SlotTickProc` (0x1d7498, 622 instructions, every sound tick): the 48 voice slots (0xE0 bytes each at 0x28c630;
the field map is in sound_n2.cpp): a slot whose voice died away (the driver's envelope reply below 2, three ticks after
its key on) reset to its defaults; the volume / pan / note / bend slides (EE floats), the pitch LFO, the portamento;
changed volumes out as command 1, changed pitches as command 4; then the reverb sends (9 / 0xA when changed) and the
tick's gathered key ons (7) and offs (8). N2's `slotTickHost` is the same operations in the same order, the floats
through the recompiled code's own FPU helpers (`FPU_ADD_S`, `FPU_CVT_W_S`, `FPU_C_OLT_S`, ...: the EE's arithmetic and
compares), the integer products 32-bit as the EE's, the commands into the host queue; DIntr / EIntr left out (nothing
preempts host code). `SOTC_N2SLOT=0`: the original; `SOTC_N2SLOT_CHECK=1`: every tick the host routine and the
original run from the same slot table, globals (0x28c5f0..) and queue; the memory and the commands are compared.

## 3. The mixer's N2 paths (sound_mixer.cpp)

All exact (the same samples and state as round 90's paths):

- **Each ADPCM block decoded once per history.** A block's 28 samples depend only on its 8 words and the two samples
  before it, so a direct-mapped table of 2 048 decoded blocks (by block number, ~132 KB) keeps them with the history
  they were decoded from; a voice entering a block with the same history copies them (a sample played again, a loop's
  next pass). Any write into sound RAM drops the blocks it covers (`ramWritten` -> `dropDecoded`, whatever the switch
  says); blocks inside a reverb work area (written by the reverb itself, not through writeRam) are never kept.
- **The voice's gather and mix fused, four outputs at a time (NEON).** Each output's four history samples one vector
  load, the 4 x 4 transposed in registers, straight into round 23's interpolation and the sums (no h0..h3 / count
  arrays); the last outputs (fewer than four) through mixSamples.
- **The reverb's 24 kHz steps of a block in one loop** (`reverbRunN2`, renderCores90's linear case): the preset, the
  28 pointers and the area read once, the log's counters only while the log is on, the written words' largest
  magnitude only for round 90's residue clear.
- **The audio thread's piece 64 frames** (was 32): the per-block work (envelope runs, the reverb's pointers, the voice
  loop) halved; the driver's status request does not wait for a piece (round 139) and is now a direct call.

`PS2X_MIX_CHECK=90` with N2 on compares N2 (path 2) against round 90's paths (path 1) on a copy of the mixer.

## 4. Checks and measurements

All runs on the p3 build (Vita3K 222 MHz, muted), scripts `build/agent-scratch/n2/chain3.sh`, results in
`build/agent-scratch/n2/vq.out` and `runs/`.

- **Checks (exact).** chkA (attract) and chkR (ride7) with `SOTC_N2SND_CHECK=1 SOTC_N2SLOT_CHECK=1 PS2X_MIX_CHECK=90`:
  512 slot ticks checked against the original Sg2SlotTickProc, 0 differ; 12 288 mixer pieces (64 frames each)
  checked against round 90's paths, 0 differ; 0 queue / ring difference lines. The boot run's own check: 36 000 ticks,
  88 507 commands, 31 transfers, 0 left over, 0 differ.
- **Recordings** (`PS2X_SPU2_WAV`, N2 on vs off, `tools/wavcmp`): attract: the two recordings are the same sample for
  sample (difference -141 to -300 dBFS a 5 s window) for the first 205 s; after that the demo's own timing drifts
  between the runs (same level and bands on both sides, difference at the level of the signal). Ride7: the riding's
  sounds follow the frame timing (hooves, wind), so the runs drift from 3.7 s on; RMS -25.2 / -25.2 dBFS and the nine
  bands within 1 dB. The bit-exact proof is the in-run checks above; the recordings show no missing or extra sound.
- **Mixer cost** (`SOTC_AUDIO_LOG=1`, mix line averages): attract 1.61 ms (N2) vs 1.94 ms (off), ride7 2.94 vs
  3.19 ms: -0.33 / -0.25 ms of audio-thread time.
- **Boot** (bootR: 600 s ride7, 89 MB taken, N2 on): booted on the first try, mounted Agro at frame 5 852, no fault;
  pictures normal (riding, ground, sky, Agro). spalign on p3.elf: 0 sp-based :128 / :256 NEON accesses (of 1 000).
