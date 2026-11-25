# Round G1: vitaGL build flags and the render thread's own GL usage (2026-11-25)

Goal: 30 fps stable on the console, picture unchanged. This round asks what vitaGL's "speedhack" build flags and
our own use of vitaGL can take off the render thread's CPU. Scratch: `build/agent-scratch/g1/` in the main checkout
(packages, env files, README.txt, run captures).

## Where the render thread stands (console, run-20261125-k2pmu)

Riding (frames with ~920-1 060 draws): the frame 40-42 ms, the game thread 38-41 ms (`game`), the render thread's
draw part 23-25 ms wall (`render`) for 16.6-18.4 ms of CPU (`cpu render`), 11 target binds and 12 scenes a frame,
~300 KB of uniforms a frame. The render thread runs on core 2 (0x40000) with the worker / offload / lane / loader
threads (cores 1-2); the game thread alone on core 0. The frame is set by the game thread, not by this thread: render
CPU saved is core-2 time given to the threads that feed the game thread, not frame time directly.

What the render thread's CPU is made of (Vita3K profile of the ride, M1's `runs/profR` with `p1.elf`; the renderer
is unchanged since 2026-11-18, so it holds for main): `sceGxmDraw` 23.2%, the draw loop 14.2%, `planFrame` 13.0%,
`sceClibMemcpy` 6.7%, `_glDrawArrays_CustomShadersIMPL` 4.2%, `uniform4` (two clones and a long-branch veneer)
4.8%, `sceGxmSetVertexStream` 2.1%, `glVertexAttribPointer` 1.7%, the vertex layout lookup 1.2%, `celo_vbo_check`
0.9%. In Vita3K every sceGxm / sceClib call is an emulated import (its share is larger than on the console).

## 1. The vitaGL build flags, read in vitaGL's source (rev 464876a + celosia.patch)

