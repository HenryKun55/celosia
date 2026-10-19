# Native renderer: map of SotC's drawing (from the game's symbols)

What the game calls to draw, found with Ghidra on SCUS_974.72 (the ELF
keeps its function names). The goal is a Metal renderer fed at this level
(model + matrix + material), with no VIF/VU1/GIF/GS emulation.

## Display lists

The game never draws directly: every system appends prebuilt DMA chains to
a display list split in layers (`iosDlPushLayer(n)` / `iosDlChainTailCurrent(addr)`
/ `iosDlPopLayer()`), and `transMicroCode(n)` (0x1114b50) appends the
upload of VU1 microcode n (table at 0x118cd90) when the layer's current one
differs. The layers are sent to the VIF1 once a frame.

## Models (`model*`, 0x1096840-0x1099980)

Objects, scenery and characters go through one model system. A loaded
model header (`modelInitialize`, `modelCreatePacketTransPacket`):

| offset | contents |
|--------|----------|
| +0x08 | owner set at load |
| +0x0c | non-zero: second fade factor applies |
| +0x14 | packets built at load: [0] one per draw packet (0x30 bytes), [1] one per surface (0x40) |
| +0x18 | flags (bit 0: LOD model), +0x1c LOD count |
| +0x20..+0x2c | bounds (copied to the display instance) |
| +0x30 | per LOD: clip box (8 corners tested with VCLIP) and the packets of that LOD |
| +0x40 / +0x44 | textures (0x20 each; +0x10 the texture resource) / count |
| +0x50 / +0x54 | surfaces (0x120 bytes each) / count |
| +0x60 / +0x64 | draw packets (8 words each) / count |

Surface (0x120 bytes): +0x18 flags (64-bit: alpha, two-pass, shadow,
underwater, boss effect, layer selection in `modelGetDlLayer`), +0x1c
texture mode (1 one texture, 2 two), +0x2c / +0x3c texture indices, +0x60
GS register packet (A+D: TEX0, ALPHA, TEST...), +0xc0 second-pass registers,
+0x10c alpha.

Surface flags read by `modelDisp` (bit n of the u64 at +0x18):
29 never
drawn; 17 boss effect (drawn only while `getNowBossEffect`, the word at
0x1186290, has bit 0); 24 shadow effect (bit 1 of the same word); 26 hidden
while the underwater fog (word at 0x1186328) is on.
Layer (`modelGetDlLayer`): 0x1020000 -> 8, bit 23 -> 10, 25 -> 16, 9 -> 17,
10 -> 12/13, 14 -> 1, 15 -> 18, else 2/3 (5/6 for the second owner), the
odd one when bit 8 is set or the instance fades (+0x244 > 0). Blending:
ALPHA_1 in the +0x60 packet, 0x44 normal, 0x48 additive.

Draw packet (8 words): [0] size in bytes, [1] LOD, [2] surface index, [3]
part (-1 = always), [4] address of the vertex block, [7] its DMA chain:
`CALL` surface registers (+0x60), `REF` vertex block (VIF data: the batch
the world microcode reads, see game/overrides/sotc_vu1_world.cpp), `MSCAL`
to the microcode, `RET`.

`modelDisp(instance, matrix)` (0x1097118): camera x model matrix, VCLIP of
the LOD's box (skips it off screen), then per packet of the current LOD:
layer from `modelGetDlLayer`, texture uploads (`texTransRequest[Multi]`)
when the surface changes, `iosDlChainTailCurrent(packet chain)`; surfaces
with flag bit 24 or 0x2200 add a second pass (layers 9/14/15, registers
from the per-surface packet). Callers: the object display functions
0x10F3B58 / 0x10F3DE8 (they also call `shadowDisp`). Instance setup:
`modelInitDispModelClass` (from 0x10EF9D0): lights (`setLight`), unit
matrix, specularity, the layer mask (+0x96 words).

Two-texture surfaces (+0x1c = 2; textures +0x2c and +0x3c; batches with
V4-16 holding both ST pairs): the microcode draws each batch twice, the
GIF tags at +0xc0 selecting context 1 then 2: the first texture blended by
ALPHA_1, then the second by ALPHA_2 (0x44 blend, 0x48 add, 0x42 subtract
Cd - Cs x As: carvings and dirt darkened onto a lit base).