Today (`./celosia vita`, r110's tree): `NO_DEBUG=1 STORE_DEPTH_STENCIL=1 HAVE_SHARK_LOG=1 NO_SPLASHSCREEN=1
HAVE_VERTEX_LAYOUT_CACHE=1 HAVE_SHADER_CACHE=1`. My rebuild with these flags matches r110's libvitaGL.a section for
section (the same .text / .data / .bss of every object).

Each candidate built alone (`build/agent-scratch/g1/vgl.sh <name> FLAG=1`); the objects whose code changed
(`arm-vita-eabi-size`, .text bytes) and what the changed code does:

| Flag | Code changed | What it does in vitaGL | Our usage | Verdict |
|---|---|---|---|---|
| `SAMPLERS_SPEEDHACK` | custom_shaders.o -144 | the draw paths drop the `if (p->frag_texunits[i])` / `vert_texunits[i]` NULL check for each sampler slot below the program's highest resource index | no sampler objects (0 `glBindSampler`); a program with a gap in its sampler resource indices would read NULL. The boot's warm-up draws all 71 programs x 12 blend states, so a gap would crash at boot | safe if it boots; gain a load + branch a texture unit a draw (~2 000 a frame: microseconds) |
| `TEXTURES_SPEEDHACK` | textures.o -2 276, gpu_utils.o -556, custom_shaders.o -264, framebuffers.o -260, ffp.o -88, gxm.o -56 | no `last_frame` per texture; `glTexSubImage2D` on a texture drawn in the last 4 frames writes in place instead of into a new copy | the native renderer never calls `glTexSubImage2D`, but raylib's `UpdateTexture` does (ps2_runtime.cpp: the emulated GS picture's upload each frame, the native picture's copy): it would be overwritten while queued scenes still read it | **rejected** (a picture risk on the GS / copy paths for a store a draw) |
| `BUFFERS_SPEEDHACK` | buffers.o -580 | `glBufferSubData` writes in place instead of into a new buffer when the buffer was drawn recently | not called on the Vita (the pools are `glMapBuffer`'d once, the arenas `glBufferData` + `glMapBuffer`) | no effect (code never runs) |
| `INDICES_DRAW_SPEEDHACK` | draw.o -296 | `glDrawElements` with indices in client memory draws from that pointer instead of a GPU copy | every indexed draw has an element buffer bound | no effect (code never runs) |
| `CIRCULAR_POOL_SPEEDHACK` | vgl.o -360, gxm.o -144 | one data ring for every frame instead of one per display buffer: a wrap writes over data queued scenes still read | the draws read VBOs and celosia.patch's own uniform pool, not this ring | **rejected** (no gain, a wrap risk) |
| `DRAW_SPEEDHACK` | does not compile | attribute pointers taken as GPU memory as given, one stream per attribute, and the vertex layout cache compiled out (`#if ... !defined(DRAW_SPEEDHACK)`) | celosia.patch's single-stream path uses `is_packed` / `target_vbo`, which the flag removes (compile errors); the layout cache is a measured win for us (rounds 13 / 74) | **rejected** (would cost more than it saves: 4 streams a draw and no layout cache) |
| `NO_TILE_CLIPPER` | gxm.o -88, tests.o -104 | no `sceGxmSetRegionClip` at scene starts / scissor updates (the scissor itself, a stencil pass, stays) | the native renderer sets no scissor | identical picture; one GXM call a scene (12 a frame) |

gtasa_vita's `DRAW_SPEEDHACK TEXTURES_SPEEDHACK SAMPLERS_SPEEDHACK` set fits a game that draws from GPU-mapped
client arrays and never updates live textures; ours draws from VBOs through the layout cache, and two of its three
flags are wrong for us. Nothing in any flag touches the costs that matter here (`sceGxmDraw`, the uniform buffer
copies, the stream / program state words): the flags' combined gain is at most a few microseconds a frame.

Combined safe set built as `vitagl-flags`: `SAMPLERS_SPEEDHACK=1 NO_TILE_CLIPPER=1 BUFFERS_SPEEDHACK=1
INDICES_DRAW_SPEEDHACK=1` (with the base flags).

## 2. Our own use of vitaGL

The per-draw sequence (native_renderer_gl.cpp's draw loop) was already trimmed by rounds 13-112: the vertex buffer,
program, blend, depth test / mask and texture are set only when they change; each uniform is compared with the
last value its program got and sent only when it differs; the frame's meshes are drawn from VBOs (mapped pools,
the arena through `glBufferData` + `glMapBuffer`), so vitaGL never copies client arrays; one vertex stream a draw
(celosia.patch); no `glReadPixels` outside diagnostics and one 16x16 boot check; no immediate mode in the 3D path;
no sampler objects; no index copies (element buffers bound). vitaGL itself copies the default uniform buffers only
when a uniform changed (`dirty_shader_*_unifs`), sets textures / programs only on a change, and its `glDrawArrays`
is a handful of checks around `sceGxmDraw`. What remains is mostly GXM's own per-draw cost and the size of the
vertex default buffer (a skinned program's `uPalette[192]` is 3 KB, copied whenever any vertex uniform changes).

Two clear, picture-neutral changes, both off by default:

1. **`SOTC_GL_G1=1`** (2: on in odd 600-frame windows, off in even ones, `[g1]` lines; for A/B in one run): the
   draw loop's 28 uniform sends go through `uniform4g1`, an always-inlined copy of `uniform4` that compares all of
   a uniform's words or-ed together (no branch a word) and calls `glUniform4fv` with the same values in the same
   order. In the profile `uniform4` was 4.8% of the render thread, in two out-of-line clones plus a long-branch
   veneer (`___ZN12_GLOBAL__N_18Renderer8uniform4..._veneer` 0.48%: the function lies farther than a branch's
   reach from the loop). The ELF grows by 9.8 KB of text (the loop's call sites).
2. **`SOTC_VGL_GC=<priority>,<core mask>`** (read at boot, before `vglInit`): vitaGL's garbage collector thread,
   woken at every swap, gets that priority and affinity (`vglSetupGarbageCollector`). vitaGL's default is the
   user default priority (160, above the game thread's 191) on any core, so its per-swap work can preempt the
   game thread on core 0. The console env uses `191,0x40000` (the render thread's core and priority). It only
   frees memory: the picture cannot change.

Considered and left: a per-switch attribute-enable cache (glEnable/DisableVertexAttribArray are 0.2% together;
other passes change the arrays between switches, so the cache would need invalidation everywhere); moving the
skin palette out of the default uniform buffer (needs GXM uniform buffers through vitaGL's GLSL translator:
shader changes, not a G1-sized step); sorting draws by state (changes the draw order: not picture-identical).

## 3. Correctness (Vita3K, 222 MHz, 89 MB eaten)

Method: `SOTC_GL_FREEZE=3000,800` (from hand-over 3000, the render thread draws that frame again and again) with
`SOTC_FIXED_STEP=1`, window captures every 20 s; `pixdiff.sh` crops the title bar and the 112 picture rows under it
(macOS blends them with the translucent bar: up to 2 levels of noise between two captures of one still frame, seen in
the baseline alone) and masks the fps overlay. The route is not frame-exact across runs (Wander's position already
differs by 0.14 at frame 2000, and the frozen hand-over held 1 002 / 972 / 808 draws in the three runs), so pixel
identity is proven inside one run (SOTC_GL_G1=2) and cross-run pictures are compared by eye.

| Run (package, env) | Boot | Frozen frame | Captures after the freeze |
|---|---|---|---|
| fzBase (base.vpk = main, ref vitaGL) | try 1, warm-up 71 programs x 12 blends | 1 002 draws | 0 differing pixels, t0180-t0480 (16 captures) |
| fzG1 (celosia-g1.vpk, `SOTC_GL_G1=2`, `SOTC_VGL_GC=191,0x40000`) | try 1; `[vita] vitaGL garbage collector: priority 191, cores 0x40000` | 972 draws | 0 differing pixels, t0240-t0480 (13 captures over 15 on / off toggles of G1) |
| fzFlags (celosia-g1flags.vpk, switches off) | try 1, warm-up 71 x 12 drawn (SAMPLERS_SPEEDHACK: no NULL sampler slot) | 808 draws | 0 differing pixels, t0220-t0480 (14 captures) |
| shrBase / shrFlags (the altar intro, captures every 3 s) | try 1 / try 1 | not frozen | the same scenes; the doves' blurred white shapes in front of the camera at the altar appear in both (base 39-42 s, flags 39-60 s: the intro's length follows the loads) |
| rideAll (celosia-g1flags.vpk, G1 on, GC placed, 600 s) | try 1 | not frozen | mounted at frame 4 678, rode to 600 s; captures every 60 s normal (field, the bridge, the cliffs) |

Every pair of captures was looked at (base / flags / G1 at the same seconds, the frozen frames); nothing out of
place: textures, shadows, Agro's and Wander's skinning, fog, the glow and the bloom as in the baseline.

Speed in Vita3K (not the console's numbers): fzG1's `[gl:phases]` lines on the frozen frame, 15 windows each way:
passes 24.82 ms (on) / 24.39 (off), second half (the host quieter) 23.44 / 23.50; render 29.50 / 29.13. No change
beyond Vita3K's noise, as expected for a few tens of microseconds a frame.

## 4. Console packages

`build/agent-scratch/g1/README.txt`. Two packages: `celosia-g1.vpk` (main + the two switches, off by default; the
vitaGL we ship) and `celosia-g1flags.vpk` (the same with `vitagl-flags`). Env files (all with K2's
`SOTC_K2=1 SOTC_M1PMU=300 SOTC_FRAME_LOG=1 SOTC_PROF129=1 SOTC_LOG_STDERR=0 SOTC_AUDIO_LOG=0 SOTC_BODY_SHADOWS=1
SOTC_FRAME_SLOW_MS=100`): `env-g1-base.txt` (= main), `env-g1-on.txt` (+ `SOTC_GL_G1=1 SOTC_VGL_GC=191,0x40000`),
`env-g1-ab.txt` (+ `SOTC_GL_G1=2 SOTC_GL_PHASE_LOG=1`). Order: g1 + base, g1 + on, g1flags + on. Expectation:
within noise; if `cpu render` / `game` do not move by more than ~0.3 ms, neither the flags nor G1 is worth
keeping and the render thread's CPU has to come from fewer draws or GXM-level work (e.g. the skin palette out of
the default uniform buffer), not from vitaGL's build.

Builds: base / g1 / g1flags 0 errors, no "segment 1 overlaps" (ELF room 122880 unchanged), spalign 0 sp-based of
2 862 hinted NEON accesses in each; g1's text +9 760 bytes over base (the inlined sends), g1flags -1 390 under g1.

## Files

- `PS2Recomp/ps2xRuntime/src/lib/native/native_renderer_gl.cpp`: `uniform4g1`, `G1U4`, the `SOTC_GL_G1` switch.
- `PS2Recomp/ps2xRuntime/platform/vita/raylib/raylib_vita.cpp`: `SOTC_VGL_GC`.
- vitaGL itself unchanged (celosia.patch untouched); the flag sets are build arguments only, recorded here:
  base `NO_DEBUG=1 STORE_DEPTH_STENCIL=1 HAVE_SHARK_LOG=1 NO_SPLASHSCREEN=1 HAVE_VERTEX_LAYOUT_CACHE=1
  HAVE_SHADER_CACHE=1`, safe set + `SAMPLERS_SPEEDHACK=1 NO_TILE_CLIPPER=1 BUFFERS_SPEEDHACK=1 INDICES_DRAW_SPEEDHACK=1`
  (`./celosia vita` keeps the base set; the safe set would go in its make line only if the console shows a gain).
- Scratch `build/agent-scratch/g1/`: vgl.sh (variant builds), build.sh, fz.sh / run.sh / v3k.sh, pixdiff.sh,
  packages, env files, README.txt, runs/*/err.txt + 2 PNGs each.