Batch formats (STCYCL n+1: n fields per vertex; the UNPACK address is the
field): 0 GIF tag, 1 position V3-32, then V2-16 ST + V4-8 colour (unlit
scenery, ~90%); V4-16 two ST pairs + V4-8 (two textures); V3-32 normal +
V2-16 + V4-8 (lit); V4-16 normal + V2-16 + V4-8 + V4-32 skin (skinned).
Skin: w = bone count, x/y/z = bone index + weight in the fraction (one bone:
full weight).

Characters are not one model: Wander is ~24 `modelDisp` calls (body,
limbs, hair, clothes), each a model of its own. The instance chain
(instance + 0xc) is a NEXT tag past the instance's 15 qwords (lights,
colours) to a RET packet: rigid parts unpack one matrix to VU1 qword 0,
skinned parts the bone palette (up to ~75 matrices, `modelSetMatrixTransTag`),
each bone x view, in camera space. Their clip transform is the projection
alone: camera context + 0x40 (camera + 0x5a0 = that x view).

The instance's own 14 qwords (to VU1 0x3d8) start with its lights, as the
lit microcode (loop 0x1818) uses them: qwords 1-4 the light matrix L
(columns: normal x, y, z, 1), 5-8 the colour matrix C (light 0-2 colours,
ambient); d = max(0, L x (n, 1)), d.w = 1, vertex colour x= C x d (0x80 =
1, clamped to 255). Skinned normals go through the bone matrices (camera
space) with the same lights.

`modelClipSkin` only picks the LOD of skinned models.

Skinned characters: `modelClipSkin` (0x1098450); 0x1098CC0 selects 14
microcodes (characters).

Cloth and hair (`meshCreate` / `meshDisp`, 0x109ab80): grids simulated on
the CPU, ~15 a frame in the shrine (Wander's cloak, Mono's dress). The "#"
object's +0x20 block: [0]/[1] two buffers, [2] rows, [3] columns, [4] the
current one. A buffer: DMA/VIF header, the view matrix (qwords 4-7), then
per row an UNPACK and columns x 4 qwords: world position, normal (world),
ST(Q), colour as floats (0x80 = 1). Each row joins the next as a strip. The
texture is whatever the caller requested just before.

Shadows: `shadowDisp(instance chain, shadow model)` (0x109e0e8, layer 3 or
7; t2 = 0: microcode 0 + 8, rigid; t2 != 0: microcode 5 + 9, skinned, the
second qword per vertex x/y/z = bone + weight, w = n, n + 1 bones). In the
route only Wander's sword has one (model 0x33dfc0, V3-32 strips). Microcode
8 takes each triangle to camera space (VU 972, the chain's RET matrix),
extrudes it by the w of VU 984-986 (the chain's qwords 1-3), cuts each
extrusion edge at z = 0.1 and draws the prism into its own buffer with
colour 1 or 255 by screen facing, ALPHA 0x68 (add) and COLCLAMP 0: a
stencil count modulo 256 against the scene's Z. `shadowDraw` reduces that
buffer (256 -> 128 -> 256) and darkens the picture by it.

Natively: stencil volumes (front +1, back -1, depth clamped so the caps at
z = 0.1 stay), the counted pixels reduced 4 x 4, blurred and darkened by
0.5 (`SOTC_SHADOW_AMOUNT`; `SOTC_SHADOW_HARD` darkens without the blur,
`SOTC_SHADOW_SHOW` draws the volumes flat, `SOTC_NATIVE_NOSHADOWS` turns
them off). The emulated GS used to clamp where COLCLAMP 0 wraps, which
turned the sword's thin shadow into a dark wedge wherever the volume's back
faces showed; it wraps now.

Water refraction: `waterRefractionBegin` (layer 9) copies the picture so
far to 256 x 256, to 128 x 128 and back (a blur) and leaves TEX0 on it; the
0x2200 surfaces are drawn there (and again in layer 15 on the reflection)
with the per-surface packet (header +0x14 [1] + s x 0x40 + 0x20), tint at
the CALL target +0x30 (alpha 128). Layer 9 (microcode 2, VU1 image 87bc2f1b) draws the copy's
colour as is (RGB 128) with the vertex alpha under PABE (below 0x80 not
blended: opaque); layer 15 (microcode 1, 98a396b7, routine 0x648) draws
reflectionBegin's rippled mirror (fbp + 0x20: 32 bands of 8 rows shifted by
(sin(2 (12 i + t) 24 pi / H) + 1)(6.4 i / H + 0.2) texels, t = *0x118c030,
H = *0x1192090) times the vertex RGB, alpha = vertex alpha x (VU 0x3f3 w /
128) x (0.2 + 0.8 (1 - |n . v|)), v the normalised camera-space position, n
the model's up axis (a Fresnel term: from above the pool bottom shows, at a
grazing angle the mirror). Bit 25 surfaces go to layer 14 instead (unrippled
mirror, ALPHA 0x8000000064: written as is). Natively:
`refractionSample` draws at layer 9 (`SOTC_NATIVE_NOREFRACT`,
`SOTC_REFRACT_SHOW` paints them red).

Immediate gl* (glBeginWithDlLayerNo 0x108bb38 .. glEnd 0x108d060): a GIF
packet built on the CPU at *0x1186398 (DMA tag, GIF tag A+D at +0x10,
then (value, register) pairs to *0x1186394: TEST, PRIM, per vertex RGBAQ,
ST or UV, XYZ2/XYZF2 in screen space), chained by glEnd in the layer
glBegin pushed. Natively glEnd's pairs go through the 2D assembler (no
depth test). In the opening: a textured ribbon at layer 23 (0x10f4724).
The solar flare mode 2 (0x109fb08, `solarFlare` when *0x118c090 == 2) only
runs in the first boot frames; mode 1 is the bloom chain.

Depth of field: `depthOfField` (0x10874e8, layer 20, while 0x11862d0 is
set) copies the picture to 256 x 256 (rounds 0x11862d4 of 256 -> 128 ->
256), then lays it over the frame in 0x11862d8 slices: sprite i at GS Z =
focusZ >> i (focus 0x11862dc taken through the camera's 0x4a0 matrix),
ZTST GEQUAL, alpha (0x80 / slices) x (i + 1). Natively: `setDepthOfField`,
one pass reading the depth (`SOTC_NATIVE_NODOF`).

Mip levels: an NTO2 texture has (format >> 12) & 15 levels, each half the
last, stored one after the other (byte +0x1f: a swizzle bit per level).
texTransResolve uploads them (MIPTBP1/2) and sets TEX1 from the header:
MXL = levels - 1, MMAG +0x74, MMIN +0x78, L +0x94, K = +0x90 x *0x118c180
(7.4 fixed). The GS level is log2(1/Q) x 2^L + K with Q = 1/w. Natively the
fragment stage picks the level the same way (FogParams lodK / lodScale /
maxLod); Metal's own derivative LOD blurred the floors.

VIF ROW: every ST unpack (V2-16, and the two-texture V4-16) uses STMOD 1
(offset). texTransResolve builds, for each requested texture, a RET packet
(DMA 0x6000000c, FLUSHE, STROW, DIRECT 7 with the GS texture registers) in
the display list; the STROW's row is the 16-bit pair at *(NTO2 header
+0xc) and +4, which the game moves every frame (water, clouds; layer 1 sky
too). Natively the row / 4096 is added to S and T per draw
(`DrawState::uvOffset`, `SOTC_NATIVE_NOROW`); the second texture's pair
takes ROW z / w, 0.

Particles (`ptclman*`, microcode 7): an environment packet (type, a
matrix, the ST extent `vec`, fog factors, VU 18..27), a material packet
(ALPHA by its argument: 1 adds, 2 subtracts, else blends) and vertex
packets of 48-byte particles: position + size, colour 0..1, sin/cos of the
rotation. The microcode expands each into a rotated square (s = size x
0.7071; A = (k.x sin, k.y cos) s, B = (-cos, k.y sin) s, k at 0x118bfe0;
type 1 along the billboard axes from *0x118bff4, 2/4 by the matrix),
type 4 as a lit fan around the centre (routine 0xe48: centre normal the camera's third axis, corners toward the centre; L = VU 26, k = VU 27; s = min((n.x L.x + n.y L.y + k.w n.z L.z + k.w) / 2, 1)^2, colour x (s k.y + k.x)); types 2, 4 and 7 add the matrix's translation row; type 5 (0xba8) the type 3 streak drawn as type 4's lit fan around the corners' mean; type 3 a streak between two points
(q0 = p1 and width, q2 = p2; sides (p - eye) x (p1 - p2), half the width,
the eye VU 16), type 7 a square by the environment matrix with its
translation and a texture cell offset per particle (q2 z / w). The
environment packet unpacks the 18 qwords at *0x118bff4 to VU 0-17 (12-15
the world-to-clip matrix, 16 the eye), then VU 18-27.

## Other systems that append chains or microcode

- `mesh*` (0x109a4d8-): `meshDisp`, `meshDispInstance`, `meshTransMatrix`,
  `meshTransLightMatrix`; callers 0x1138050, 0x1383048.
- `shadow*` (0x109d158-): `shadowDisp`, `shadowDraw`.
- particles `ptclman*` (0x1099e00-): material, vertex and environment packets.
- `waterRefractionBegin/End` (0x109c108).
- sprites / text: `kPrintf*`, `spriteTexFast`, `getSpriteDraw`; `bgDraw`
  (0x1085258) is a full-screen fog sprite (from `zFog`), not scenery.
- line objects (debug and effects), `dispVuColiDispWork`, `dischargeMain`.
- textures: `texTransRequest`, `texTransRequestMulti`, `texResetVRAMRequest`.

## 2D: sprite objects

Menus, the title, the map and the load/save boxes are sprite objects
(`spriteCreateObject`, 0x50 bytes: [0] texture resource (+0x20 NTO2), +4
RGBA, +8 alpha factor, +0x10..0x1c UV rectangle, +0x20..0x2c ST, +0x30/+0x34
position, +0x40/+0x44 scale, +0x48 rotation and flags: blend mode bits
40-43, ST bit 48) drawn by `spriteDrawSprite(obj, z, layer, ...)`
(0x10a32b8; floats f12-f17: position, scale, rotation, alpha factor). It
writes A+D registers with `gsReg(reg, value)` (0x1c5b00): PRIM (triangle
strip), RGBAQ, UV or ST, XYZF2 per corner (12.4, screen centred on 2048,
512 x 448), ALPHA (0x44 / 0x48 / 0x42 / 0x54 by mode), in display list layer
25 + layer. Title screen: ~9 per frame; memory card and map: ~200.
The recompiled function runs in pieces the dispatcher continues after the
call returns: hooks that wrap the call see none of its gsReg writes.

The same gsReg path: `spriteDrawSpriteUV/ST` (obj in a2) and `UV2/ST2` (obj
in a0) draw SPRITE primitives (two corners) in layer 26;
`spriteDrawSpriteTexNoSet(obj, z)`; `fadeScreen(amount)` (0x108a408, full
screen, colour at 0x1186340/44/48, layer at 0x1186350) and
`drawDemoBar(amount, layer, height)` (0x108a588, the cinema bars) blend with
ALPHA 0x64, (Cs - Cd) x FIX + Cd, FIX = amount x 128. `spriteNoTexFast`
writes its registers itself and is only called by screen effects (bloom,
shadows, reflection, fog, boss death).

All of the 2D goes through gsReg from a handful of functions (found with
SOTC_GSREG_CENSUS=1, gsReg calls by return address): the sprite family
above, fadeScreen / drawDemoBar, subtitles (0x134b490: pre-rendered lines,
one textured sprite, 64 texel rows per line, layer from the script), HUD
gauges (0x13f6b20 / 0x13f7178 / 0x13f75d0: ellipses of 44 segments, fans
and strips with vertex colour), the title menu backdrop (0x14033f0), the
loading indicator (0x1c2dd0: lines), windows (spriteDraw*, windowPrintf,
windowSprite*). Each asks for its texture with texTransRequest(resource)
just before drawing and pushes its display list layer. The other gsReg
callers are screen effects that draw into other buffers (0x10f37c8 frame
copy, 0x108ac18 / screenBlur, bossDeathEffect*, reflection*, shadow*,
waterRefraction*, drawFrontFog, 0x109e2a0 bloom).

## Distance fog (zFog, bgDraw)

The sky (surfaces in layer 1, flag bit 14) is drawn into a separate
background buffer, not over the scene: the dome is small and would hide the
landscape. After the opaque world (layers 2-3), `zFog` (0x10ac0a8, layer 4)
writes a fog amount into the frame's alpha by depth: N + 1 full screen
sprites at view depths far - margin - width + width x i / N (N, width,
margin at 0x118c46c/70/74, far at 0x118c480), alpha i x 128 / N, Z test
GEQUAL, so each pixel keeps the amount of the last slice in front of it.
`bgDraw` (0x1085258) then blends the background buffer over the scene by
that alpha (ALPHA 0x54: (Cs - Cd) x Ad + Cd): the distance fades into the
sky. `drawFrontFog` adds a flat colour on top (layer 0x14, colour and
amount at 0x118b8dc..f0, set by graphSetFogParam).

## Vertex fog (GS FOG)

Every world batch's GIF tag has FGE: the microcode writes F = vf17.w +
vf17.x / w per vertex and the GS mixes the colour with FOGCOL by F / 255.
The factors are getFogFactor0/1 (0x1089b70/bb0) from near/far distances
and rates at 0x12147dc..ec; the picture matches the line through (near,
nearRate) and (far, farRate) in 1 / w, which is half of getFogFactor1's
base. FOGCOL is graphSetFogParam's colour (0x118b8dc/e0/e4). The opening
canyon is mostly this fog (dark blue grey); the sky (layer 1) is not fogged.

## Layers that do not go to the screen

`0x1098e20` (from modelSetEnviroment) chains a camera packet (VU 0x3c0:
camera context +0x4a0 / +0x220 / +0x260 / +0x1a0, fog factors) to layers
2, 3, 5, 6, 8, 9, 10, 14, 15, 17, 18, a second one with +0x520 to layers 12
and 13, a third to layer 1 (sky). `reflectionBegin` (0x109ad58) opens layer
12 by pointing FRAME/ZBUF at an off-screen buffer (cleared to the sky
colour): layers 12/13 hold the mirrored world, layers 14/15 come back to the
screen and sample that buffer. `refractionBegin` uses layer 19,
`waterRefractionBegin` layer 9, both with buffers of their own.
Reflecting surfaces (flags 0x2200) are drawn again in layer 15 by
microcode 1 with the colour their per-surface pass packet unpacks to VU
0x3f3 (header +0x14 [1], 0x40 each, +0x20 a CALL to the constants; e.g.
33, 48, 46, 128); natively they sample the mirror image at their own screen
position (planar reflection). Drawing
layers 12/13 on the screen gave the grey curtains over the shrine and the
pit (they are the walls seen in a mirror, from a different camera).

## screenBlur (0x108a780)

screenBlur(angle f12, scale f13, layer a0, copy width a1, a2, steps a3,
dx t0, dy t1): the picture so far is copied (bilinear) and drawn back
`steps` times, copy i scaled (scale - 1) x (n - i) / n + 1, turned by
-angle x the same fraction (only past 0.0157 rad), shifted by (dx, dy) x
the same fraction, alpha 128 / (i + 1): the average of the copies. In the
cutscenes it runs every frame near identity (scale 0.99998), which only
softens the picture; larger values give the zoom blurs.

## Bloom (0x109e2a0, layer 0x14)

A mask of what lies beyond GS Z 0x118c094 (0 in the shrine: only pixels
with no depth, i.e. what the background buffer shows: sky and the far
landscape) in colour 0x118c0d0 (white), reduced, blurred with 12 shifted
sprites (+-1, 2, 3 texels; weights 0x40 / 0x20 / 0x10 at 0x118c0fc/10c/11c),
a second round when 0x118c12c != 0, lerped with the last frame's
(0x118c12c / 128 = 0x76), and added over the frame (ALPHA 0x48) in colour
0x118c140 (0xaf, 0xaf, 0xa5) with alpha 0x118c14c. The parameters chase
targets at 0x118c09c..b0 by 1/12 per frame. GS Z of a view depth w:
camera context + 0x4a0, zA + zB / w (242.5 + 834987 / w). The native
renderer redoes it with its own passes (quarter resolution, separable
blur, history texture). The 12 shifted copies blend by ALPHA 0x44 (a lerp,
alpha 0x40 / 0x20 / 0x10 at 0x118c0fc / 10c / 11c), so the blur is
normalized; the final add is full strength (colour 0x118c140, alpha
0x118c14c).

The chain as the GS runs it (traced sprite by sprite): mask in 336 (512 x
448) -> 448 (256 x 256, bilinear) -> 480 (128) -> 488 (64); 510, the last
frame's *blurred* result, drawn over 488 with alpha 0x118c12c (0x76:
0.92); 510 = 488 blurred (a copy, then the 12 shifted lerps); 480 = 488
blurred the same way at 128 x 128; 448 = 480 enlarged; added over 224
(UV 0.5..256.5). Since the history is the blurred result, it is blurred
again every frame: the glow keeps spreading over the sky and swells for a
dozen frames when the sky comes into view (y = 0.92 B(y) + 0.08 m). The
sequential lerps weigh the later taps more (+1 after -1), so the glow also
drifts down and right; the lerps truncate. Natively (Metal, desktop GL)
the same buffers and steps (`gsBloom`); SOTC_BLOOM_OLD=1 the earlier
quarter-resolution approximation. At SOTC_BLOOM_SCALE=1 the added light
measures within ~15% of the GS's (the PS2's bloom isolated with
PS2X_GS_SKIP_TBP=14336).

Model instance opacity (modelDisp, to the microcode at packet + 0xdc):
1 - instance[0x91] (+0x244), times 1 - instance[0x92] (+0x248) when the
header's +0xc is set; 0 hides the instance (e.g. the dark wisps near Wander).

## Census (SOTC_CENSUS=1, shrine, Wander standing; per frame)

| function | calls | |
|----------|-------|---|
| modelDisp | ~50 | 35 distinct models, ~2800 draw packets in their headers |
| modelClipSolid / modelClipSkin | 124 / 20 | from `ClipLayoutObjModel`: scenery, objects and characters are all layout-object models, clipped part by part |
| meshDisp | 7-15 | |
| ptclmanEnd (particles) | ~50 | |
| spriteTexFast | 192 | screen passes: 0x109E2A0 (32 call sites, the bloom chain), 0x10A0100, `depthOfField`, `shadowDraw`, `reflectionBegin`, `waterRefractionBegin`, `screenBlur`, `bossDeathEffectEnd` |
| texTransRequest | ~310 | the game re-uploads its textures to the 4 MB GS memory every frame |
| transMicroCode | ~237 | |

So nearly all 3D goes through one model system; screen effects are
sprites over buffers; and the per-frame texture uploads disappear once
textures live in GPU memory.

## Checked against a frame (tools/model_view.py, SOTC_MODEL_DUMP)

- The VU1 matrix of a model's batches is exactly `S x camera x model`:
  camera = camera context (`iosgluGetCameraContext`) + 0x5a0, model = the
  matrix passed to `modelDisp`, S maps clip to GS pixels: x = 2048 + 256 x/w,
  y = 2048 - 224 y/w (the depth row scaled too). 22 of 50 models of the frame
  matched the matrices the VU1 used to 1e-4.
- A draw packet holds several world batches, each ending in MSCNT
  (GIF tag, positions V3-32, ST V2-16, colours V4-8; triangle strips).
- The packets drawn: the instance's LOD (bits 40-42 of the 64-bit word at
  instance + 0x250, clamped to the model's LOD count) and, per part, the
  visibility the clip wrote at LOD table + part x 0x30 + 0x2c (0, 2 drawn).
- Positions are model-local; the sky is model 0 of the frame (radius
  10000-33000); large one-packet models around Wander are transparent
  planes (fog/light). Lit surfaces have no vertex colour: the lit
  microcode computes it from normals. Skinned characters (Wander) are not
  drawn by `modelDisp`.

Still to read for drawing: textures (ST, texture resources, CLUTs), the lit
batch (normals) and its lights (instance + 0x20..0x9f), surface materials
(GS registers at surface + 0x60: alpha, test), skinned characters.

## Next

1. Read one model straight from guest memory (header -> surfaces ->
   packets -> vertex blocks) and draw it with Metal next to the emulated
   picture.
2. Then the Metal renderer at `modelDisp` level, system by system.
