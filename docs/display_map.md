# Display and models: a map for S4

Written in Decomp D11 (2026-11-16) for round S4, the redesign of the draw path for the Vita: the
game's draw data fed to the GPU directly, with no PS2 GS / GIF / VIF packet in between, and the render thread on core 2
freed. It covers what the code is, how a frame's display runs from the game thread to the present, every hand-written
native of the area with its switch and fallback, the host systems built so far, the guest data layouts as the code states
them, what S4 needs (section 5), and the costs earlier rounds measured. Every address is GAMECORE's (0x10xxxxx-0x14xxxxx)
or the boot ELF's (0x1Bxxxx-0x1Exxxx).

Sources: game/overrides/sotc_native.cpp (the native renderer's hooks), decomp_walk.cpp, decomp_layout.cpp,
decomp_layoutsimple.cpp, decomp_light.cpp, decomp_hot43b.cpp, decomp_ambient.cpp, decomp_modelclip.cpp, decomp_sortlist.cpp,
decomp_dl.cpp, decomp_leaves.cpp, decomp_isys48.cpp, decomp_texanim.cpp, decomp_texframe.cpp, decomp_texreset.cpp,
decomp_textrans.cpp, decomp_hot46f.cpp, decomp_hot47g.cpp, decomp_texsearch.cpp, decomp_tex64.cpp, decomp_hot43g.cpp,
decomp_hot44g.cpp, decomp_hot48a.cpp, decomp_hot48b.cpp, decomp_skinpal.cpp, decomp_group.cpp, decomp_prefetch.cpp;
PS2Recomp/ps2xRuntime (include/runtime/native_renderer.h, src/lib/native/native_renderer_gl.cpp, src/lib/ps2_memory.cpp,
src/lib/ps2_gif_events.h, src/lib/ps2_vif1_interpreter.cpp, src/lib/gs/gs_frontend.cpp, src/lib/ps2_vita_runtime.cpp);
docs/native_renderer_map.md, docs/vu1_gpu_path.md, docs/vu1_routines.md, docs/vu1_fast_recompiler.md,
docs/native_port_plan.md, docs/decomp.md (rounds cited by number), docs/characters_s3.md. "(guess)" marks what the sources
do not state. file:line citations are to game/overrides/ unless stated otherwise.

**Area definition.** tools/decomp_status.py:46 puts a function in the area when its name matches
`disp|model|mesh|draw|layout|lod|clip|sort|shadow|texture|tex|sprite|gl|vram|light|fog|blur|bloom|render|screen|color|colour`
and no earlier area matched (Characters and motion is tested first), or when it is a native in
decomp_(layout|light|modelclip|pointsort|prefetch|sortlist|texanim|texreset).cpp or sotc_native.cpp (:60). docs/
decomp_status.json gave 978 functions, 489 native, 60.8% by code size after D10; after D11 (the honest count,
`SOTC_WORKER91=0`) 977 functions, **507 native**, 61.6% by code size (one function moved to characters by its file).

**Caution about the definition.** The regex is a substring test on the lower-cased name, so:
- `tex` matches `context`: iosgluGetCameraContext (0x108F518) and get_cur_camera_context (0x1160F68) count as display
  (they are camera getters; the display does read them hundreds of times a frame).
- `sort` matches execPointListSort (0x142DA48, the AI path's point search), and decomp_pointsort.cpp is in SOURCE_AREAS:
  an AI function filed under display.
- `clip` matches the collision clips (ClipCollisionDB, ClipCollisionFinder*), but Collision comes after Display in AREAS,
  so those collision names land here unless their file is decomp_collision.cpp (its natives are filed by source first).
- `disp` matches `dispatch`; `gl` matches `global` / `glow`.
- In D11 this put ClipCollisionFinderWithKeepSlopeLimit (a collision finder) among the display functions D11 lifted
  and then cut for speed. decomp_status.py's rule is left as it is so the counts stay comparable round to round; the
  table in section 7 lists the area as the tool counts it.
- UpdateLayoutClassGeometry (decomp_layout.cpp) is counted here by its file, but it is the characters' layout pass
  (characters_map.md 2b lists it).
The tables below give the real registration lines.

---

## 1. The parts

| part | what it is | main guest functions (addresses) |
|---|---|---|
| **Display walk** | The gamecore display task's call each frame. Walks the layout display group: per object the class's update (+0xd8) and clip (+0xdc) callbacks, the visible flag, AddSortObj; copies the sort list into a second list; the light pass; then per drawn object iosDlLockObj, lights (+0xd4) and draw (+0xd0) callbacks. | DispDispObjGroupModel 0x10F52C8; AddSortObj 0x110DC20; ClearSortList 0x110DE38; GetFirstSortObj / GetNextSortObj 0x110DD28; GetLayoutClass 0x10F1A20; GetGeoDispClass 0x10ED688; UpdateLayoutClassSimpleGeometry 0x10EED58 (billboards / camera links); InitLayoutObjMatrix 0x10F5820; iosDlLockObj (decomp_isys.cpp:462) |
| **Model clip / LOD** | The common layout class's clip callback: the instance's fade (+0x244), the LOD scale (+0x250), modelClipSolid (far cull, bounding sphere, LOD level with hysteresis, root box VCLIP) or modelClipSkin for skinned models, the LOD level into bits 40-42 of +0x250. | ClipLayoutObjModel 0x10F4890; modelClipSolid 0x1098840; modelClipSkin 0x1098450 (recompiled); modelClipPoint 0x1099C50; clipByMinMaxCube 0x1087300; clipBySphere 0x1087260; 01097F88 (a model picking its own LOD) |
| **Model / mesh draw prep** | The draw callback: solid models' part matrices (camera +0x1A0 x part) and modelDisp per part; skinned characters' palette (dispSkinned) then their model draws and a tail jump to shadowDisp; the heat haze strips; cloth / hair grids (DispCloth inline, then the grid mesh and meshDisp). | DispLayoutObjModel 0x10F5920; solidDisp 0x10F3A08; dispSkinned 0x10F3CF0 (the +0xD0 display callback of skinned objects); modelDisp 0x1097118; shadowDisp 0x109E0E8; hazeStrips 0x10F3EB8 / 0x10F3FB0; grid mesh 0x1137F78 (recompiled; its native is commented out at decomp_hot48b.cpp:1091); meshDisp 0x109AB80; modelInitDispModelClass (from 0x10EF9D0); modelSetEnviroment 0x10998B8 and its camera packets 0x1098E20 |
| **Display list and layers** | The game never draws directly: every system appends prebuilt DMA chains to a display list split in layers (iosDlPushLayer(n), iosDlChainTailCurrent(tag), iosDlPopLayer). transMicroCode(n) appends a VU1 microcode upload when the layer's current one differs (table 0x118CD90). Two buffers, swapped once a frame by iosDlSwap; the list goes to VIF1 once a frame. | iosDlPushLayer 0x1C0210; iosDlPopLayer 0x1BF8C0; iosDlChainTailCurrent 0x1BFE88; iosDlGetLayer 0x1C0198; iosDlGetWorkBufId 0x1C0138; kernel_sub_001BF590 (the buffer reset); transMicroCode 0x1114B50; resetMicroCodeManager; iosDlSwap 0x1BFF50 (recompiled); iosDlTrans 0x1C0058; iosSyncGP 0x1C3340; iosDmaSendSChainPath1; iosAllocPacketPtrPath1 / iosSetPacketPtrPath1 |
| **GS register packets (2D and screen passes)** | A+D (value, register) pairs appended to a packet opened by gsRegBegin and closed by gsRegEnd (state at *0x1E11A4). Used by sprites, windows, HUD gauges, subtitles, fades, cinema bars and every screen effect (bloom, blurs, reflection set-up, fog sprites, boss death, shadow reduce). | gsReg 0x1C5B00; gsRegEnd 0x1C3AD0; spriteNoTexFast 0x1C5110; spriteTexFast 0x1C5548; spriteDrawSprite 0x10A32B8 and the family 0x10A24C0-0x10A5270; spriteDraw* / windowPrintf 0x114FD00-0x1150278; windowSprite* 0x1155B98-0x1155ED8; fadeScreen 0x108A408; drawDemoBar 0x108A588; drawFrontFog (0x108A288 range); subtitles 0x134B490; HUD gauges 0x13F6B20-0x13F8958; title backdrop 0x14033F0; loading lines 0x1C2DD0; SpriteMenuSetSpriteObjectParam 0x110FD90 |
| **Immediate-mode gl\* layer** | A GIF packet built on the CPU at *0x1186398 (DMA tag, GIF A+D tag at +0x10, then pairs to *0x1186394): TEST, PRIM, per vertex RGBAQ, ST or UV, XYZ2 / XYZF2 / XYZ3 in screen space; vertices go through a 256-slot ring in the gl block at 0x1186390; glEnd chains the packet into the layer glBegin pushed. Users: the heat haze strips (~half the kicks), the particle quads 01142350, the boss-death fan, the opening's ribbon. | glBeginWithDlLayerNo 0x108BB38; glBegin; glColor4f / glTexCoord2f / glVertex3fv 0x108EBC0 (its own part 0x108D358); glVertexKick 0x108D730; untransformed vertices 0x108D478; glEnd 0x108D060; glEnd's software clip 0x108CD80; glReset; glZOffset; iosgluGetViewCoordPosition |
| **Textures and VRAM** | Per frame: texReset clears every texture's transfer words and runs texAnim (scroll offsets, animated CLUT); each drawer asks texTransRequest(resource) before drawing (~310 a frame); texTransResolve, from the GS FINISH interrupt (round 60), walks the request lists and, per texture, allocates GS VRAM (vramAlloc) and builds the upload packet (BITBLTBUF / TRXREG / IMAGE per level and CLUT, then TEXFLUSH / TEX0 / TEX1 / MIPTBP / TEST) or reuses the one built before. The VRAM block allocator keeps free / used lists. | texReset 0x10AB2A0; texAnim 0x10A9EA8; texResetTransfers 0x10AB618 (also "vramUnbind" in decomp_hot46f.cpp:71); texTransRequest 0x10AAD48; texTransRequestMulti; texTransResolve 0x10A54F0 (its CLUT records in decomp_hot47h.cpp); texResetVRAMRequest 0x10AAFA8; vramAlloc 0x10ABD38; vramFree 0x10ABEE0; vramReset 0x10ABF20; the block allocator 0x10AB6D0 / 0x10AB848 / 0x10AB9A0; texSearchMopByName 0x10AB3A8; texInitialize / texDelete; texScaleClutByName |
| **Lights** | The light pass copies the light work area into the scratchpad (LoopExecPreLightColMan), then every lit object's +0xE0 callback (CalcLightLayoutObj -> SetGeoDispClassLightColMan) runs the global lights in range, the ambient grid sample, the game's point lights, the blend and the object's normal / colour light matrices; then 010F4A18 per layout object (its lights in camera space). modelInitDispModelClass's setLight puts them into the instance's own VU1 packet. | lightPass 0x10F4FB8; CalcLightLayoutObj 0x10F4D48; ExecGlobalLightMan 0x10F8728; blendLights 0x10F8360; SetWorkFromGlobalLight 0x10F9168; lightMatrices 0x10F5F60; objectLight 0x10F4A18; getAmbientColorC 0x1091E30; GetInverseMatrixFromQuaternionPos 0x1BBE60; GetRateInOutCube 0x1136F30; ExecGameLight (recompiled) |
| **Fog** | Three kinds: per-vertex GS fog on every world batch (FGE; factors getFogFactor0 / 1 0x1089B70 / 0x1089BB0 from 0x12147DC..EC; colour from graphSetFogParam, 0x118B8DC..E4); zFog's depth slices into the frame's alpha at layer 4, then bgDraw blends the sky / background buffer over the scene by it; drawFrontFog's flat colour at layer 0x14. | zFog 0x10AC0A8; bgDraw 0x1085258; drawFrontFog; graphSetFogParam; getFogFactor0 / 1; getFogUnderWater (the word 0x1186328) |
| **Shadows** | shadowDisp chains a stencil-count volume per caster (microcode 8 rigid / 5 + 9 skinned); shadowDraw reduces the count buffer and darkens the picture. The skinned bodies' shadowDisp is reached by a tail jump that the recompiled code makes a direct C++ call. | shadowDisp 0x109E0E8; shadowDraw; setShadowEffect 0x10871C8 (flag 0x1186294) |
| **Screen effects** | Sprites over off-screen buffers: water reflection (layers 12-15) and refraction (layers 9, 19), the bloom chain (layer 0x14), screenBlur / cameraBlur, feedbackBlur (layer 29), depthOfField (layer 20), bossDeathEffect (layer 8 composite), solarFlare. | reflectionBegin 0x109AD58; waterRefractionBegin / End 0x109C108 / 0x109D078; refractionBegin / End (0x109C610 range); bloom pass 0x109E2A0 (from solarFlare 0x10A16C8); screenBlur 0x108A780; feedbackBlur / feedbackBlur2 (0x1089BF8-0x108A0E0); depthOfField 0x10874E8; bossDeathEffectBegin (0x1085640 range) / End 0x1085E40; 0x10F37C8 (screen copy for the haze); DispWithSwordBlur 0x13FDD68 |
| **Frame start / end** | graphFrameInit resets the frame's graphics; graphFrameFinish runs the screen effects. | graphFrameInit 0x108FBE8 (from the frame start task 01346318); graphFsEffectReset 0x108FCC8; graphFrameFinish |
| **Particles (other map)** | ptclman packets (microcode 7) and their hooks; see docs/particles_map.md and particles_s1.md. S1 already writes draw records straight into the renderer's job. | ptclmanBuildMat / Env / Vtx 0x1099E00 / 0x109A280 / 0x1099F38; particleDraw 0x113E940; disp_particle_geodisp 0x113F6A8 |

### The VU1 microcode the packets target

The world, characters, cloth, shadows, water and particles each target one of 14 VU1 microcode images (docs/
vu1_routines.md): world `f40e22b1@05b0` (52% of VU1 work on the route), lit loop `@0710`, second-UV passes `@0628` /
`@0698`, characters `059698f3`, cloth `8178e9d1` / `1fab41fc`, shadows (microcode 8 / 5 + 9), water (`98a396b7`
microcode 1, `87bc2f1b` microcode 2), particles (microcode 7). With the native renderer drawing, **no VU1 program runs**:
src/lib/ps2_vif1_interpreter.cpp:841-843 skips VU1 when GS::skipRaster(), and ps2_memory.cpp:1901-1904 gathers the VIF1
chain without UNPACK / MPG data. The host reimplements each microcode's maths in its shaders (lit loop, skinning, water
Fresnel, shadow extrusion, particle expansion: native_renderer_map.md).

### The native renderer's hooks (sotc_native.cpp installSotcNative :12244-12370)

Installed only with SOTC_NATIVE set (the Vita sets SOTC_NATIVE=1 unless env.txt says otherwise:
PS2Recomp/ps2xRuntime/src/lib/ps2_vita_runtime.cpp:662). Each hook is a function-table entry; where a native exists for
the same address, `sotc_decomp::behindHook` gives the hook the native to call (rounds 42-48).

| guest function | addr | hook (sotc_native.cpp) | what the host does |
|---|---|---|---|
| modelDisp | 0x1097118 | modelDispHook :10597 | In native mode (no PS2 lists: `!tuning().ps2Lists && GS::skipRaster()`) the guest modelDisp does **not** run (`ctx->pc = ra`, :10639-10645). The hook computes camera (+0x5A0) x model, takes the instance's LOD, fade, palette, lights, scroll rows and projection into a job; modelDispState (the part tree's camera tests written to +0x2C, the layer cache *0x118BFC8, setShadowEffect's flag 0x1186294, the instance packet's opacity +0xDC, the VU0 CLIP register) runs on the mds lane; the draw (plan, packets, meshes, skinning, DrawState) on the hook worker. |
| iosgluGetCameraContext | 0x108F518 | getCameraContextHook :3964 | Notes the camera context (`g_cameraContext`), then its native. |
| gsReg | 0x1C5B00 | gsRegHook :6655 | Classifies the write by its return address (`k2D` ranges :6008-6022, feedbackBlur 0x1089BF8-0x108A0E0, refractionEnd 0x109C610-0x109CD28, the bloom 0x109E2A0-0x109FB08) and feeds 2D pairs to the 2D assembler (`feed2D`); then the guest gsReg still writes the pair into the packet. |
| texTransRequest | 0x10AAD48 | texTransRequestHook :6735 | Notes the requested texture's NTO2 header (`g_requested`, the resource's +0x20) for the next 2D / mesh draw, then the native. |
| iosDlPushLayer / PopLayer | 0x1C0210 / 0x1BF8C0 | pushLayerHook :6744 / popLayerHook :6803 | A host layer stack (`g_layers`) mirrors the guest's; at shadowDisp's two pops (ra 0x109E1C4 skinned, 0x109E274 rigid; model in s1, chain in s2) the shadow volume is captured (round 37). |
| glEnd | 0x108D060 | glEndHook :6556 | Parses the packet's pairs from *0x1186398 + 0x20 to *0x1186394 into the 2D assembler (the haze's screen copy by ra 0x10F3FB0-0x10F4890; boss fan by ra 0x1085640-0x1085E40 goes off-screen), then the guest glEnd closes and chains the packet. imm128 bypasses glBegin / glEnd for the two quad sites (below). |
| spriteTexFast | 0x1C5548 | spriteTexFastHook :9556 | The screen passes' copies (bloom chain, depthOfField, shadowDraw, reflection, waterRefraction, screenBlur, bossDeath): the host's own effect passes replace them. |
| shadowDisp | 0x109E0E8 | shadowDispHook :7083 | Rigid shadows (Wander's sword); the skinned bodies are taken at the pop (above). |
| ptclmanBuildEnv / Vtx / Mat | 0x109A280 / 0x1099F38 / 0x1099E00 | ptclEnvHook :7930, ptclVtxHook :7950, ptclMatHook :7894 | Copy the particle inputs into a job (ptcl103 / pkt117 / S1 bypass the packets). |
| screenBlur | 0x108A780 | screenBlurHook :8518 | `native_renderer::addScreenBlur`. |
| meshDisp | 0x109AB80 | meshDispHook :6836 | Copies the cloth grid's current buffer (view matrix + rows x columns x 4 qwords) into a job at the hook (round 23's desync fix); the guest meshDisp still chains it. |
| zFog | 0x10AC0A8 | zFogHook :2971 | Reads N / width / margin / far (0x118C46C / 70 / 74 / 80) into `native_renderer::Fog`. |
| (the kick) | host DMA kick | `ps2_stats::kickHook` :12320 | At each display list kick: the frame's palettes copied and the skinned pass queued as a job (SOTC_KICK_DEFER, round 34), or a join then skinAtKick. |
| (round 61 joins) | 17 addresses | mdsjoin::wrap :12190 | Before every reader of what modelDispState writes (clipBySphere, clipByMinMaxCube, ClipPoint, 0108C9C0, 0108C188, modelDisp's walk 01099A08 / 01099BA8 / 01099BC0, modelResetSurface 0x10993C8, modelSetSurface 0x1099768, 01097B70, 01097D88, 01099828, 01097928, 01097A68, setShadowEffect, bossDeathEffectEnd) the lane is joined. |
| (round 64 joins) | kSites (decomp_tex64.cpp:521) | decomp_tex64::wrap | Before every reader of texReset's loop output (modelDisp, meshDisp, iosDlTrans, spriteTexFast, the walk, texTransRequest / Resolve ...) the offload thread is joined. |

**The return-address contract.** The recompiled spriteDrawSprite runs in pieces the dispatcher continues after a call
returns, so a hook that wraps a 2D function sees none of its gsReg writes (native_renderer_map.md "2D"). The hooks are
therefore on gsReg / glEnd / the layer stack, and tell callers apart by `ra`. Every native that calls these functions
(reflectionBegin decomp_hot43g.cpp:29-40, the bloom pass decomp_hot48a.cpp:33-40, graphFrameInit, the walk) calls them
through the function table with `ra` = its jal site + 8 and the original's frame `sp`. The same applies to shadowDisp: the
skinned call is a tail jump, so the shadow is taken at iosDlPopLayer's return address.

### Frame flow (who calls what, in what order)

The frame is counted by display-list kicks (`ps2_stats::vif1Chains`); the host notices a new frame at the first hook after
a kick (syncFrame, sotc_native.cpp:5772).

1. **Frame start** (task 01346318; docs:14814): **graphFrameInit 0x108FBE8** (decomp_hot48b.cpp:5-12): effectReset, the
   frame time into 0x118B8C0 / clock 0x118B8F4, **texReset 0x10AB2A0** (LOD scale *0x118C180 = camera +0xC / 500, the
   request slot arrays cleared, vramReset, then every texture node: transfer words cleared and texAnim; with SOTC_TEX64 the
   loop runs on the offload thread), glReset, windowSetMouseStatus, resetMicroCodeManager, then for each display-list layer
   iosDlPushLayer / iosDlChainTailCurrent(0x118B760 + 0xB0 x work buffer id) / iosDlPopLayer. InitLayoutObjMatrix
   0x10F5820.
2. **The script pass, characters, camera, particles** (characters_map.md 1): ExecCharaGeometry writes each character's
   skinning matrices (class +0x160); the camera manager writes the camera context; the particle manager steps.
3. **The display walk** (DispDispObjGroupModel 0x10F52C8, a0 the camera object; decomp_walk.cpp:4155):
   1. The group pass: at its start clip140 lists the plain layout objects for the offload helper. Per object: billboard /
      camera-link turn and UpdateLayoutClassSimpleGeometry; +0xD8 update; +0xDC clip (**ClipLayoutObjModel** -> clip83
      or modelClipSolid / modelClipSkin; the LOD into +0x250 bits 40-42); the visible flag (+0x260 bit 8; a character's
      +0xCC0 told); AddSortObj by the key at +4. Particle geodisps' clip (ptcl62 / ptcl79).
   2. The sort list copied into the second list without the objects with +0x260 bit 4.
   3. **The light pass** 0x10F4FB8 (walk83's colour cache).
   4. The draw loop, per object in sort order: iosDlLockObj; +0xD4 lights; +0xD0 draw:
      - solid: **DispLayoutObjModel 0x10F5920 -> solidDisp 0x10F3A08**: part matrices (solid103 skips the VU1 blocks),
        then per part **modelDisp** (hooked; host job) and **shadowDisp** (hooked);
      - skinned: **dispSkinned 0x10F3CF0** writes the palette (class +0x160 x camera +0x1A0) into model +0x104, the parts'
        modelDisp calls, the tail jump to shadowDisp (captured at iosDlPopLayer's return 0x109E1C4); the heat haze strips
        0x10F3EB8 (quad117 / imm128 -> immFeed128 -> 2D assembler);
      - cloth: DispCloth (inline) -> grid mesh 0x1137F78 (normals, buffer, layer, microcode, camera matrix) -> **meshDisp**
        (hooked: grid copied into a job) -> iosDlPopLayer;
      - particle geodisps: draw117's chain to the drawers (ptcl103 / pkt117 / S1 jobs).
4. **graphFrameFinish** (guest): the screen effects in their layers: zFog (hooked, layer 4), bgDraw, reflection /
   refraction, the bloom pass 0x109E2A0 (layer 0x14, from solarFlare), cameraBlur -> screenBlur (hooked), feedbackBlur
   (layer 29), depthOfField, bossDeathEffectEnd, waterRefractionEnd. The 2D: menus, windows, HUD, subtitles, fades and
   bars through gsReg (hooked by ra). (The exact interleaving of the 2D and graphFrameFinish: guess.)
5. **The send**: iosSyncGP 0x1C3340 -> iosDlSwap 0x1BFF50 (recompiled: WaitSema on the list's semaphore *0x2132D0, the
   buffer index flipped, the isys lock) -> iosDlTrans 0x1C0058 -> iosDmaSendSChainPath1, kicked by the host
   (sotc_overrides.cpp:869-871, `sysnative`, once the paths are idle). At the kick: `kickHook` (palettes copied, the
   skinned pass queued as a job); with SOTC_DMA_ASYNC the chain state and VIF1 registers go to the DMA thread, which walks
   the chain for SIGNAL / FINISH / LABEL and MSKPATH3 only (one FINISH a frame); the result is applied at the GP
   thread's next poll (1 721 of 1 800 chains, round 60), then the FINISH interrupt runs SotC's handler 0x1C37F0 -> iFinish
   -> **texTransResolve** (round 60).
6. **The next frame's first hook**: syncFrame -> hookJoin(k_frame) (the frame's model jobs done) -> sotc_native endFrame
   (:3880-3961: glow composite, depth of field, sky near plane, fog, bloom, `native_renderer::endFrame(512 x scale, 448 x
   scale)`) -> the GL backend's endFrame (native_renderer_gl.cpp:9180): **waitData** (the render thread took the frame
   handed over before), the recorded lists swapped (draws, extra meshes, arenas, palettes, blurs, uploads, fog, bloom,
   glow, dof).
7. **The render thread** (core 2): applies uploads, plans the frame (planFrame :5068: layer order, merges of neighbouring
   draws by texture / state / matrix), runs the passes (background, mirror, the picture's segments between effects) and
   the GL calls (vitaGL), gives the frame's data back, then presents (glPresent86: `vglSwapBuffers`, which blocks while
   vitaGL's 3-buffer / 2-pending display queue is full). The display callback (pace110 / pace132 / pace143,
   raylib_vita.cpp) sets each picture on its refresh slot. In flight: the game thread on N+1, the render thread on N,
   the GPU on N-1 / N-2 (round 86).

---

## 2. The hand-written natives

Registration lines are the SOTC_DECOMP_FUNCTION / SOTC_DECOMP_AB_FUNCTION lines (AB: in SOTC_AB's "new" set). Switches
are on unless `SOTC_X=0` except where marked **off** (sotc_decomp.h:723-726: texr128 among them). Many natives here fall
back to the recompiled code when a pointer is outside main RAM; with SOTC_DECOMP_LIFT_FALLBACK (D5) that fallback may be
served by a lifted copy.

### 2a. The walk, clip, sort

| addr | guest name | host fn (file:line, reg) | switch | exact / port | fallback to recompiled(addr) |
|---|---|---|---|---|---|
| 0x10F52C8 | DispDispObjGroupModel | decomp_walk::dispDispObjGroupModel (:4155, AB reg :4856) | round 7; sort140, clip140, walk67, walk83, draw95 inside | exact (guest-visible writes, v0 / f0, callee-saved regs at each guest call) | sp frame outside RAM (:4162-4163); an object or pointer outside RAM resumes the recompiled code at a loop head 0x10F5348 / 0x10F5710 (:4188-4196) |
| 0x10F4890 | ClipLayoutObjModel | decomp_walk::clipLayoutObjModel (:1920, AB :4857); clip83 inside (SOTC_WALK83); clip140 results (SOTC_CLIP140) | SOTC_WALK83, SOTC_CLIP140 | exact (round 83: level, LOD bits, depth keys identical; clip140 0 differ in ~5 M clips) | frame / object outside RAM (:1925-1926); skinned, self-LOD, unknown LOD modes go to the previous native path (~15% of calls, round 83); runToReturn into recompiled at :1952 / :1967 |
| 0x1098840 | modelClipSolid | decomp_walk::modelClipSolid (FLOAT reg :4860) | — | exact | frame / instance / matrix outside RAM (:860-861); runToReturn at :903 / :986 |
| 0x1099C50 | modelClipPoint | decomp_modelclip::modelClip (reg :297) | SOTC_FASTNATIVE picks the fast path (:195) | exact (VU0 regs, ACC, Q, CLIP history, status left as the PS2) | not fast and not plain RAM (:200) |
| 0x10F5920 | DispLayoutObjModel | decomp_walk::dispLayoutObjModel (:3935, AB :4858) | — | exact; DispCloth inline | frame / object outside RAM (:3940-3941) |
| 0x10F3A08 | solidDisp | decomp_walk::solidDisp (:2040, AB :4859) | SOTC_SOLID103 (skips the VU1 matrix blocks in native mode for models without shadow / lens scale) | exact except the skipped blocks (no reader: poison runs, round 103) | frame / s1 / s1+0x144 outside RAM (:2045-2046) |
| 0x110DC20 | AddSortObj | decomp_dl::addSortObj (AB :285) | (switch not read; round 6) | exact | several range checks (:72-181), nothing written before |
| 0x110DD28 / 0x110DE38 | GetNextSortObj / ClearSortList | decomp_sortlist.cpp (:90-91); inline in the walk; sort140's host bucket words | SOTC_SORT140 | exact (sort140 check: 0 differ in ~9 M scans) | — |
| 0x10F1A20 | GetLayoutClass | decomp_layoutclass::getLayoutClass (decomp_layout.cpp:650) | SOTC_LEAVES | exact | leaves off or unusual cases (:559-584) |
| 0x10ED688 | GetGeoDispClass | decomp_ptcl62::geoDispClass (decomp_ptcl62.cpp:263) | SOTC_PTCL62 | exact | not read |
| 0x10EED58 | UpdateLayoutClassSimpleGeometry | decomp_layoutsimple::updateSimple (:292) | SOTC_MOTION | exact | motion off, negative count, helper jal sites changed (:114-115) |
| 0x10F24D8 | UpdateLayoutClassPastSimpleGeometry | decomp_copymatrix::pastSimple (decomp_math.cpp:412) | — | not read | — |
| 0x10F26D0 | UpdateLayoutClassGeometry | decomp_layout::updateLayoutClassGeometry (:505) -> pose82::layout | SOTC_POSE82, SOTC_FASTNATIVE | pose82: last bits | negative bone count (:282) |
| 0x10F5820 | InitLayoutObjMatrix | decomp_group::initLayoutObjMatrix (decomp_group.cpp:275, AB) | — | exact | range checks (:164-224) |
| (kLockObj, address not read) | iosDlLockObj | decomp_isys.cpp:462 | SOTC_ISYS | exact | not read |
| 0x142DA48 | execPointListSort | decomp_pointsort::sort (:405) | SOTC_PATH97 | exact | **not a display function** (AI path points; filed here by SOURCE_AREAS) |

### 2b. Models, skin palettes, haze, gl*

| addr | guest name | host fn | switch | exact / port | fallback |
|---|---|---|---|---|---|
| 0x10F3CF0 | dispSkinned | decomp_skinpal::skinned (:58, AB :139) | none | palette exact (camera rows clamped once) | obj not in RAM, obj+0x194 != 0, bones < 0 or > 1024, a pointer outside RAM (:62-77); always resumes the recompiled code at 0x10F3DF0 (:135) |
| 0x10F3EB8 | hazeStrips | decomp_walk::strips (:3585, AB :4861); quad117 / quad140 / imm128 inside | SOTC_GLV117, SOTC_QUAD140, SOTC_IMM128, SOTC_CLIP134 | exact (gl block 0x5300 bytes, VU0 / ACC / Q / CLIP, f0-f2 compared; 0 differ) | frame / chara / layout class outside RAM (:3588-3589, :3617); runToReturn at :3784 |
| 0x1142350 | particleQuad | decomp_walk::quad1142350 (:3405, reg :4862) | SOTC_GLV117 | exact | glv117 off, ranges, jal at 0x114240C not 0x1283D0 (:3410-3412) |
| 0x108D730 | glVertexKick | decomp_walk::glVertexKickA9<> (:2430, AB :4863) | SOTC_A9CMP picks the integer-compare copy | exact (joins the lane: clipNeeded, :2435) | frame outside RAM, gl camera not readable, index >= 256 (:2441-2442); runToReturn at :2639 |
| 0x108EBC0 | glVertex3fv | decomp_walk::glVertex3fvEntry (AB :4864) | — | exact | :2714 |
| 0x1097118 | modelDisp | (hooked, not a native: section 1) | SOTC_NATIVE | — | with PS2 lists (`tuning().ps2Lists` or the GS rasterising) the guest modelDisp runs (:10639) |
| 0x1137F78 | grid mesh | decomp_hot48b::gridMesh, **commented out** (decomp_hot48b.cpp:1091) | — | — | recompiled |

### 2c. Display list, GS register packets, screen passes

| addr | guest name | host fn | switch | exact / port | fallback |
|---|---|---|---|---|---|
| 0x1C0210 / 0x1BF8C0 | iosDlPushLayer / PopLayer | decomp_leaves::pushLayer / popLayer (:99 / :120, reg :377-378), behind sotc_native's hooks | SOTC_LEAVES | exact | depth >= 255, negative layer, empty stack (the LoaderSysPrintf error paths) (:103-104, :123-124) |
| 0x1BFE88 | iosDlChainTailCurrent | decomp_dl::chainTail (AB :286) | (switch not read) | exact | range checks (:200-203) |
| 0x1C0198 | iosDlGetLayer | decomp_dl::getLayer (:282) | SOTC_LEAVES | exact | leaves off (:245-246) |
| 0x1114B50 | transMicroCode | decomp_dl::transMicroCode (:283) | SOTC_LEAVES | exact | leaves off or jal sites changed (:257-258) |
| 0x1BF590 | kernel_sub_001BF590 (the display list buffer reset) | decomp_isys48::dlReset (:819) | SOTC_ISYS | exact | isys off |
| 0x1C5B00 | gsReg | decomp_hot44g::gsReg (:60), behind gsRegHook | SOTC_HOTLIST44 | exact (a3 / v1 / a2 / v0 as the code leaves them) | hotlist44 off (:34-35) |
| 0x1C3AD0 | gsRegEnd | decomp_hot48b::gsRegEnd (:1086) | SOTC_HOTLIST48 | exact | hotlist48 off |
| 0x1C5110 | spriteNoTexFast | decomp_leaves::sprite (:379) | SOTC_LEAVES | exact | leaves off (:173-174) |
| 0x110FD90 | SpriteMenuSetSpriteObjectParam | decomp_hot48b::spriteMenuParam (:1094) | SOTC_HOTLIST48 | exact | hotlist48 off |
| 0x108FBE8 | graphFrameInit | decomp_hot48b::graphFrameInit (:121, reg :1085) | SOTC_HOTLIST48 | exact (guest calls by table, original ra) | *0x118B74C == 0 does nothing (as the original); hotlist48 off |
| 0x109AD58 | reflectionBegin | decomp_hot43g::reflectionBegin (:525) | SOTC_HOTLIST | exact (141 guest calls with original ra / sp) | hotlist off (:247-248) |
| 0x109E2A0 | bloom display pass | decomp_hot48a::bloomDisplay (:588) | SOTC_HOTLIST48 | exact (107 guest calls) | hotlist48 off (:289-290) |
| 0x1085E40 | bossDeathEffectEnd | decomp_hot48b::bossDeathEffectEnd (:1095) | SOTC_HOTLIST48 | exact on its every-frame path | flags 0x1186290 != 0 (the boss's death: 4 KB of gsReg drawing): the whole function recompiled, decided at entry |
| 0x13FDD68 | DispWithSwordBlur | decomp_hot46e::swordBlur (:1212) | SOTC_HOTLIST46 | not read | not read |

### 2d. Textures and VRAM

| addr | guest name | host fn | switch | exact / port | fallback |
|---|---|---|---|---|---|
| 0x10AB2A0 | texReset | decomp_texframe::texReset (AB :285) | SOTC_FASTNATIVE (round 49 path), SOTC_TEX64 (loop on the offload thread) | exact; tex64 leaves dead scratch registers as the entry context's (decomp_tex64.cpp:41-48) | any node / pointer / slot outside RAM, checked up front (:83, :91, :216, :224) |
| 0x10A9EA8 | texAnim | decomp_texanim::texAnim (:707) | SOTC_FASTNATIVE | exact (FPU_* rounding; iosGetCosf / Sinf guest calls) | pointers outside plain RAM (:138) |
| 0x10AB618 | texResetTransfers (manager_sub_010AB618) | decomp_texreset::reset (AB :313); texr128's indexed path | SOTC_TEXR128 **off** (on in the console env) | exact (texr128 check 0 differ) | node / record outside RAM or list > 1 << 20: hand-off at loop head 0x10AB630 (:30-39) |
| 0x10AAD48 | texTransRequest | decomp_isys48::texTransRequest (:827), behind texTransRequestHook | SOTC_ISYS | exact | isys off |
| 0x10A54F0 | texTransResolve | decomp_textrans::resolve (:310); records by decomp_hot46f::record (type 1 VRAM reset, texture-only placement) and decomp_hot47h::clutRecord (CLUT requests) | SOTC_MOTION, SOTC_HOTLIST44 / 46 / 47 | exact (the work paths are the generated code's statements, or transcriptions) | motion off (:253-254); jump tables at 0x1192850 / 0x1192900 / 0x1192970 changed (record() returns false); other records run in a compiled copy of the generated code |
| 0x10AB6D0 / 0x10AB848 / 0x10AB9A0 | VRAM block place / alloc / free | decomp_hot47g::place / alloc / freeBlock (:290-292) | SOTC_HOTLIST47 | exact except scratch GPRs and the loop checkpoint | hotlist47 off (:110, :172, :244) |
| 0x10AB3A8 | texSearchMopByName | decomp_texsearch::texSearchMopByName (:83) | — | not read | :50, :69 |

### 2e. Lights

| addr | guest name | host fn | switch | exact / port | fallback |
|---|---|---|---|---|---|
| 0x10F4FB8 | lightPass | decomp_light::lightPass (:1653) | SOTC_FASTNATIVE; join95 inside | exact | :1281 |
| 0x10F4D48 | CalcLightLayoutObj | decomp_hot43b::calcLightLayoutObj (:868); walk83's colour cache | SOTC_HOTLIST, SOTC_WALK83 | cache: **port** (tolerance 0.05 units, refresh 16 passes; colours within ~2/255, normals identical, round 83) | hotlist off (:361-362); cache only for 16-aligned blocks in RAM (:256) |
| 0x10F4A18 | objectLight | decomp_light::objectLight (:1652) | — | exact | :1486 |
| 0x10F5F60 | lightMatrices | decomp_light::lightMatrices (:1654) | — | exact | :1025 |
| 0x10F8728 | ExecGlobalLightMan | decomp_light::execGlobalLightMan (:1655) | — | exact | :736 |
| 0x10F8360 | blendLights | decomp_light::blendLights (:1656) | — | exact | :509-512 |
| 0x10F9168 | SetWorkFromGlobalLight | decomp_light::setWorkFromGlobalLight (:1657) | — | exact | :242 |
| 0x1091E30 | getAmbientColorC | decomp_ambient::getAmbientColorC (:339) | SOTC_MOTION | exact | motion off, jal sites changed (:157-158) |
| 0x1BBE60 / 0x1136F30 | GetInverseMatrixFromQuaternionPos / GetRateInOutCube | decomp_light.cpp:1658-1659 | — | exact | :342 / :489 |

### 2f. Camera getters (filed here by the regex)

| addr | guest name | host fn | switch | fallback |
|---|---|---|---|---|
| 0x108F518 | iosgluGetCameraContext | decomp_leaves::gluCameraContext (:376), behind the hook | SOTC_LEAVES | leaves off |
| 0x1160F68 | get_cur_camera_context | decomp_leaves::curCameraContext (:375) | SOTC_LEAVES | leaves off |

Lifted (D1-D10) functions of the area are not listed: they are exact lifts of the recompiled code in decomp_liftd*_*.cpp
and change no interface. D11's lifts (SOTC_LIFTD11) are this round's.

---

## 3. Host systems built so far

| system | switch, default | thread / core | what it owns | exact? | measured (round) |
|---|---|---|---|---|---|
| **Native renderer** (runtime/native_renderer.h; sotc_native.cpp) | SOTC_NATIVE (1 on the Vita) | hooks on the game thread; jobs on the hook worker; drawing on the render thread | Meshes (VIF world batches decoded once per packet id: V3-32 positions, V2-16 ST, V4-8 colours, normals, skin indices), textures (NTO2 pixels decoded, unswizzled, RGBA8 + mips), draw records (`DrawState`: texture, clamp, blend, order = layer, opacity, UV offset, LOD K / scale / max, fog flags, alpha test, overlay, screen, shadow volume, reflection / refraction / Fresnel, skin palette, 32 light floats) | picture rule (frames compared against the GS path), draw lists checked by draw taps | the whole port's base (rounds 1-37) |
| **Draw plans** (round 8) | SOTC_PLAN, SOTC_PLAN_RUNS | game thread / worker | per (header, LOD): the LOD's packets with id, skinned, mesh presence, lit; invalidated by header / list hashes; unused 600 frames go; budget 3 000 new packets a frame | exact (plan-verify: 5.4 M draws, 0 differ) | modelDisp 3.41 -> 2.51 ms attract, route hook.packets 4.02 -> 1.92 (Vita3K 240); drawMulti: ~70% fewer draw records |
| **Hook worker** (`hookjobs`, round 23) | SOTC_HOOK_ASYNC (on on the Vita) | `HookWorker`, cores 1-2, prio 150 (above render / builds at 160) | ring of 512 `HookJob`s: model, mesh, shadow, particle, kick and 2D batch jobs, run in order; everything the game may change after the hook is copied in (opacity, LOD, part results, matrices, palette, lights, cloth grid, scroll rows, K scale) | exact by the in-order rule; checks SOTC_HOOK_ASYNC_VERIFY, SOTC_KICK_DEFER_CHECK (30 M words read, 0 changed) | Vita3K A/B -4.2% / -8.9% frame (round 23); worker105 moved shadow volumes and the kick pass onto it (round 105). Console 9.5 ms mean / 16.5 p90 a frame (round 105) |
| **Kick defer** (round 34) | SOTC_KICK_DEFER (1 with the worker) | game thread copies, worker draws | 4 kick buffers of palettes (`g_kickJobs`) | exact (checked) | console waited 3-4 ms a frame at the kick before (round 32 log) |
| **mds lane** (round 61) | SOTC_MDS_LANE (on) | `threads::kLane`, cores 1-2 | modelDispState's work (part tree +0x2C results, layer cache, shadow flag, CLIP register pending via clipShift / clipNeeded) | exact (SOTC_MDS_LANE_VERIFY: 75 257 jobs, 0 differ, Vita3K 444) | game -0.6..-1.6 ms (Vita3K 222/300); console estimate 1.5-2.5 ms. Console lane 2.7 ms mean (round 105). Joins that help: 0.03-0.2 ms a frame (round 128) |
| **clip134** | SOTC_CLIP134 (on) | game thread | quad117 records CLIP judgements instead of joining the lane | exact | removes the console's ~1 lane join a frame at the first immediate quad (round 134) |
| **DMA walk on a worker** (round 60) | SOTC_DMA_ASYNC (on) | `threads::kDma`, cores 1-2 | the display list's VIF1 / GIF event walk (Vif1Walker / GifEventScanner) on a snapshot; events applied at the GP poll | exact (SOTC_DMA_ASYNC_CHECK: 10 200 chains, 0 differ, 625 MB of reads unchanged, Vita3K 444) | game -1.6..-1.9 ms, frame -0.6..-1.4 (Vita3K); walk 0.56-0.61 ms on the worker at 222; console dma 2.4 ms (round 105) |
| **Chain walk fast paths** (rounds 6, 8, 31) | PS2X_DMA_PREFETCH, PS2X_DMA_WALK_FAST, PS2X_DMA_WALK2; packet cache PS2X_DMA_CACHE (off: slower) | wherever the walk runs | NEXT/CALL-with-two-NOPs fast path, in-place reads, walker state in locals | exact | Vif1Walker::feed 3.7 -> 2.4% (round 8) |
| **Vif1Compactor / GifEventScanner** (native port phase 4, 2026-10-16) | PS2X_GS_NODRAW (1 with SOTC_NATIVE=1), PS2X_VIF1_FULL=0 | runtime | VIF1 codes without their data; GIF data read in place for SIGNAL / FINISH / LABEL | exact for the events | walk + VIFcode pass ~2% of game-thread samples, from ~13% (native_port_plan.md) |
| **Texture animation offload** (tex64, round 64) | SOTC_TEX64 (on) | `threads::kOffload`, cores 1-2 | texReset's node loop (clears + texAnim) on a copy of the context; joins before every reader | exact (SOTC_TEX64_VERIFY) | ~1.5% of the game thread moved (Vita3K title world); S3's table: 1.8% of a core |
| **clip140 helper** | SOTC_CLIP140 (on) | `threads::kOffload`, cores 1-2 | the plain layout objects' clip results computed from the list's end; the walk takes finished ones, never waits | exact (0 differ in ~5 M clips) | ClipLayoutObjModel 1.34 -> 0.41% of the game thread + 0.26 walk side; touched functions 5.04 -> 3.35% (Vita3K, round 140); console estimate 0.5-0.7 ms riding; S3: 0.9% of a core |
| **walk83** | SOTC_WALK83 (on) | game thread | light colour cache (signature of the frame's lights, per-object inputs, tolerance) and clip83 (plain NEON clip) | clip exact; lights port | altar +2.76 ms, route 6 +1.45 (Vita3K 222 A/B) |
| **walk67 / pref69 prefetch records** | SOTC_WALK67, SOTC_PREF69 (on) | game thread | last frame's lines per object, asked for ahead | prefetch only | Vita3K slower (walk67 off -0.61 ms game, round 127); console never measured |
| **scroll127** | SOTC_SCROLL127 (on) | game thread | per (plan, header) the texture scroll rows' chains | exact (890 000 hooks, 0 differ) | 1.10 -> 0.17% of the game thread |
| **sort140 / quad140** | on | game thread | sort buckets as host words; quad117 flattened | exact | sort lists 1.37 -> 0.55%; quad117 2.23 -> 2.10% |
| **imm128** | SOTC_IMM128 (on) | game thread | glBegin's writes and pairs made natively, fed to the 2D assembler; no guest GIF packet at the two quad sites | exact (same assembler input: 235 500 quads, 0 differ) | immediate-quad functions 4.45 -> 3.34% of the game thread |
| **2D assembler** | — (glEnd / gsReg async: SOTC_GLEND_ASYNC, SOTC_GSREG_ASYNC, SOTC_2D_TEX_ASYNC) | game thread feeds; untextured batches as worker jobs | `g_2d` (prim, RGBAQ, alpha, ST / UV, header, layer, strip, batch) | same draws | gsReg's join was 7.3% of the console's game thread before round 31 |
| **ptcl103 / pkt117 / S1** | on | game thread | particle jobs from inputs, no ptclman packets; S1 writes records straight into the job | ptcl103 / pkt117 exact; S1 port | see particles_map.md |
| **GL backend and render thread** (native_renderer_gl.cpp) | SOTC_NATIVE; renderThread tuning | render thread core 2 (prio 160) | the recorded frame (draws, extra meshes, arenas, palettes, uploads), mesh pools (65 535 vertices, 2.25 MB each) with RAM twins, merged-run copies (budget ~6.8 MB), the texture cache, render targets | same GL calls by construction across the rounds' changes | see section 6 |
| fat116 | SOTC_GL_FAT116 (on) | render thread | records kept whole (member = record, mesh) | exact (1.8 M draws, 0 plan entries differ) | render thread -1.6..-2.0 ms (~11-13%, Vita3K) |
| lean pass / keep layout (round 74) | SOTC_GL_LEAN_PASS, SOTC_GL_KEEP_LAYOUT (on) | render thread | passes over drawn draws only, binary search per layer; vitaGL's layout cache kept across buffer switches | exact | sceClibMemcpy was 9% of the render thread (layout copies) |
| few scenes (round 73), regions (94) | SOTC_GL_FEW_SCENES (on) | render thread | two pictures swapped per frame, glow at the shadows' break, depth / stencil not stored after the last scene | same pictures | 37 scenes at the altar before |
| runsrc118 | SOTC_RUNSRC118 (on) | render thread | RAM twins of mesh pools; merged runs copied from RAM | exact | console: run copies read GPU memory at ~35 ms / MB before |
| present86 | SOTC_GL_PRESENT86 (1) | render thread | data handed back before the present | same GL stream | +1-1.8 ms where the render side is the longer (simulated) |
| pace110 / pace132 / pace143 | SOTC_PACE110 / 132 / 143 (on) | the display callback (raylib_vita.cpp) | each picture's refresh slot; pulls when the game waits (132) or the swap waits behind a held picture (143) | display timing only | pace143: wait +1.16 +- 0.72 ms off minus on (Vita3K); console expectation ~0.4 ms a frame over play |
| VU1 GPU path / native VU1 loops / VU1 fast recompiler | PS2X_VU1_DIRECT, PS2X_VU1_NATIVE (sotc_vu1_world.cpp), plan only for the recompiler | VU1 worker (emulated path only) | VU1 batches to GS::submitVuBatch; hand-written world / lit / UV loops | verify: 5.3 M runs, 0 mismatches | the emulated path only: with SOTC_NATIVE=1 no VU1 program runs (section 1) |

---

## 4. Guest data layouts

**Model header** (native_renderer_map.md; sotc_native.cpp:1-23, 9108-9121):

| off | field |
|---|---|
| +0x08 | owner set at load |
| +0x0C | non-zero: second fade factor (+0x248) applies |
| +0x14 | packets built at load: [0] one per draw packet (0x30 bytes), [1] one per surface (0x40: second-pass / reflection packets, +0x20 a CALL to constants) |
| +0x18 | flags (bit 0: LOD model); +0x1C LOD count |
| +0x20..+0x2C | bounds |
| +0x30 + 4 x lod | per LOD: the part table (0x30-byte nodes; +0x2C the clip's visibility result, 0 / 2 drawn) |
| +0x40 / +0x44 | textures (0x20 each; +0x10 the texture object, whose +0x20 is the NTO2 header) / count |
| +0x50 / +0x54 | surfaces (0x120 each) / count (+0x54 == 1 changes the part cull: sotc_native.cpp:9117) |
| +0x60 / +0x64 | draw packets (8 words) / count |

**Surface** (0x120): +0x18 u64 flags (bit 29 never drawn, 17 boss effect, 24 shadow effect, 26 hidden under water, 23 /
25 / 9 / 10 / 14 / 15 / 8 layer selection in modelGetDlLayer, 0x2200 reflection), +0x1C texture mode (1 / 2), +0x2C /
+0x3C texture indices, +0x60 GS register packet (A+D: TEX0, ALPHA, TEST...), +0xC0 second-pass registers, +0x10C alpha.

**Draw packet** (8 words): [0] size, [1] LOD, [2] surface index, [3] part (-1 always), [4] vertex block address, [7] its
DMA chain (CALL surface registers, REF vertex block, MSCAL, RET). A vertex block holds world batches (GIF tag, V3-32
positions, V2-16 ST, V4-8 colours, MSCNT each; STMOD 1 ST offset from the VIF ROW).

**Model instance** (the walk's geometry / instance record): +0 object (object +0x20 = header), +0xC the instance chain
(a NEXT tag past its 15 qwords of lights / colours to a RET packet; rigid parts: one matrix at VU1 qword 0; skinned: the
palette, up to ~75 matrices; double-buffered by frame, round 34), the instance packet's +0xD0 / +0xDC (written from +0x24C
and the opacity), +0x104 the skin palette block ((4i + ((i + 1) >> 6)) << 4 per bone, decomp_skinpal), +0x194 a parent
matrix block, +0x230..+0x238 view depths / screen point (modelClipPoint writes a1 + 0x230), +0x240, +0x244 fade, +0x248
second fade, +0x24C, +0x250 u64 (LOD scale; LOD level bits 40-42), +0x260 u64 draw flags (bit 2 skips the opacity write,
bit 4 out of the second sort list, bit 8 visible). Instance lights (to VU1 0x3D8): qwords 1-4 the light matrix L, 5-8 the
colour matrix C.

**Layout object / class** (decomp_walk.cpp:5-45): +0x1B6 billboard turn, +0x1B7 camera link, class callbacks +0xD0 draw, +0xD4 lights, +0xD8 update, +0xDC clip, +0xE0 light callback; the class's +0x9C LOD scale; sort key at +4;
a character's +0xCC0 told when visibility changes. clip140's per-object result is rec +0x20..+0x34, geo +0x230..+0x23C and
+0x250..+0x257 plus v0 / f0. Walk globals (decomp_walk.cpp:118-123): group base 0x118CB20, walk on 0x118CB00, sort lists
0x118CB08 / 0x118CB0C, strips off 0x118CAF8, cloth on 0x118CAF4, LOD on 0x118BFD4, LOD modes 0x1191FE0, class table
0x1194070.

**Sort list** (decomp_dl.cpp:4-10): +4 bucket count, +8 scale (float), +0xC mode (0 ascending, 1 descending), buckets
chained through +8 / +0xC in key order.

**Camera context** (*(0x122A8F0 + 4 x *0x118FE4C)): +0xC lens factor, +0x40 projection, +0x140 eye, +0x1A0 view,
+0x1E0, +0x220 / +0x260, +0x4A0 (GS Z rows: zA = [10], zB = [14]; camera packet), +0x520 (mirror camera packet), +0x5A0
view x projection (the model hook's camera), +0x5E0 clip test, +0x660 screen.

**Display list** (decomp_leaves.cpp:11-17, decomp_isys48.cpp:46-52, decomp_dl.cpp:12-17): record at 0x2132D8: +0 / +4
two words by buffer, +8 / +0xC the packet buffers, +0x10 the work buffer (*0x2132E8; +0x20 its free pointer), +0x14 the
layer (packet) count, +0x18 the buffer index (iosDlGetWorkBufId returns *0x2132F0), +0x1C the current layer; each layer a
0x30-byte packet (+0 0x10000000, +0x10 a NEXT to the following, +0x20 / +0x24 self pointers; +0x24 the layer's tail). The
end packet's GIF tag at 0x1E10C0. Layer stack: depth *0x1E10B0 (max 255), saved layers from 0x213308. The list's
semaphore *0x2132D0. A chain-tail packet is 32 bytes: qword 0 a0 << 32 | 0x50000000, qword 1 the tail's +0x14 << 32 |
0x20000000.

**Layers seen** (native_renderer_map.md): 1 sky / far scenery (separate background buffer), 2 / 3 opaque world (5 / 6
second owner), 4 zFog, 8 glow (boss death) and soft shadows, 9 water refraction copy, 10, 12 / 13 the mirrored world
(off-screen), 14 / 15 reflections back on screen, 16, 17, 18, 19 refraction, 20 front fog / bloom / depth of field, 23
screen copy (haze) and the opening's ribbon, 25 + n sprites, 26 sprite UV / ST, 29 feedbackBlur.

**gsReg packet** (*0x1E11A4; decomp_hot48b.cpp:13-21, decomp_leaves.cpp:18-26): +0 DMA tag, +4 packet start, +8 GIF tag,
+0xC write pointer, +0x10 / +0x14 / +0x18 counts, +0x1C closed / "no packet" mode; sprite mode *0x1E11A8. A pair is
(value, register), 8 bytes each.

**gl block** (0x1186390; decomp_walk.cpp:2392-2405, round 128's begin128 list, round 117's check size 0x5300): +0 the
camera context the kick uses (checked for 0x6A0 bytes, decomp_walk.cpp:2440-2441; guess that it is a pointer), +4 the
packet write pointer (*0x1186394), +8 the packet start (*0x1186398), +0xC, +0x10 the primitive (from the table at
0x11916C0), +0x14, +0x18 texturing, +0x2C UV mode (1: UV through 0x1BE700), +0x5C perspective texturing, +0x90 the
current colour, +0xA0 / +0xA4 the current ST, +0x1B0 flags (bit 0, bit 3 transformed, bit 4), +0x1B4 the XYZ3 count
(from 0x11916A0), +0x1B8, +0x200 + 64 x i the vertex ring (256 slots: position +0x200, ST +0x210, colour +0x220),
+0x41F0 the ring index, +0x41F4 the Z scale, +0x41F8 + i the ADC bytes, +0x5220. The view stack at 0x1214EE0. glBegin's
jump table 0x11916E0.

**Texture** (decomp_hot46f.cpp:86-110, sotc_native.cpp:13-18, native_renderer_map.md "Mip levels"): the resource's +0x20
is the NTO2 header H (magic 0x324F544E): +0xC the block B, +0x14 pixels offset, +0x18 CLUT offset, +0x1C format (bits 0-5
PSM, 6-11 CPSM, 12-15 levels, 16-19 / 20-23 log2 width / height), byte +0x1F a bit per level (uploaded as PSMCT32 for
PSMT8 / PSMT4: GS block order), TEX1 / TEST fields from +0x54 to +0x94 (MMAG +0x74, MMIN +0x78, K +0x90, L +0x94 per the
map). B: +0 / +4 the STROW pair (the scroll row the game moves every frame), +0xC the CLUT's pixels, +0x10 / +0x14 the
register packet per context, +0x18 the texture's VRAM, +0x1C the CLUT's VRAM (vramAlloc records, +0xE the base), +0x24 the
reference count. Texture list *0x118C184 (node +4 record, +0xC next). Texture state s (texanim): +0..+0xB scroll, +0xC
palette in use, +0x10..+0x1C transfer words, +0x40 source palette, +0x44 + 4 x id the per-buffer destination palette.
Animation record r (+0x20 of the texture): see decomp_texanim.cpp:7-19.

**Transfer request** (decomp_isys48.cpp:72-83, decomp_hot46f.cpp:86-89): +0 type (0 texture, 1 VRAM reset), +4 (type 1)
the manager's new +0x4C, +8 flags (bit 1 forget the placement), +0x10 / +0x14 the texture in slot 0 / 1 (slot 1 = CLUT
request), +0x18 the tag packet in the display list (resolve writes it), +0x1C next. Lists: heads *0x1215E44, tails
*0x1215E48, count *0x1215E4C, indexed by layer.

**VRAM manager** (decomp_hot47g.cpp:2-10, decomp_hot46f.cpp:68): 0x118B8D0 [0] texture, [1] CLUT; +0x4C, +0x50 rover,
+0x54 / +0x58 free list head / count, +0x5C / +0x60 used list; node pool 0x118C464; node +0 prev, +4 next, +8 block record
(+0 state 1 free / 2 used, +4 packed start (bits 16-30) and size (bits 1-15), +8 manager, +0xC node, +0x10 / +0x14
address-order neighbours).

**Cloth grid** ("#" object, sotc_native.cpp:6820-6830): +0x20 block: [0] / [1] two buffers, [2] rows, [3] columns, [4]
the current one; a buffer is 0xB0 bytes (DMA / VIF header, view matrix at qwords 4-7) then per row an UNPACK and columns x
4 qwords (world position, normal, ST(Q), colour floats). The grid mesh's object (decomp_hot48b.cpp:22-33): +0 the grid,
+0xC rows (0x18 each: +0xC normals, +0x10 positions), +0x24 mesh, +0x28 model.

**Lights / fog / effect globals**: boss / shadow effect word 0x1186290 (bit 0 boss, bit 1 shadow), shadow request
0x1186294, underwater 0x1186328, fade colour 0x1186340..48 and layer 0x1186350; glow 0x11862A0 / B0..B8 / C0 / C4; depth
of field 0x11862D0..DC; zFog N / width / margin / far 0x118C46C / 70 / 74 / 80 (near margin 0x118C478); front fog
0x118B8DC..F0; vertex fog factors 0x12147DC..EC; bloom 0x118C094 (Z), 0x118C09C..B0 (targets), 0x118C0C0..14C
(colours, taps, temporal 0x118C12C); ripple time 0x118C030, H 0x1192090; LOD K scale 0x118C180; the layer cache
*0x118BFC8.

---

## 5. What S4 needs

### 5a. What still goes through PS2 packets, and who reads them

With SOTC_NATIVE=1 the GS rasterises nothing (gs_frontend.cpp:822-833), no VU1 program runs, GS local memory is not
written by uploads (gs_frontend.cpp:1668), and modelDisp's packets left the chain in round 13. What remains of the PS2
format is the game building packets and three host-side readers parsing some of them back:

| packet the game builds | who reads it today | could S4 skip it? |
|---|---|---|
| The display list's DMA chain (~1 400-2 200 tags a frame: ~680 NEXT / CALL links, CALL targets RET packets of FLUSH + DIRECT A+D lists, ~50 object packets; round 31) | The DMA thread's walk, for one FINISH a frame (no SIGNAL / LABEL seen in 3 000 chains, round 8); the FINISH interrupt drives texTransResolve | The game waits on FINISH (iosCheckDrawFinish / the GP thread's poll); a host completion without the walk needs every producer's DIRECT data proven free of SIGNAL / FINISH / LABEL (round 31: "VIFcodes only" is not enough). |
| Model draw packets / VIF vertex blocks | The renderer's mesh build, once per packet id (packetId's 32-word sample; plan checks) | Already read as data, not translated per frame; a host mesh cache keyed by the guest packet stays (round 8's plans). |
| Instance chains (lights + skin palette as VU1 UNPACKs) | The hook copies palette and lights (readChainPalette), and the kick re-reads palettes (skinAtKick / kick jobs) | Round 103 sized "palette written into the VU1 packet then read back": ~0.75% of the game thread; kept because the kick's deferred copies read the same packets. dispSkinned's source (class +0x160 x camera +0x1A0) is host-computable. |
| gsReg A+D packets (2D, fades, screen passes) | gsRegHook feeds the 2D assembler by ra; screen passes are re-done by host effect passes | The guest gsReg still writes every pair; the native renderer ignores the packet. Skipping the write needs every reader of the packet gone (the chain walk's DIRECT scan reads it for events). |
| gl* GIF packets (glBegin / glEnd) | glEndHook parses the pairs back | imm128 already skips them at the two quad sites; other users (boss fan, opening ribbon) still build packets. |
| Cloth buffers (DMA / VIF header + grid) | meshDispHook copies the current buffer into a job | The grid's source (row normals / positions, decomp_hot48b.cpp:22-33) is guest float data; the VIF layout is only the buffer's packing. |
| Texture upload packets (texTransResolve: BITBLTBUF / TRXREG / IMAGE, TEX0 / TEX1 / MIPTBP) and the VRAM allocator | Nothing on the host reads them for drawing (textures decoded from NTO2 headers in guest RAM); the chain walk scans their GIF tags | The VRAM allocator's placement state is guest-visible (texture records +0x18 / +0x1C, block reference counts); S4 would keep the game's bookkeeping or prove nothing reads it (guess: only texTransResolve and texInitialize / texDelete, per round 64's census). |
| Camera packets (0x1098E20), microcode uploads (transMicroCode ~237 a frame), shadow chains, ptclman packets | The renderer reads the camera context directly; microcode and camera packets have no host reader (guess, apart from the chain walk); shadows are taken at the pop; particles via jobs | ptcl103 / pkt117 / S1 already skip ptclman packets. |

So the "translation" left is not a per-frame GS emulation: it is (1) the game's own code still building PS2-format
packets that no host drawer needs (~3% of the game thread by round 103's count: ~1.5 ms console), (2) the chain walk for
the FINISH event (DMA thread, ~0.6 ms of a helper core, 2.4 ms console), (3) host parsers that read guest packets back
(2D pairs, gl pairs, palettes from UNPACKs, cloth buffers), and (4) the render thread's own per-draw work on host records.

### 5b. What the game produces that a native draw could consume directly

- **Per model draw**: instance, header, LOD (instance +0x250 bits 40-42 clamped to +0x1C), the part visibility the clip
  writes (+0x2C of the LOD's part table), the model matrix (modelDisp's a1), the camera (+0x5A0), opacity (+0x244 /
  +0x248), lights (instance packet qwords 1-8, made by setLight from the light pass), surfaces' texture / blend / test
  registers (+0x60), and the texture scroll rows (B +0 / +4). The plans (round 8) already hold the per-(header, LOD)
  packet list on the host.
- **Skinned**: the character's +0x160 skinning matrices (ExecCharaGeometry) times the camera's +0x1A0; dispSkinned is the
  only writer of the palette block (+0x104).
- **Cloth**: the grid's guest float rows (the cloth step's output).
- **Particles**: S1's host snapshot at the emitter's step (particles_s1.md 2).
- **2D**: sprite objects (0x50 bytes: texture resource, RGBA, UV / ST, position, scale, rotation, blend bits;
  native_renderer_map.md "2D"); the gauges and windows write gsReg pairs computed inline (guess: no object to read for
  them).
- **Effects**: the parameters are guest globals the host already reads at endFrame (bloom, glow, fog, depth of field,
  ripple); the guest's own sprite passes for them are redundant in native mode.

### 5c. Threads and cores carrying the display today

| thread | core | display work | cost |
|---|---|---|---|
| game thread | 0 | the walk, clip / LOD, light pass, sort lists, the hooks' game-thread half (copies into jobs, plans' checks, scroll rows), the immediate quads, the 2D feed, packet building (gsReg, chain tails, texture requests / resolve, microcode) | display path 14.5% riding / 19.2% shrine of the game thread (Vita3K sampler, rounds 113 / 116) = 8.8 / 7.1 ms on the console (round 113); 6.81 ms riding (r135 console, round 140) |
| render thread | 2 (prio 160) | uploads, planFrame, passes, GL calls (vitaGL), present | Vita3K 35.2% of a core attract (30.5% with S3's jobs), 30.7% at the lake (characters_s3.md 8); 14.0-16.2 ms a frame (round 116); console 21.8 mean / 29.4 p90 ms (round 105), 25-37 ms (49 at ~2 600 draws) (round 113); round 116's console split riding: plan 5-10, passes 12-17, calls 6-8 ms (round 143) |
| hook worker | 1-2 (prio 150) | model / mesh / shadow / particle / kick / 2D jobs: packet walk, mesh builds, CPU skinning, draw records | Vita3K 14.8% / 13.8% of a core (S3), 17.0% lake; console 9.5 mean / 16.5 p90 ms (round 105); round 89 console 14-16 ms (mesh jobs ~300 KB of meshes a frame, lag 5-6 ms) |
| mds lane | 1-2 | modelDispState | Vita3K 2.9%; console 2.7 ms (round 105) |
| DMA thread | 1-2 | chain walk | Vita3K 3.3% ("VIF walker", S3); console 2.4 ms |
| offload | 1-2 | tex64 (1.8%), clip140 (0.9%), pf141 (1.1%) | console offload 2.1 ms (round 105) |
| build | 1-2 | mesh / texture builds on loads | console 2.5 ms (round 105; mostly idle polling before round 105's build105) |

Cores 1-2 were 70-85% busy on the console (round 89); round 113: ~10-15 ms a frame free at 30 fps. Vita3K's per-core
figures are instructions at the throttle; the console's cores 1-2 have real memory waits (characters_s3.md 8).

### 5d. Blockers and risks

- **Guest-visible writes of the walk.** Rounds 103, 116 and 127: the walk's natives keep every register, VU0 / CLIP state
  and memory write exact for the recompiled callers; 50+ class callbacks write state the next frame reads (LOD
  hysteresis, fade, depth keys, visibility, the character's +0xCC0). A host scene table must reproduce each or keep the
  guest walk as the producer.
- **The CLIP register chain.** modelDisp's root test ORs in the three earlier judgements; the register chains from model
  to model through every test between (round 61). Readers: clipBySphere, clipByMinMaxCube, ClipPoint, 0108C9C0, 0108C188,
  modelClipPoint, glVertexKick. Skipping guest modelDisp is already handled by the lane's pending register
  (clipShift / clipNeeded); any new skip must keep that.
- **The layer cache and shadow flag.** modelSetSurface / modelResetSurface and four model functions read *0x118BFC8;
  setShadowEffect / bossDeathEffectEnd read 0x1186294 (round 61).
- **Ordering.** The renderer's draw order is the order hooks and jobs run (layers by `order`, then index). 2D batches,
  shadow volumes, particles and models interleave in the job ring; any direct path must emit in the same place
  (round 23's rule; SOTC_HOOK_ORDER_CHECK).
- **Inputs that change after the hook.** The cloth grid flips buffers (98.5% changed before the job ran), the palettes,
  instance chains (double-buffered), scroll rows and the K scale move between hook and kick (rounds 23 / 34). Any
  deferred reader must snapshot at the hook.
- **The FINISH event and texTransResolve.** The game's frame pacing (GP thread poll, iosCheckDrawFinish) and the texture
  resolve hang off the chain's FINISH; removing the walk changes when the guest sees completion (round 60 already moved
  it to the first poll).
- **Readers of guest buffers in place.** A 64-byte CLUT the game animates in place each frame is shared by both display
  lists (round 60's "touched in flight" finding); texReset / texAnim write palettes the kick sends.
- **The return-address contract.** Hooks key on ra ranges (k2D, feedback, refraction, bloom, shadow pops, haze); natives
  must keep calling through the table with original ra (section 1). Lifting or redesigning a caller can silently break
  a range.
- **iosDlLockObj** defers an object's delete past the frame's DMA; the jobs and the lane read the instance after the walk
  (round 103): a direct path that reads instances later must keep the lock or copy.
- **Exactness rule vs port.** Most of the area is exact; walk83's lights and S1's particles are port-level. S4's
  equivalence needs the in-run per-call compares (draw taps, job compares) that rounds 7, 8, 103, 117, 128 used: two runs
  are not frame-identical.
- **The render thread's remaining cost is per draw**, and 94% of the splits between draws are texture changes (round 116):
  fewer draws need reordering (picture changes) or atlases (edge filtering); both broke the picture rule in rounds 87 /
  116. Freeing core 2 means moving or shrinking plan / pass / call work, not the hook side.
- **The GPU is the limit in heavy frames** on the console (round 73: GPU ~55-60 ms for a heavy frame at 512 x 448;
  round 86: render-side frames are render + GPU backlog). Freeing the render thread's CPU does not shorten those frames.

### 5e. Candidate first steps (guess; from the rounds' own "not done" lists)

1. Skip the PS2-format producers that no host drawer reads in native mode, each after a reader census like round 61's:
   the gsReg pair writes for the k2D ranges, transMicroCode, the camera packets, the instance chain's palette UNPACKs
   (with the kick's copies taken from class +0x160 x camera instead), the cloth buffer's VIF packing.
2. A host completion for the frame (no chain walk) once (1) leaves only proven event-free packets in the chain.
3. Render-thread work moved earlier: per-record state compares and texture lookups done when the job makes the record
   (the hook worker is on cores 1-2 too), or a plan cached across frames for static draws (round 74 / 116 territory).

---

## 6. Costs earlier rounds measured

| where | figure | round |
|---|---|---|
| Console, heavy scene (2026-10-22) | modelDispHook's packets ~10 ms self, hook.state 3-4, packetId 3, modelClipSolid 2.4 ms for ~200 calls (12 us a call vs 0.17 on the Mac: DRAM misses) | docs:248-262 |
| Console, attract | "dispSort_010F4FB8" (the light pass) 2.75 ms self; Vita3K 1.69 -> 1.06 ms after decomp_light | docs:205-243 |
| Vita3K 240, attract | DispDispObjGroupModel 12.6 ms of the frame (4.4 unprobed): modelDisp 3.2, particle display 3.9, light pass 1.2, haze strips 1.3, CalcLightLayoutObj 0.8, cloth 1.0, modelClipSolid 0.5; after round 7: 11.74 | 7 |
| Vita3K 240 | modelDisp 3.41 -> 2.51 ms attract; route hook.packets 4.02 -> 1.92, packetId 1.15 -> 0.11; iosDmaSendSChainPath1 2.04 -> 1.63; route frame 45.8 -> 41.6 | 8 |
| Census, shrine | modelDisp ~50 calls (35 models, ~2 800 draw packets), modelClipSolid / Skin 124 / 20, meshDisp 7-15, ptclmanEnd ~50, spriteTexFast 192, texTransRequest ~310, transMicroCode ~237 a frame | native_renderer_map.md |
| Plain, per frame | ~100 instances, ~4 000 packets, ~3 200 draws over ~440 surfaces and ~200 textures (SOTC_HOOK_STATS) | docs:255-258 |
| Vita3K 222, process | game 1.0 core, render thread 0.3 (0.65 after the shrine); renderer hooks 8.6% of the game thread; Vif1Walker::feed 1.55% + writeIORegister 1.43% | 23 |
| Vita3K 222, walk window | iosDmaSendSChainPath1 3.2 ms a frame; Vif1Walker::feed 1.5% + chain loop 1.4% | 31 |
| Vita3K 222 / 300 | DMA walk 0.56-0.61 / 0.40 ms on the worker; game -1.6..-1.9 ms; render thread 17.2-21.5 ms a frame | 60 |
| Vita3K 222 | hook.state 1.7 ms (title world); lane: game -0.6..-1.6 ms | 61 |
| Console r72 (1 000+ draws) | frame 59.8 ms; render draw part 27.9 (CPU 23.6), present 30.3, game wait 24.2; GPU ~55-60 ms a heavy frame; picture 512 x 448 | 73 |
| Vita3K altar | 37 GPU scenes, 36 target binds, 12.3 MB depth / stencil a frame | 73 |
| Console r73 (1 000+ draws) | render draw part 38-44 ms wall (~30 CPU), present 16-20; 1 300-2 500 draws; altar 1 140 records -> 385 GL draws + ~100 alpha-test second passes | 74 |
| Vita3K 222, altar | group pass 4.3 ms; ClipLayoutObjModel 2.4 ms for 146 objects (16 us each); light pass 2.3 (SetGeoDispClassLightColMan ~85 us a call, 15-20 lit objects) | 83 |
| Vita3K 222 A/B | walk83: altar +2.76 ms, route 6 +1.45, attract +0.49-0.59 | 83 |
| Console r84 | render-side frames 40.3 ms = render 15.6 + present 23.5 (~250 draws, 17-22 targets); game-bound frames 46.6 (render 19.0 + present 1.4) | 86 |
| Vita3K 222, by subsystem | display walk: altar 10.6 ms (own 5.3: group pass 2.9, light 1.1; modelDisp 2.4, sort 1.3, particle draws 1.1, cloth meshes 0.8), route 7.4 (own 4.0), attract 9.5 (own 5.8) | 89 |
| Console r87 | render 22-50 ms CPU (core 2), worker 14-16 (lag 5-6), lane 2-4, build 3-4, dma 3, offload 3; cores 1-2 70-85% busy | 89 |
| Vita3K 222, with probes | display walk 18.4 / 11.9 / 10.8 ms (altar / route 6 / riding); display list send (iosDlTrans, GPFrameInit, DMA) 1.9 / 1.2 / 0.8; texAnim 1.0 / 0.7 / 0.8 | 96 |
| Vita3K 222, altar sampler | display path ~15%: walk natives 3.7 (clip83 1.1), model / mesh hooks 4.7 (palette read back 0.75, scroll rows 0.75, lane joins 0.3), light 1.4, prefetch 1.2, sort lists 0.6, particle draw chain ~2.1; PS2-format layers ~3% | 103 |
| Console r103 | game 46.3 / 65.4 (mean / p90), render 21.8 / 29.4, hook worker 9.5 / 16.5, lane 2.7, dma 2.4, offload 2.1, build 2.5; game waits: sync end 1.84, lane help 1.04, hook joins 0.50 | 105 |
| Console r110 | render thread 25-37 ms (49 at ~2 600 draws); GPU occupancy 37-52 ms field, 42 shrine; display path 8.8 ms riding / 7.1 shrine of the game thread | 113 |
| Vita3K 222 | render thread 14.0-16.2 ms a frame; fat116 -1.6..-2.0 ms; riding 1 426 draws -> 538 GL draws; splits: texture 94% | 116 |
| Vita3K 222, riding | immediate-mode vertices 4.03% -> 3.32% of the game thread (glVertexKick 1.76 + glVertex3fv 0.38 -> quad117 1.42); 108 kicks a frame (98 haze) | 117 |
| Console r116 | run copies 47-142 MB per 600 frames in 1.2-5.0 s (~35 ms / MB, GPU memory reads); textures ~10 ms each on slow frames | 118 |
| Vita3K | 141 textures 188 ms (alpha scan 120, swizzle 39, vitaGL 24) -> 148 textures 91 ms | 118 |
| Vita3K 222, riding sampler | quad117 1.73, walk 1.33, hookScrollRows 1.04, clip83 1.10, mdsPartWalk 0.78, mdsPlanPackets 0.62, modelDispHook 0.60, strips 0.57, prefetch::note 0.52, sortOrder 0.49, texreset 0.47, reflectionBegin 0.44, lightPass 0.42, gsRegHook / gsReg / vertex2D 0.9 | 127 |
| Vita3K | immediate-quad functions 4.45 -> 3.34% (imm128) | 128 |
| Console r130 / r132 | `sync end` (the hand-over wait) ~15-25 ms a frame in play, present 15-42 (before pace132) | 132 |
| Console r132, riding | manager_sub_010F3FB0 (haze strips) 3.2%; mdsjoin::wrap<8> (modelResetSurface's lane join) 1.2% | 134 |
| Console r135, riding | display 6.81 ms of the game thread: modelDisp entry 2.37% (2.28 in syncFrame's zone), walk 2.11, clipLayoutObjModel 2.06, haze 1.65, solidDisp 0.93, getLayoutClass 0.59 | 140 |
| Vita3K 222 | touched walk functions 5.04 -> 3.35% (clip140, sort140, quad140); field game 14.3 -> 13.8 ms | 140 |
| Console r135 | `wait render` 3.02 ms a frame overall, ~1.2 in play; present-bound windows (21% of play) wait 2.67 vs 0.75 | 143 |
| Vita3K 222, attract (S3) | render thread 35.2% of a core (30.5 with S3's jobs), hook worker 14.8%, VIF walker 3.3, mds lane 2.9, texture animation 1.8, pf141 1.1, clip140 0.9; lake: render 30.7, hook worker 17.0 | S3 |
| Phase 2 plan (emulated path) | Vita3K 444, eagle cliff: ~450 VU1 programs a second, VU1 thread ~88% busy, ~0.6 fps; ~6 000 world batches a frame | vu1_gpu_path.md |

Not stated in the sources read: a console split of the render thread after round 116; the GPU time per scene on the
console with names (round 86's `[gpu86s]` was to give it); the cost of the guest packet building alone on the console
(round 103's ~1.5 ms is a scaled estimate); how much of the hook worker's time is mesh building versus skinning versus
record making on the console.

## 7. Every function of the area

The area as tools/decomp_status.py counts it after D11 (977 functions: 507 native, 470 recompiled), generated by
build/agent-scratch/d11/dispmap.py from the final build's honest status: the source file (or "recompiled"), the direct
callers and callees (jal), the argument offsets and globals each touches (R / W and width), and for the recompiled ones
what reaches them (the call-count runs of D7-D11) or why they are left.

| address | function | size | now | direct callers | callees | touches | reached by / would reach it |
|---|---|---:|---|---|---|---|---|
| 01b1138 | iosRecvMsg | 376 | sotc_native.cpp | kernel_sub_001AD270, kernel_sub_001C8078, kernel_sub_001C80A0, mcManager | SignalSema, WaitSema, iosCallBackThreadMgr, iosGetThread, iosJumpRecoverPoint +1 | $a0: +0x0:R4 +0x4:R4 +0x8:R4/W4 +0xc:R4 +0x14:R4/W4 +0x18:R4/W4; $a1: +0x0:W4; globals: 001e0be8:&R 001e2288:& |  |
| 01b92d0 | iosScaleMatrixV | 56 | decomp_layout.cpp | DebugDispVolumeShapeColor, DispWithSwordBlur, DispWithTorchBlur, SCRFuncHorseBossCircumferenceMain, UpdateLayoutClassGeometry, UpdateLayoutClassSimpleGeometry +7 | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16; $a1: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a2: +0x0:R16 |  |
| 01c0138 | iosDlGetWorkBufId | 24 | decomp_texanim.cpp | debugLineObjectDisp, graphFrameInit, graphInitAllLayer, graphInitLayer, graphSetFogParam, iosGsInitDC +7 | - | globals: 002132d8:& 002132f0:R |  |
| 01c30d0 | set_nowloadingflag | 624 | sotc_native.cpp | bootGPThread, gamecore_sub_013529F0, setNowLoadingMessageDisplayFlag, st_gameflow | getDispScreenHeight, getDispScreenWidth, getDrawScreenHeight, getDrawScreenWidth, iosGetCosf +2 | globals: 001e0c88:& 001e117c:&RW 001e1180:&RW 001e1188:W |  |
| 107b9e8 | DEBUG_disp_object_info | 848 | recompiled | gamecore_sub_0133F0E0 | DEBUG_get_cur_using_anb_file_name, DebugGetAnimObjName, GetCharacterClass, GetCharacterClassCurAnimationCurFrame, GetCharacterClassCurAnimationTotalFrame +4 | globals: 0118feb0:& 0118ff20:& 0118ff28:& 0118ff40:& 0118ff50:& 0118ff58:& 0118ff70:& 0118ff80:& 0118ff90:& 01 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107bd38 | DEBUG_disp_ray | 448 | recompiled | - | kernel_sub_001B7B60, memset | $a0: +0x0:R16; $a1: +0x0:R16; $a3: +0x0:R16; globals: 001e0f00:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107bef8 | DEBUG_disp_cylinder_object | 872 | recompiled | DEBUG_disp_region_object | iosRotMatrixX, iosRotMatrixY, iosScaleMatrix, iosUnitMatrix, kernel_sub_001B7B60 | $a0: +0x0:R16; $a1: +0x0:R16; globals: 01185d04:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107c260 | DEBUG_disp_collision_attribute_info | 760 | recompiled | DEBUG_disp_collision_attribute_info_extend | iosSetKPrintfLocateContext, kprintfContext | globals: 01191360:& 01191380:& 011913a0:& 011913c0:& 011913d0:& 011913e0:& 011913f0:& 01191400:& 01191410:& 01 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107c558 | DEBUG_disp_center_collision_attribute_info | 360 | recompiled | - | kprintfContextCentering | globals: 01191448:& 011914b0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107c718 | disp_debug_utility | 56 | recompiled | gamecore_sub_01341E80 | manager_sub_0107B2C8 | globals: 01185cf8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107c7c8 | DEBUG_get_2d_font_disp_position | 192 | recompiled | gamecore_sub_0133F0E0 | getDrawScreenHeight, getDrawScreenWidth | $a0: +0x0:W4; $a1: +0x0:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107ca20 | DEBUG_get_parent_layout_name | 72 | recompiled | - | GetLayoutClass, GetLayoutParentObj | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107ca68 | DEBUG_disp_region_object | 192 | recompiled | disp_camera_region | DEBUG_disp_box_object, DEBUG_disp_cylinder_object, DEBUG_disp_sphere_object | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107cb28 | DEBUG_disp_sphere_object | 464 | recompiled | DEBUG_disp_region_object, disp_camera_lws_animation_debug_info, manager_sub_01161390 | iosScaleMatrix, kernel_sub_001B7B60 | $a0: +0x0:R16; $a1: +0x0:R16; globals: 01185d00:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107ccf8 | DEBUG_disp_box_object | 160 | recompiled | DEBUG_disp_region_object, manager_sub_01161390 | kernel_sub_001B7B60 | $a0: +0x0:R16; $a1: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107cd98 | DEBUG_disp_collision_attribute_info_extend | 240 | recompiled | gamecore_sub_01341AF8 | DEBUG_disp_collision_attribute_info, DebugGetParentCharColAttr, GetLayoutClass, GetLayoutParentObj, iosSetKPrintfLocateContext +1 | globals: 01191440:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107ce88 | DEBUG_disp_collision_name | 216 | recompiled | gamecore_sub_0133F0E0 | DebugGetParentCharColAttr, GetLayoutClass, GetLayoutParentObj, iosSetKPrintfLocateContext, kprintfContext | globals: 01191440:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107d1b8 | GAMEUTIL_get_limit_clip_yaw_rel | 272 | recompiled | - | GAMEUTIL_get_limit_clip_yaw_abs, manager_sub_01082BD8 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 107e5e8 | GAMEUTIL_act_pitch_rot_limit_clip | 520 | recompiled | - | GAMEUTIL_act_pitch_rot, iosApplyRotMatrix, iosGetACosfExact, iosRotMatrixX | $a0: +0x0:W16 +0x4:R4 +0x8:R4; globals: 001e0fc0:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1080508 | GAMEUTIL_increase_yaw_limit_clip | 32 | recompiled | gamecore_sub_01347E40, gamecore_sub_01347EE8, gamecore_sub_01348298, get_max_min_clip_base_yaw_nico_yaw_pitch_cntrl | GAMEUTIL_increase_yaw_limit_clip_extend | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1080528 | GAMEUTIL_increase_yaw_limit_clip_extend | 232 | recompiled | GAMEUTIL_increase_yaw_limit_clip, get_merge_yaw_nico_yaw_pitch_cntrl | GAMEUTIL_get_difference_yaw_and_yaw, GAMEUTIL_increase_yaw_limit_extend, GAMEUTIL_is_between_yaw_and_yaw | $a0: +0x0:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1080610 | GAMEUTIL_get_limit_clip_yaw_abs | 152 | recompiled | GAMEUTIL_get_limit_clip_yaw_rel, gamecore_sub_013A9060 | GAMEUTIL_get_move_radi_from_yaw1_to_yaw2, GAMEUTIL_is_between_yaw_and_yaw_extend_abs | $a0: +0x0:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10807e0 | GAMEUTIL_increase_pitch_limit_clip | 392 | decomp_liftd2_00.cpp | gamecore_sub_01347F88, gamecore_sub_01347FC8, gamecore_sub_01348218, gamecore_sub_01410240, gamecore_sub_014220D8, gamecore_sub_01422710 +4 | - | $a0: +0x0:W4 |  |
| 1080f50 | GAMEUTIL_is_reach_aim_yaw_angle | 320 | decomp_liftd2_00.cpp | gamecore_sub_01416AC8, manager_sub_01084A70 | - | $a0: +0x0:W4 |  |
| 1081090 | GAMEUTIL_is_reach_aim_pitch_angle | 200 | decomp_liftd2_00.cpp | gamecore_sub_01416AC8 | - | $a0: +0x0:W4 |  |
| 1081158 | GAMEUTIL_near_to_aim_yaw_angle | 384 | recompiled | - | GAMEUTIL_inc_or_dec_for_near_to_aim_yaw | $a0: +0x0:R4/W4; $a1: +0x0:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10812d8 | GAMEUTIL_near_to_aim_pitch_angle | 264 | decomp_liftd3_00.cpp | manager_sub_01084C30 | GAMEUTIL_inc_or_dec_for_near_to_aim_pitch | $a1: +0x0:W4 |  |
| 1084300 | GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl | 8 | decomp_liftd4_00.cpp | CAM_constructor_follow_rot, CAM_doit_view_cut_avoid_cntrl, CAM_init_aim_angle_rot, CAM_reset_view_cut_avoid_cntrl, GAMEUTIL_constructor_look_at_vs_offset | - | $a0: +0x4:W4 |  |
| 1084880 | constructor_aim_angle_interp_cntrl | 40 | decomp_liftd4_00.cpp | CAM_reset_target_offset, constructor_camera_default_angle | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 |  |
| 10848a8 | reset_aim_angle_interp_cntrl | 16 | decomp_liftd2_00.cpp | doit_camera_default_angle | - | $a0: +0xc:W4 |  |
| 10848b8 | doit_aim_angle_interp_cntrl | 176 | decomp_liftd2_00.cpp | doit_camera_default_angle, gamecore_sub_01410EE8, gamecore_sub_01417ED8 | manager_sub_01084A70, manager_sub_01084C30 | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4/W4 |  |
| 1084968 | set_interp_param_aim_angle_interp_cntrl | 16 | decomp_liftd2_00.cpp | doit_camera_default_angle | - | $a0: +0x4:W4 +0x8:W4 |  |
| 1085258 | bgDraw | 352 | decomp_liftd4_00.cpp | zFog | getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSizeTBP, gsReg, gsRegBegin +4 | globals: 01191530:&R 01191538:R 01191540:&R 01191548:R |  |
| 1085640 | bossDeathEffectBegin | 2048 | sotc_native.cpp | graphFsEffectReset | getDrawBufferAddressFBP, getDrawBufferAddressTBP, getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSize +17 | globals: 01186280:& 01186290:&R 01186298:&RW 0118629c:R 01191590:&R 01191598:R 01214740:&R 01214744:R |  |
| 10871c8 | setShadowEffect | 16 | sotc_native.cpp | modelDisp | - | globals: 01186294:W |  |
| 1087260 | clipBySphere | 160 | sotc_worker.cpp | manager_sub_0113B1D8 | iosgluGetCameraContext | - |  |
| 1087300 | clipByMinMaxCube | 408 | decomp_walk.cpp | dischargeMain, fallenLeafMain, manager_sub_0113B1D8, modelClipSolid | - | $a0: +0x0:R16; $a1: +0x0:R16; $a2: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16 |  |
| 1088b10 | fogEffectSetParam | 16 | recompiled | - | - | globals: 011862f4:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1089120 | initFogEffect | 336 | decomp_liftd7_01.cpp | - | changeFogParam, getFogColorB, getFogColorG, getFogColorR, getFogFarDist +4 | $a0: +0x10:R4 |  |
| 1089270 | deleteFogEffect | 8 | sotc_prof.cpp | - | - | - |  |
| 10893c0 | foginterp | 816 | decomp_liftd6_00.cpp | ExecFogColMan | graphSetFogParam, iosSetBGColor | globals: 01186320:&RW 01186324:RW 0118632c:R 012147b0:& 012147b4:R 012147b8:R 012147bc:R 012147c0:R 012147c8:R |  |
| 10896f0 | setFogImmediate | 472 | decomp_liftd3_00.cpp | changeFogParam | graphSetFogParam, iosSetBGColor | globals: 01186324:RW 0118632c:R 012147b0:&R 012147b7:R 012147b8:R 012147bf:R 012147c0:R 012147c7:R 012147c8:R  |  |
| 10898c8 | GetNowFogColorR | 16 | recompiled | - | - | globals: 012147f4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10898d8 | GetNowFogColorG | 16 | recompiled | - | - | globals: 012147f8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10898e8 | GetNowFogColorB | 16 | recompiled | - | - | globals: 012147f8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10898f8 | getFogMode | 16 | decomp_liftd4_00.cpp | graphFrameFinish, manager_sub_010881D0 | - | globals: 012147b0:& |  |
| 1089908 | getFogNearDist | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147b4:& |  |
| 1089918 | getFogFarDist | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147b8:& |  |
| 1089928 | getFogNearRate | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147bc:& |  |
| 1089938 | getFogFarRate | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147c0:& |  |
| 1089948 | getFogRate | 16 | recompiled | - | - | globals: 012147c4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1089958 | getFogOffset | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147c8:& |  |
| 1089968 | getFogUnderWater | 16 | decomp_liftd6_01.cpp | ExecFogColMan, dischargeMain, gamecore_sub_01352888, gamecore_sub_0138D910, manager_sub_010EBDB0, manager_sub_0113A5A8 +5 | - | globals: 01186328:& |  |
| 1089978 | getFogColorR | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147cc:& |  |
| 1089988 | getFogColorG | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147d0:& |  |
| 1089998 | getFogColorB | 16 | decomp_liftd4_00.cpp | ExecFogColMan, effectSet, initFogEffect, manager_sub_010EBDB0 | - | globals: 012147d4:& |  |
| 10899a8 | changeFogParam | 24 | decomp_liftd4_00.cpp | effectSet, initFogEffect | setFogImmediate | - |  |
| 10899c0 | SetAndLockBgColor | 104 | recompiled | - | iosSetBGColor | globals: 0118632c:W 01214800:W 01214804:W 01214808:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1089a28 | UnlockBgColor | 16 | recompiled | - | - | globals: 0118632c:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1089a38 | SeemlessInitFogTargetParamWorldLayout | 104 | decomp_liftd7_01.cpp | gamecore_sub_01367778 | - | $a0: +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4 +0x30:R1 +0x31:R1 +0x32:R1 +0x33:R1; globals: 01186320:W 012147b0:& 0 |  |
| 1089aa0 | setFogParamDefault | 192 | decomp_liftd6_01.cpp | st_reloadtask | - | globals: 011862f8:&R 011862ff:R 01186300:R 01186307:R 01186308:R 0118630f:R 01186310:R 01186317:R 01186318:R 0 |  |
| 1089b60 | fogSetFogColorMulRatio | 16 | decomp_liftd7_01.cpp | gamecore_sub_013FB270 | - | globals: 01186324:W |  |
| 1089b70 | getFogFactor0 | 64 | decomp_liftd2_00.cpp | glSetFogFactor, manager_sub_01098E20, ptclmanBuildEnvPacket | - | globals: 012147d8:& 012147dc:R 012147e0:R 012147e4:R 012147e8:R |  |
| 1089bb0 | getFogFactor1 | 72 | decomp_liftd2_00.cpp | glSetFogFactor, manager_sub_01098E20, ptclmanBuildEnvPacket | - | globals: 012147d8:& 012147dc:R 012147e0:R 012147e4:R 012147e8:R 012147ec:R |  |
| 1089bf8 | feedbackBlur | 600 | recompiled | - | getDispBufferAddressTBP, getDispScreenHeight, getDispScreenWidth, getDrawScreenHeight, getDrawScreenWidth +6 | globals: 01186330:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1089e50 | feedbackBlur2 | 656 | decomp_liftd7_09.cpp | gamecore_sub_014098A0, gamecore_sub_0140A8A8, graphFrameFinish | getDispBufferAddressTBP, getDispScreenHeight, getDispScreenWidth, getDrawScreenHeight, getDrawScreenWidth +7 | globals: 01186330:&RW |  |
| 108a0e0 | screenBrightness | 424 | decomp_liftd8_00.cpp | graphFrameFinish | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +2 | - |  |
| 108a288 | drawFrontFog | 384 | sotc_native.cpp | graphFrameFinish | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +2 | globals: 0118b8d0:& 0118b8dc:R 0118b8e0:R 0118b8e4:R 0118b8f0:R |  |
| 108a408 | fadeScreen | 384 | decomp_liftd3_00.cpp | gamecore_sub_01346368 | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +2 | globals: 01186340:&R 01186344:R 01186348:R 01186350:R |  |
| 108a588 | drawDemoBar | 504 | decomp_liftd3_00.cpp | gamecore_sub_01346368, gamecore_sub_0135FC30 | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +2 | - |  |
| 108a780 | screenBlur | 2440 | sotc_native.cpp | cameraBlur, gamecore_sub_0140A6A8 | getDrawBufferAddressFBP, getDrawBufferAddressTBP, getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSizeFBP +9 | globals: 01191680:&R 01191688:R |  |
| 108b108 | GetBlurRadius | 128 | decomp_liftd2_00.cpp | cameraBlur | - | globals: 01191690:R |  |
| 108b188 | cameraBlur | 2008 | decomp_liftd2_00.cpp | graphFrameFinish | GetBlurRadius, getDrawScreenHeight, getDrawScreenWidth, gluGetLookAtPosition, iosGetATan2fExact +2 | globals: 01180000:R 01186354:R 01186358:R 0118635c:R 01186360:R 01186364:R 01186368:&RW 0118636c:R 01186370:R  |  |
| 108b960 | skipBlur | 16 | recompiled | fsEffectReset | - | globals: 01186330:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108b970 | setFadeColor | 72 | decomp_liftd7_01.cpp | fadeBlackOut, fadeOut, fadeWhiteOut, scrFadeBlackIn, scrFadeBlackOut, scrFadeWhiteIn +3 | - | globals: 01186340:&W 01186344:W 01186348:W |  |
| 108b9c8 | cameraBlurClearIntensityArray | 96 | decomp_liftd4_00.cpp | SetLayoutDisplayFlag, effectSet | gluGetLookAtPosition | globals: 01214eb0:&W 01214ec0:& 01214ed8:& 01214edc:&W |  |
| 108ba28 | getCameraBlurSw | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186354:& |  |
| 108ba48 | getBlurWorkSize | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118635c:& |  |
| 108ba58 | getNBlurPlanes | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186360:& |  |
| 108ba68 | getBlurIntensity | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186364:& |  |
| 108ba78 | getBlurZBase | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118636c:& |  |
| 108ba88 | getBlurMinThreshold | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186370:& |  |
| 108ba98 | getBlurMaxThreshold | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186374:& |  |
| 108baa8 | getBlurShutterSpeed | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186378:& |  |
| 108bab8 | getBlurXYZRatio | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118637c:& |  |
| 108bac8 | getBlurCamMoveThreshold | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186380:& |  |
| 108bad8 | getBankAngle | 96 | recompiled | - | iosGetATan2fExact | $a1: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108bb38 | glBeginWithDlLayerNo | 616 | recompiled | glBegin, gluSetWrapMode, manager_sub_0108BDA0, manager_sub_0108C5D8, manager_sub_0108D730 | iosDlPushLayer, iosGetPacketPtrPath1 | globals: 01186390:& 01186394:&RW 01186398:RW 0118639c:W 011863a0:RW 011863a4:RW 011863a8:R 011863ac:R 011863b0 | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 108d060 | glEnd | 240 | decomp_walk.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispCollisionFinderKeep, DebugDispMotion, DispWithSwordBlur +29 | iosDlChainTailCurrent, iosDlPopLayer, iosSetPacketPtrPath1, manager_sub_0108CD80 | globals: 01186390:& 01186394:R | reached (8439954 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4) |
| 108d150 | glBlendFunc | 320 | decomp_liftd8_00.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispCollisionFinderKeep, DebugDispMotion, disphalfarc +12 | - | globals: 01186390:& 01186394:&R 01186398:&RW 011863c0:R |  |
| 108d290 | glBlendFuncParticle | 200 | decomp_liftd8_00.cpp | manager_sub_01142080, manager_sub_01142230 | - | globals: 01186390:& 01186394:& 01186398:&RW 011863c0:R |  |
| 108de88 | glRotatef | 1072 | recompiled | - | iosGetSinCosf, memset | globals: 01214ee0:& 012156e0:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e2b8 | glReset | 160 | decomp_hot48b.cpp | graphFrameInit | iosDlChainTailCurrent, iosDlPopLayer, iosDlPushLayer, setLight | globals: 01186390:& 011863ac:W 011863bc:W 011863c0:W 011863c4:W 011863c8:W 011863ec:W 011864c0:& 01186500:& 01 | reached (854088 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4) |
| 108e358 | glEnable | 240 | decomp_walk.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispCollisionFinderKeep, DebugDispMotion, DebugDispVolumeShapeColor +22 | - | globals: 01186390:& 011863a8:W 011863ac:W 011863b0:W 011863b4:W 011863b8:W 011863bc:W 011863c0:W 011863c4:W 01 |  |
| 108e448 | glDisable | 224 | decomp_walk.cpp | DebugDispMotion, DebugDispVolumeShapeColor, DispWithSwordBlur, DispWithTorchBlur, SCRFuncSetAttainForParallelTrans, SCRFuncSetAttainForPreParallelTrans +13 | - | globals: 01186390:& 011863a8:W 011863ac:W 011863b0:W 011863b4:W 011863b8:W 011863bc:W 011863c0:W 011863c4:W 01 |  |
| 108e528 | glMatrixMode | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e530 | glBegin | 48 | decomp_walk.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispCollisionFinderKeep, DebugDispMotion, DispWithSwordBlur +25 | glBeginWithDlLayerNo, iosDlGetLayer | - | reached (8238505 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4) |
| 108e560 | glNormal3f | 144 | recompiled | gamecore_sub_01382610 | iosApplyRotMatrix | globals: 01186390:& 01186410:&RW 01186540:RW 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e5f0 | glNormal3fv | 176 | recompiled | - | iosApplyRotMatrix | $a0: +0x0:R4 +0x4:R4 +0x8:R4; globals: 01186390:& 01186410:&RW 01186540:RW 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e6a0 | glNormal4f | 144 | recompiled | - | iosApplyRotMatrix | globals: 01186390:& 01186410:&RW 01186540:RW 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e730 | glNormal4fv | 128 | recompiled | - | iosApplyRotMatrix | globals: 01186390:& 01186410:&RW 01186540:RW 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e7b0 | glColor3f | 128 | recompiled | - | - | globals: 01186390:& 01186420:&RW 01186424:&W 01186428:W 0118642c:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e830 | glColor3fv | 136 | recompiled | - | - | $a0: +0x0:R4 +0x4:R4 +0x8:R4; globals: 01186390:& 01186420:&RW 01186424:&W 01186428:W 0118642c:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e8b8 | glColor4f | 128 | decomp_liftd2_00.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispPosition, DebugDispPositionColor, DispWithSwordBlur +27 | - | globals: 01186390:& 01186420:&RW 01186424:&W 01186428:W 0118642c:W |  |
| 108e938 | glColor4fv | 144 | decomp_liftd2_00.cpp | bossDeathEffectBegin, disphalfarc, disphalfarcpillar, drawbar, gamecore_sub_01407640, manager_sub_01142350 | - | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4; globals: 01186390:& 01186420:&RW 01186424:&W 01186428:W 0118642c:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108e9c8 | glTexCoord2f | 40 | decomp_walk.cpp | DispWithSwordBlur, DispWithTorchBlur, gamecore_sub_01382610, gamecore_sub_01407640, manager_sub_010F3FB0 | - | globals: 01186390:& 01186430:W 01186434:W 01186438:W 0118643c:W | reached (13444668 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4x,d8:tb4y,d8:tbz) |
| 108e9f0 | glTexCoord2fv | 48 | recompiled | manager_sub_0113E218, manager_sub_01142350 | - | $a0: +0x0:R4 +0x4:R4; globals: 01186390:& 01186430:W 01186434:W 01186438:W 0118643c:W | D11: lifted, not reached at top level in the trace runs (earlier call counts: 1827588 calls, d9:dv1) |
| 108ea20 | glVertex2f | 144 | decomp_liftd3_00.cpp | bossDeathEffectBegin, drawbar, gamecore_sub_01407640, manager_sub_01100090 | manager_sub_0108D358, manager_sub_0108D730 | globals: 01186390:& 0118a580:R 01191804:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108eab0 | glVertex2fv | 120 | recompiled | - | manager_sub_0108D358, manager_sub_0108D730 | $a0: +0x0:R4 +0x4:R4; globals: 01186390:& 0118a580:R 01191808:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108eb28 | glVertex3f | 152 | decomp_liftd8_00.cpp | disphalfarcpillar, gamecore_sub_0136D4B0, gamecore_sub_013A5DE0, manager_sub_01107850, manager_sub_01107C18, manager_sub_01107EF0 +2 | manager_sub_0108D358, manager_sub_0108D730 | globals: 01186390:& 0118a580:R |  |
| 108ebc0 | glVertex3fv | 72 | decomp_walk.cpp | DebugDispCollisionFinder, DispWithSwordBlur, DispWithTorchBlur, disphalfarc, disphalfarcpillar, gamecore_sub_0133A048 +4 | manager_sub_0108D358, manager_sub_0108D730 | $a0: +0x0:R16; globals: 01186390:& 0118a580:R |  |
| 108ec08 | glVertex4f | 152 | recompiled | - | manager_sub_0108D358, manager_sub_0108D730 | globals: 01186390:& 0118a580:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108eca0 | glVertex4fv | 72 | recompiled | DebugDispCollisionDBEdge, DebugDispCollisionEdge, gamecore_sub_01382610, manager_sub_0113E218, manager_sub_01142350 | manager_sub_0108D358, manager_sub_0108D730 | $a0: +0x0:R16; globals: 01186390:& 0118a580:R | D11: lifted, not reached at top level in the trace runs (earlier call counts: 1827588 calls, d9:dv1) |
| 108ece8 | glGetColor | 88 | recompiled | - | - | $a0: +0x0:R4/W16/W4 +0x4:R4/W4 +0x8:R4/W4 +0xc:R4/W4; globals: 01186420:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108ed40 | glGetZTest | 16 | recompiled | gamecore_sub_0133B450 | - | globals: 011863c4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108ed50 | glLoadIdentity | 40 | recompiled | SCRFuncHorseBossCircumferenceMain, SCRFuncHorseEscapeFromBossMain, SCRFuncSetAttainForParallelTrans, SCRFuncSetAttainForPreParallelTrans | iosUnitMatrix | globals: 01214ee0:& 012156e0:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108ed78 | glLoadMatrixf | 48 | decomp_walk.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DebugDispVolumeShapeColor, DispWithSwordBlur, DispWithTorchBlur +13 | iosCopyMatrix | globals: 01214ee0:& 012156e0:R |  |
| 108eda8 | glGetMatrixf | 48 | recompiled | - | iosCopyMatrix | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108edd8 | glPushMatrix | 96 | recompiled | - | iosCopyMatrix | globals: 01214ea0:& 01214ee0:& 012156e0:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108ee38 | glPopMatrix | 32 | recompiled | - | - | globals: 012156e0:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108ee58 | glTranslatef | 112 | recompiled | - | - | globals: 01214ee0:& 01214f10:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108eec8 | glScalef | 232 | recompiled | - | memset | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108efb0 | glRotatex | 336 | recompiled | - | iosGetSinCosf, memset | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f100 | glRotatey | 344 | recompiled | - | iosGetSinCosf, memset | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f258 | glRotatez | 344 | recompiled | - | iosGetSinCosf, memset | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f3b0 | glZOffset | 16 | decomp_walk.cpp | DebugDispCollisionDBEdge, DebugDispCollisionEdge, DebugDispCollisionFinder, DispWithSwordBlur, DispWithTorchBlur, gamecore_sub_0133F998 +2 | - | globals: 0118a584:W |  |
| 108f3c0 | glSetReg | 48 | decomp_liftd4_00.cpp | gamecore_sub_01382610, manager_sub_01142080, manager_sub_01142148, manager_sub_01142230 | - | globals: 01186390:& 01186394:&RW |  |
| 108f3f0 | glGetCurrentMatrix | 48 | recompiled | - | iosCopyMatrix | globals: 01214ee0:& 012156e0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f420 | glLightVector | 56 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; globals: 01186390:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f458 | glSetLightVector | 32 | decomp_hot43b.cpp | CalcLightLayoutObj | iosCopyMatrix | globals: 011864c0:& |  |
| 108f478 | glSetLightColor | 32 | decomp_hot43b.cpp | CalcLightLayoutObj | iosCopyMatrix | globals: 01186500:& | reached (13325624 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4) |
| 108f498 | gluSetWrapMode | 128 | decomp_liftd8_00.cpp | manager_sub_01142080, manager_sub_01142230 | glBeginWithDlLayerNo, glEnd, iosDlGetLayer | globals: 01186390:& 01186394:&RW |  |
| 108f518 | iosgluGetCameraContext | 24 | decomp_liftd7_09.cpp | AI_OptFloatFrameSample, ClipLayoutObjModel, DEBUG_update_direction, ViewIntervalClip, cameraBlur, clipBySphere +49 | get_cur_camera_context | - |  |
| 108f530 | glSetFogFactor | 216 | sotc_worker.cpp | manager_sub_011425F0 | getFogFactor0, getFogFactor1 | globals: 01186390:&R 011863f0:W |  |
| 108f608 | glSetFogFactorDirect | 16 | decomp_liftd8_00.cpp | manager_sub_011425F0 | - | globals: 011863f0:W |  |
| 108f618 | gluGetLookAtPosition | 96 | decomp_liftd3_00.cpp | NewCreateFogColClass, NewCreateGameLightClass, cameraBlur, cameraBlurClearIntensityArray | get_cur_camera_context | $a0: +0x0:W16 |  |
| 108f678 | iosgluGetViewCoordPosition | 160 | decomp_liftd2_00.cpp | DispWithSwordBlur, DispWithTorchBlur, iosInitKPrintfContext3D, manager_sub_010F3FB0 | - | $a0: +0x0:R4/W16/W4 +0x4:R4/W4; $a1: +0x0:R16; globals: 01186390:R 01214ee0:& 012156e0:R |  |
| 108f718 | gluGetCameraSpaceXYZ | 160 | recompiled | - | get_cur_camera_context | $a0: +0x0:R16/W16; $a2: +0x0:R16/W16; globals: 001e0fb0:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f7b8 | gluUpdateCameraContext | 40 | decomp_liftd3_00.cpp | doit_cameraman_manager | get_cur_camera_context | globals: 01186390:W |  |
| 108f7e0 | gluGetBrightness | 72 | decomp_liftd2_00.cpp | manager_sub_0113E940, manager_sub_0113EC40 | - | $a0: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 108f838 | glGetZWrite | 16 | recompiled | - | - | globals: 011863c8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f848 | glGetLineSmooth | 16 | recompiled | - | - | globals: 011863b0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108f8b0 | glResetWithLayerNo | 40 | recompiled | - | iosDlChainTailCurrent, iosDlPopLayer, iosDlPushLayer | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108faf8 | LockFullScreenEffect | 16 | recompiled | - | - | globals: 0118b8c8:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108fb08 | UnlockFullScreenEffect | 16 | recompiled | - | - | globals: 0118b8c8:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108fb18 | graphSetFogParam | 144 | decomp_liftd4_00.cpp | foginterp, setFogImmediate | iosDlGetWorkBufId | globals: 0118b8d0:& 0118b8dc:W 0118b8e0:W 0118b8e4:W 0118b8e8:W 0118b8ec:W 0118b8f0:W |  |
| 108fba8 | getMBlur | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118b740:& |  |
| 108fbb8 | setMBlur | 16 | recompiled | - | - | globals: 0118b740:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 108fe40 | getFullScreenEffect | 16 | recompiled | - | - | globals: 0118b748:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1090d28 | kPrintfAsSprite | 384 | recompiled | - | getDrawScreenHeight, getDrawScreenWidth, getSpriteDispFlag, iosDlGetLayer, manager_sub_010905A0 +1 | globals: 001c9550:& 0118b900:&R 012156e8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1090ff8 | kprintfContext | 184 | recompiled | DEBUG_disp_collision_attribute_info, DEBUG_disp_collision_attribute_info_extend, DEBUG_disp_collision_name, DEBUG_disp_object_info, DebugDispItem, disp_camera_lws_animation_debug_info +7 | iosDlPopLayer, iosDlPushLayer, manager_sub_010905A0, vsprintf | $a0: +0x38:R4; globals: 0118b900:R 012156e8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10910b0 | kprintfContextCentering | 352 | recompiled | DEBUG_disp_center_collision_attribute_info, DebugDispCollisionDBEdgeAngle, DebugPrintf3D, DebugPrintf3D_Context, dispgpinfo, gamecore_sub_0133BB48 +4 | iosDlPopLayer, iosDlPushLayer, iosGetFontInfo, manager_sub_010905A0, vsprintf | $a0: +0x0:W4 +0x8:R4 +0x38:R4; globals: 001c9550:& 0118b900:&R 0118b904:R 012156e8:&R 012156e9:&R 012156ea:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091210 | kprintfContextSJIS | 184 | recompiled | - | iosDlPopLayer, iosDlPushLayer, manager_sub_010905A0, vsprintf | $a0: +0x38:R4; globals: 0118b900:R 012156e8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10916f8 | kSetTex | 16 | decomp_liftd7_01.cpp | loadCdNfbFile | - | globals: 0118ba50:W |  |
| 1091708 | kColor | 32 | recompiled | GAMEUTIL_disp_col_plane_info_debug, manager_sub_0107B2C8, manager_sub_01150D20, windowPrintf, windowPrintfBackBoard, windowPrintfScale | - | globals: 0118b900:& 0118b990:W 0118b994:W 0118b998:W 0118b99c:W 0118b9a0:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091728 | iosInitKPrintfContext | 176 | decomp_liftd3_00.cpp | disp_camera_lws_animation_debug_info, gamecore_sub_0133AD68, gamecore_sub_0133AFA0, gamecore_sub_0133EDA8, gamecore_sub_0133F0E0, gamecore_sub_013402E0 +2 | getDrawScreenHeight, getDrawScreenWidth, iosDlGetLayer | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 + |  |
| 10917d8 | iosInitKPrintfContext3D | 312 | recompiled | DebugDispCollisionDBEdgeAngle, DebugPrintf3D, DebugPrintf3D_InitContext, dispgpinfo, gamecore_sub_0133BB48, gamecore_sub_0133D0D0 +3 | getDrawScreenHeight, getDrawScreenWidth, glLoadMatrixf, iosDlGetLayer, iosgluGetViewCoordPosition | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 + | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091910 | iosSetKPrintfColorContext | 24 | recompiled | DebugDispItem, gamecore_sub_0133D0D0, gamecore_sub_0133EDA8, gamecore_sub_0133F0E0 | - | $a0: +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091928 | iosSetKPrintfLocateContext | 24 | recompiled | DEBUG_disp_collision_attribute_info, DEBUG_disp_collision_attribute_info_extend, DEBUG_disp_collision_name, DEBUG_disp_object_info, gamecore_sub_0133AD68, gamecore_sub_0133F0E0 +1 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091940 | iosSetKPrintfLocateContextRelativePosition | 56 | recompiled | gamecore_sub_0133D0D0 | - | $a0: +0x0:R4/W4 +0x4:R4/W4 +0x8:R4/W4 +0xc:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091978 | iosSetKPrintfDynamicsContext | 8 | recompiled | - | - | $a0: +0x34:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091980 | iosSetKPrintfDlContext | 8 | recompiled | - | - | $a0: +0x38:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091988 | iosSetKPrintfScaleContext | 8 | recompiled | - | - | $a0: +0x3c:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1091b58 | setLightParam | 336 | decomp_liftd7_01.cpp | gamecore_sub_01367890 | manager_sub_010919A0 | $a0: +0x34:R4 +0x38:R4 +0x3c:R4 +0x40:R4 +0x44:R4 +0x48:R4 +0x4c:R4 +0x50:R4 +0x54:R4 +0x58:R4 +0x5c:R4 +0x60: |  |
| 1091ca8 | setLight | 392 | decomp_liftd3_00.cpp | glReset, modelInitDispModelClass | iosMakeLightColorMatrix, iosMakeNormalLightMatrix | globals: 01215af0:&R 01215b00:&R 01215b10:&R 01215b20:&R 01215b30:&R 01215b40:&R 01215b50:&R 01215b60:&R 01215 |  |
| 1091e30 | getAmbientColorC | 1600 | decomp_liftd3_00.cpp | ExecGlobalLightMan, getAmbientColor | GetAmbientLightClass, GetAmbientLightObjGroup, floorf, isysGroupForExit, isysGroupForFirst +2 | $a0: +0x0:R16/W16/W4 +0x4:W4 +0x8:W4 +0xc:W4; globals: 0118bae0:&R 0118bae4:R 0118bae8:R 01191c08:& |  |
| 1092490 | getAmbientColor | 72 | recompiled | - | getAmbientColorC | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10924d8 | lightsw0 | 16 | recompiled | - | - | globals: 01215b60:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1092548 | lightsw1 | 16 | recompiled | - | - | globals: 01215b64:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10925b8 | lightsw2 | 16 | recompiled | - | - | globals: 01215b68:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1092628 | lightsw3 | 16 | recompiled | - | - | globals: 01215b6c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1092668 | setLightParamDefault | 24 | decomp_liftd6_01.cpp | st_reloadtask | manager_sub_010919A0 | - |  |
| 1092680 | rotLightVector | 272 | decomp_liftd7_01.cpp | manager_sub_010919A0 | iosApplyCurrentMatrix, iosRotCurrentMatrixX, iosRotCurrentMatrixY, iosRotCurrentMatrixZ, iosUnitCurrentMatrix | $a1: +0x0:R4 +0x4:R4 +0x8:R4; globals: 0118baa0:& 01215af0:& 01215b00:& 01215b10:& |  |
| 10927b0 | getLightVector | 56 | recompiled | - | - | $a0: +0x0:W16; globals: 01215af0:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10927e8 | debugLineObjectCreateAndSetAttributreIfChange | 1088 | recompiled | gamecore_sub_0133B450 | iosAllocPacketPtrPath1, iosDlChainTailCurrent | globals: 0118bb68:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1092f50 | lineObjectDispImmediate | 1384 | recompiled | - | _isysCreateObj, iosCopyMatrix, iosDlChainTailCurrent, iosDlLockObj, iosScaleMatrixV +6 | globals: 001e0c88:& 010965c8:& 01215bd8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095918 | debugLineObjectSetup | 40 | recompiled | gamecore_sub_0133B450 | transMicroCode | globals: 0118bb68:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095940 | debugLineObjectSetAttribute | 88 | recompiled | - | iosDlChainTailCurrent | globals: 0118bb68:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095998 | debugLineObjectCreateAttributre | 648 | recompiled | - | iosAllocPacketPtrPath1 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095c20 | debugLineObjectSetMatrix | 344 | recompiled | gamecore_sub_0133B450 | iosAllocPacketPtrPath1, iosDlChainTailCurrent, iosgluGetCameraContext | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; globals: 0118bb70:&R 0118bb78:R 0118bb80:R 0118bb88:R 0118bb90:R  | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095d78 | debugLineObjectSetBillboardMatrix | 424 | recompiled | - | iosAllocPacketPtrPath1, iosDlChainTailCurrent, iosScaleMatrixV, iosUnitRotation, iosgluGetCameraContext | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; globals: 0118bbb0:&R 0118bbb8:R 0118bbc0:R 0118bbc8:R 0118bbd0:R  | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095f20 | debugLineObjectDisp | 64 | recompiled | gamecore_sub_0133B450 | iosDlChainTailCurrent, iosDlGetWorkBufId | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1095f60 | lineObjectDispImmediatef | 720 | recompiled | - | iosCopyMatrix, iosDlChainTailCurrent, iosDlLockObj, iosScaleMatrixV, iosUnitRotation +4 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1096840 | modelCreatePacketTransPacket | 576 | decomp_liftd2_00.cpp | modelInitialize | iosMallocAlign, manager_sub_010965D0 | $a1: +0x14:W4 +0x50:R4 +0x54:R4 +0x60:R4 +0x64:R4 |  |
| 1096a80 | modelGetDlLayer | 264 | decomp_liftd2_00.cpp | modelDisp, modelGetDlLayerIndex, modelInitDispModelClass | - | $a0: +0x8:R4; $a1: +0x18:R8 |  |
| 1096b88 | modelInitDispModelClass | 984 | decomp_liftd2_00.cpp | manager_sub_010EF930 | getSpecularity, iosUnitMatrix, modelGetDlLayer, setLight | $a1: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x110:R4 +0x220:R4 +0x244:W4 +0x248:W4 +0x24c:W4 +0x250:R8/W4/W8 +0x254: |  |
| 1096f60 | modelReInitialize | 440 | recompiled | garbageNmoCallBack | manager_sub_010965D0 | $a1: +0x14:R4 +0x40:R4 +0x44:R4 +0x50:R4 +0x54:R4 +0x60:R4 +0x64:R4 | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 1097118 | modelDisp | 3696 | decomp_liftd9_90.cpp | manager_sub_010F3A08, manager_sub_010F3CF0 | getFogUnderWater, getNowBossEffect, gsReg, gsRegBegin, gsRegEnd +12 | $a0: +0x0:R4 +0xc:R4 +0x244:R4 +0x248:R4 +0x24c:R4 +0x250:R8 +0x258:R4 +0x260:R8; $a1: +0x0:R16 +0x10:R16 +0x2 |  |
| 1098450 | modelClipSkin | 1008 | recompiled | ClipLayoutObjModel | iosgluGetCameraContext, manager_sub_01097F88, manager_sub_01099C50 | $a0: +0x0:R4 +0x230:R4 +0x234:R4 +0x238:R4 +0x250:R4/R8; globals: 0118bfd4:R | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 1098840 | modelClipSolid | 1152 | decomp_walk.cpp | ClipLayoutObjModel | clipByMinMaxCube, iosgluGetCameraContext, manager_sub_01097F88, manager_sub_01099C50 | $a0: +0x0:R4 +0x230:R4 +0x234:R4 +0x238:R4 +0x250:R4/R8; $a1: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; globals: |  |
| 1098e20 | manager_sub_01098E20 | 1448 | sotc_native.cpp | modelSetEnviroment | getDrawScreenHeight, getDrawScreenWidth, getFogFactor0, getFogFactor1, getZFogNear +6 | globals: 0118bfcc:&R |  |
| 10993c8 | modelResetSurface | 24 | decomp_liftd2_00.cpp | gamecore_sub_0136D4B0, gamecore_sub_01382FC8, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113ACB8 | - | globals: 0118bfc8:R |  |
| 10993e0 | modelResetLayer | 64 | recompiled | - | iosDlChainTail | globals: 0118bfcc:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099420 | modelNameByObj | 16 | recompiled | - | - | $a0: +0x20:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099430 | modelNameByHeader | 8 | recompiled | - | - | $a0: +0x70:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099438 | modelSetMatrixTransTag | 296 | decomp_liftd6_00.cpp | manager_sub_010EF930 | - | $a1: +0x0:W8 +0x8:W4 +0xc:W4 |  |
| 1099560 | modelGetTextureObjByIndex | 24 | recompiled | - | - | $a0: +0x40:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099578 | modelGetTextureResourceByIndex | 24 | recompiled | - | - | $a0: +0x40:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099590 | modelDeleteDispModelClass | 64 | decomp_liftd6_00.cpp | FlushLayoutObjImmediate | - | $a0: +0x0:R4 |  |
| 10995d0 | modelGetLinkCount | 24 | recompiled | - | - | $a0: +0x18:R8 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10995e8 | modelIncLinkCount | 56 | recompiled | - | - | $a0: +0x18:R8/W8 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099620 | modelDecLinkCount | 56 | recompiled | - | - | $a0: +0x18:R8/W8 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099658 | modelGetWorkSize | 40 | decomp_liftd6_00.cpp | loadCdNmoFile, loadCdSeamlessStageFile | - | $a0: +0x54:R4 +0x64:R4 |  |
| 1099680 | modelInitialize | 208 | decomp_liftd6_00.cpp | gamecore_sub_0135D280, loadCdNmoFile | modelCreatePacketTransPacket | $a1: +0x8:W4 +0x18:R8/W8 +0x40:R4 +0x44:R4 +0x50:R4 +0x54:R4 |  |
| 1099750 | modelGetName | 8 | recompiled | GetLayoutClassObjectName | - | $a0: +0x70:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1099758 | SetDisplayModelNearClippingSwitch | 16 | decomp_liftd7_02.cpp | demoManBootRequest, demoManEnd | - | globals: 0118bfd4:W |  |
| 1099768 | modelSetSurface | 296 | recompiled | manager_sub_01137F78 | iosDlChainTailCurrent, iosDlGetLayer, texTransRequest, texTransRequestMulti | $a0: +0x14:R4 +0x40:R4 +0x50:R4; globals: 0118bfc8:R | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 10998b8 | modelSetEnviroment | 200 | decomp_walk.cpp | DispDispObjGroupModel | getSpecularity, manager_sub_01098CC0, manager_sub_01098E20 | $a1: +0x0:R4; globals: 0118bfc8:&R 0118bfcc:&R 0118bfd0:&R |  |
| 1099980 | modelInit | 128 | decomp_liftd3_01.cpp | graphInit | iosMallocAlign | globals: 001e0c88:& 0118bfc8:&W 0118bfcc:&W 0118bfd0:&RW |  |
| 1099a00 | ModelDispGetWorkSize | 8 | sotc_prof.cpp | manager_sub_010EF500 | - | - |  |
| 1099a08 | manager_sub_01099A08 | 584 | sotc_native.cpp | manager_sub_01099A08, modelDisp | manager_sub_01099A08 | $a1: +0x0:R16 +0x20:R4 +0x24:R4 +0x2c:R4/W4 |  |
| 109a170 | ptclDisplayListPut | 56 | decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0 +3 | iosGetPacketPtrPath1, iosSetPacketPtrPath1 | - |  |
| 109a1a8 | ptclDisplayListGet | 80 | decomp_hot46j.cpp | dischargeMain, dispVuColiDispWork, gamecore_sub_0138D480, gamecore_sub_0138D910, manager_sub_0113DBF8, manager_sub_0113DEA0 +3 | iosDlGetEndAdrs, iosGetPacketPtrPath1 | - |  |
| 109a4d8 | meshDispInstance | 152 | recompiled | - | iosDlChainTailCurrent, iosDlLockObj | $a0: +0xc0:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109a570 | meshCreate | 1240 | decomp_liftd6_00.cpp | gamecore_sub_01384590, manager_sub_011382E0 | _isysCreateObj, iosMallocAlign, iosSPrintf, iosUnitMatrix, isysSetObjImmediateDeleteFunc | globals: 0109acf8:& 0109ad00:& 01192028:& |  |
| 109aa68 | meshTransMatrix | 128 | recompiled | - | iosAllocPacketPtrPath1, iosCopyMatrix, iosDlChainTailCurrent | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109aae8 | meshTransLightMatrix | 152 | recompiled | - | iosAllocPacketPtrPath1, iosCopyMatrix, iosDlChainTailCurrent | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109ab80 | meshDisp | 72 | decomp_hot48b.cpp | gamecore_sub_01382FC8, manager_sub_01137F78 | iosDlChainTailCurrent, iosDlLockObj | - |  |
| 109abc8 | meshFlip | 32 | decomp_hot48b.cpp | gamecore_sub_01382FC8, manager_sub_01137F78 | - | $a0: +0x8:R1 |  |
| 109abe8 | meshInstanceFree | 24 | recompiled | - | iosFree | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109ac00 | meshCreateInstanceCopy | 200 | recompiled | - | iosMallocAlign, iosUnitMatrix | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109acc8 | meshGetWorkSize | 24 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109ace0 | meshObjectFree | 24 | decomp_liftd6_01.cpp | FlushClothWork, gamecore_sub_01384E08 | isysRequestDeleteObj | - |  |
| 109ad08 | CreateMeshObj | 80 | recompiled | - | _isysCreateObj, isysSetObjImmediateDeleteFunc | globals: 0109acf8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109be88 | getRefBlurLevel | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c000:& |  |
| 109d0e8 | getRefractBlurSpeed | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c038:& |  |
| 109d0f8 | getRefractBlurSpeedRand | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c03c:& |  |
| 109d108 | getRefractBlurLevel | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c040:& |  |
| 109d118 | getRefractBlurLevelRand | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c044:& |  |
| 109d158 | shadowReset | 456 | decomp_liftd4_00.cpp | graphFsEffectReset | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +7 | globals: 0118c07c:R 0118c08c:R |  |
| 109d320 | shadowDraw | 2904 | decomp_liftd3_00.cpp | graphFrameFinish | getDrawBufferAddressFBP, getDrawBufferAddressTBP, getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSize +12 | globals: 0118b750:&R 0118b754:R 0118b758:R 0118b75c:R 0118c060:& 0118c070:& 0118c080:&RW 0118c084:R 01192120:& |  |
| 109de78 | getShadowIntensity | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c084:& |  |
| 109de88 | getShadowDebug | 16 | recompiled | - | - | globals: 0118c088:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109de98 | setShadowIntensity | 16 | recompiled | - | - | globals: 0118c084:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109dea8 | GetShadowLength | 8 | decomp_liftd6_00.cpp | manager_sub_010EF930 | - | $a0: +0x18:R4 |  |
| 109deb0 | shadowGetWorkSize | 8 | sotc_prof.cpp | loadCdNsoFile | - | - |  |
| 109deb8 | shadowModelInit | 120 | decomp_liftd7_02.cpp | loadCdNsoFile | iosMallocAlign | $a1: +0x10:R4 +0x14:R4 +0x18:W4 +0x1c:W4 |  |
| 109df30 | shadowModelReInit | 40 | decomp_liftd7_02.cpp | garbageNsoCallBack | - | $a0: +0x10:R4 +0x14:R4 +0x1c:R4 |  |
| 109df58 | shadowReInit | 168 | recompiled | graphFrameBufferReInitialize | getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSize, getZBufferAddressFBP, iosSetDCParam | globals: 0118c08c:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 109e000 | shadowInit | 232 | decomp_liftd7_01.cpp | graphInit | getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSize, getZBufferAddressFBP, iosCreateDC +2 | globals: 001e0c88:& 0118c08c:&W |  |
| 109e0e8 | shadowDisp | 424 | decomp_liftd9_90.cpp | manager_sub_010F3A08, manager_sub_010F3CF0 | gsReg, gsRegBegin, gsRegEnd, iosDlChainTailCurrent, iosDlPopLayer +2 | $a1: +0x1c:R4; globals: 0118c080:R 0118c088:&R |  |
| 109e290 | getShadowDispFlag | 16 | recompiled | - | - | globals: 0118c07c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a1738 | getSolarFlareBlur | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c09c:& |  |
| 10a1748 | getSolarFlareColorR | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c0a8:& |  |
| 10a1758 | getSolarFlareColorG | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c0ac:& |  |
| 10a1768 | getSolarFlareColorB | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c0b0:& |  |
| 10a1788 | getSolarGlowLevel | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c0a4:& |  |
| 10a1d50 | spriteInitObject | 448 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, spriteCreateObject | - | $a0: +0x0:W4 +0x4:W1 +0x5:W1 +0x6:W1 +0x7:W1 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x2 |  |
| 10a24c0 | spriteDrawSpriteTexNoSet | 3576 | recompiled | - | gsReg, gsRegBegin, gsRegEnd, iosGetSinCosf, manager_sub_010A21F8 | $a0: +0x4:R1 +0x5:R1 +0x6:R1 +0x7:R1 +0x8:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4 +0x28:R4 +0 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a32b8 | spriteDrawSprite | 3688 | sotc_overrides.cpp | gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408910, gamecore_sub_01408B48 +8 | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +4 | $a0: +0x0:R4 +0x4:R1 +0x5:R1 +0x6:R1 +0x7:R1 +0x8:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4 +0x |  |
| 10a43c8 | setVertexPos | 552 | recompiled | spriteDrawSpriteST, spriteDrawSpriteUV | getDrawScreenHeight, getDrawScreenWidth | $a0: +0x0:R4/W4 +0x4:R4/W4 +0x8:R4/W4 +0xc:R4/W4; $a1: +0x30:R4 +0x34:R4 +0x40:R4 +0x44:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a45f0 | spriteDrawSpriteUV | 672 | recompiled | - | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +3 | $a0: +0x0:R4 +0x4:R1 +0x5:R1 +0x6:R1 +0x7:R1 +0x8:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a4890 | spriteDrawSpriteST | 680 | recompiled | - | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +3 | $a0: +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a4b38 | spriteDrawSpriteUV2 | 920 | sotc_overrides.cpp | - | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +4 | $a0: +0x0:R4 +0x4:R1 +0x5:R1 +0x6:R1 +0x7:R1 +0x8:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4; $a1: +0x0:R4 +0x4:R4 |  |
| 10a4ed0 | spriteDrawSpriteST2 | 928 | sotc_overrides.cpp | - | getDrawScreenHeight, getDrawScreenWidth, gsReg, gsRegBegin, gsRegEnd +4 | $a0: +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 |  |
| 10a5270 | spriteCreateObject | 72 | decomp_liftd8_00.cpp | gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48, gamecore_sub_01409DD0 +1 | iosMallocAlign, spriteInitObject | - |  |
| 10a52b8 | spriteDeleteObject | 24 | decomp_liftd7_02.cpp | gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48, gamecore_sub_01409DD0 | iosFree | - |  |
| 10a52d0 | spriteSetPosition | 16 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48 +7 | - | $a0: +0x30:W4 +0x34:W4 |  |
| 10a52e0 | spriteSetTextureCoordinateST | 48 | decomp_liftd2_00.cpp | feedbackBlur2, gamecore_sub_01408B48, gamecore_sub_01408F70, gamecore_sub_01409DD0, gamecore_sub_0140A2C0 | - | $a0: +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x48:R8/W8 |  |
| 10a5310 | spriteSetTextureCoordinateSTFromUV | 168 | decomp_liftd2_00.cpp | gamecore_sub_013F8B58, gamecore_sub_013F9BA8 | - | $a0: +0x0:R4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x48:R8/W8 |  |
| 10a53b8 | spriteSetTextureCoordinateUV | 88 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_013F8B58, gamecore_sub_013F9BA8, manager_sub_011104A8, manager_sub_011106E8, manager_sub_01110A40 +2 | - | $a0: +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x38:W4 +0x3c:W4 +0x48:R8/W8 |  |
| 10a5410 | spriteSetColor | 24 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_013F9BA8, gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8 +3 | - | $a0: +0x4:W1 +0x5:W1 +0x6:W1 +0x7:W1 |  |
| 10a5428 | spriteSetScale | 16 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48 +6 | - | $a0: +0x40:W4 +0x44:W4 |  |
| 10a5438 | spriteSetRotation | 8 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48 +2 | - | $a0: +0x48:W4 |  |
| 10a5440 | spriteSetBlendMode | 144 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_01407E88, gamecore_sub_01408258, gamecore_sub_01408450, gamecore_sub_014086A8, gamecore_sub_01408B48 +2 | - | $a0: +0x48:R8/W8 |  |
| 10a54d0 | spriteSetFade | 8 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, gamecore_sub_013F8B58, gamecore_sub_013F9BA8 | - | $a0: +0x8:W4 |  |
| 10a54d8 | spriteSetPriority | 8 | decomp_hot48b.cpp | SpriteMenuSetSpriteObjectParam, manager_sub_01113D18 | - | $a0: +0x4c:W1 |  |
| 10a54e0 | getSpriteDispFlag | 16 | recompiled | kPrintfAsSprite | - | globals: 0118c150:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10a54f0 | texTransResolve | 14664 | decomp_hot47h.cpp | gamecore_sub_01345F60 | iosAllocPacketPtrPath1, isysObjSignalSema, isysObjWaitSema, manager_sub_010AB618, vramAlloc +2 | globals: 0118b8d0:&R 0118b8d4:R 0118c180:R 0118c190:&R 01192488:&R 0119248f:R 01215e44:R 01215e4c:R |  |
| 10a90d8 | texClutScrollOne | 1312 | recompiled | gamecore_sub_01387BC8 | iosDlGetWorkBufId | $a1: +0x1c:R4; $a2: +0x40:R4/W4 +0x44:R4/W4 +0x48:R2/R4 +0x4c:R2/R4 +0x50:R4; $a3: +0x8:R2/W2 +0xa:R2/W2 +0xc: | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aa748 | texInit | 344 | decomp_liftd9_90.cpp | graphInit | iosMallocAlign, iosgluGetCameraContext, isysInitGroup, vramReset | globals: 001e0c88:& 0118c178:W 0118c180:W 0118c184:R 01215e40:& 01215e48:&W 01215e4c:&RW |  |
| 10aa8a0 | texScaleClutAlphaByName | 384 | recompiled | - | strcmp | globals: 0118c184:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aaa20 | texScaleClut | 432 | decomp_liftd4_04.cpp | texScaleClutByName, updateEyeWork | - | $a0: +0x20:R4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 |  |
| 10aabd0 | texName | 88 | recompiled | - | - | $a0: +0x20:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aac28 | texNameRes | 80 | recompiled | - | - | $a0: +0x18:R4 +0x1c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aac78 | texGetCSM1Idx | 24 | recompiled | SpriteInitStatusClut, SpriteSetStatusClut | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aac90 | texGetTexByteSize | 40 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aacb8 | texGetTexTBPSize | 56 | decomp_liftd2_00.cpp | vramAlloc | - | - |  |
| 10aacf0 | texGetSize | 88 | recompiled | gamecore_sub_0135F4A0, manager_sub_01152680, windowSprite | - | $a0: +0x20:R4; $a1: +0x0:W4; $a2: +0x0:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10aad48 | texTransRequest | 296 | decomp_isys48.cpp | dischargeMain, disp_particle_emitter, gamecore_sub_0134B490, gamecore_sub_0135F4A0, gamecore_sub_01382610, gamecore_sub_01382FC8 +16 | iosAllocPacketPtrPath1, iosDlChainTailCurrent, iosDlGetLayer, iosDlLockObj | $a0: +0x20:R4; globals: 01215e44:R 01215e48:&R |  |
| 10aae70 | texTransRequestMulti | 312 | sotc_overrides.cpp | modelDisp, modelSetSurface | iosAllocPacketPtrPath1, iosDlChainTailCurrent, iosDlGetLayer, iosDlLockObj | $a0: +0x20:R4; $a1: +0x20:R4; globals: 01215e44:R 01215e48:&R |  |
| 10aafa8 | texResetVRAMRequest | 224 | decomp_liftd5_50.cpp | bgBegin, bgDraw, bossDeathEffectBegin, bossDeathEffectEnd, reflectionBegin, reflectionEnd +2 | iosAllocPacketPtrPath1, iosDlChainTailCurrent, iosDlGetLayer | globals: 01215e44:R 01215e48:&R |  |
| 10ab088 | texGetTextureModuleGroup | 16 | recompiled | - | - | globals: 01215e40:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ab098 | texGetWorkSize | 96 | decomp_liftd4_00.cpp | loadCdNtoFile, loadCdWinTexFile | - | $a0: +0x1c:R4 |  |
| 10ab0f8 | texInitialize | 152 | decomp_liftd4_00.cpp | loadCdNtoFile, loadCdWinTexFile | isysAddGroupWithLinkParam, manager_sub_010A8E38 | $a0: +0x20:R4; globals: 0118c184:&R 01215e40:& |  |
| 10ab190 | texDelete | 112 | decomp_liftd6_00.cpp | unloadCdNtoFile | - | $a0: +0x20:R4; globals: 0118c184:&RW |  |
| 10ab200 | texClutReset | 160 | recompiled | gamecore_sub_01387BC8 | - | $a0: +0x20:R4; globals: 01192280:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ab2a0 | texReset | 264 | decomp_texframe.cpp | graphFrameInit | iosgluGetCameraContext, manager_sub_010A9EA8, vramReset | globals: 0118c180:W 0118c184:R 01215e44:&R 01215e48:&R 01215e4c:&R |  |
| 10ab3a8 | texSearchMopByName | 192 | decomp_liftd7_09.cpp | StaffRollStart, gamecore_sub_0137BEE8, gamecore_sub_01382610, gamecore_sub_01382FC8, gamecore_sub_01387BC8, gamecore_sub_0139E5C8 +8 | strcmp | globals: 0118c184:R |  |
| 10ab468 | texScaleClutAlpha | 192 | decomp_liftd8_00.cpp | SCRCharaInitBossWork, SCRExecBossPreProcess, gamecore_sub_0137BEE8 | - | $a0: +0x20:R4 |  |
| 10ab528 | texScaleClutByName | 240 | recompiled | - | strcmp, texScaleClut | globals: 0118c184:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ab660 | texNameResErrorCheck | 80 | recompiled | - | - | $a0: +0x18:R4 +0x1c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ab6b0 | getTexTransEnableFlag | 16 | recompiled | - | - | globals: 0118c44c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ab6c0 | texSetModelName | 16 | recompiled | - | - | globals: 0118c450:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10abd38 | vramAlloc | 152 | recompiled | texTransResolve | manager_sub_010AB848, texGetTexTBPSize | - | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 10abdd0 | vramCreateBlock | 248 | decomp_liftd6_00.cpp | graphFrameBufferReInitialize, graphInit | iosMallocAlign, manager_sub_010ABC48 | globals: 001e0c88:& 0118c458:&RW |  |
| 10abec8 | vramDeleteBlock | 24 | recompiled | graphFrameBufferReInitialize | iosFree | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10abee0 | vramFree | 64 | decomp_hot47h.cpp | texTransResolve | manager_sub_010AB9A0 | $a0: +0x8:R4 +0xc:R4 | reached (152769105 calls; d10:fv1,d10:fv2,d10:hang1,d10:ngo1,d10:ngo2,d10:ngp1,d10:ngp2,d10:ngv1,d10:ngv2,d10:sv1,d7:attract,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:ng5,d8:ng6,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4) |
| 10abf20 | vramReset | 160 | recompiled | texInit, texReset, texTransResolve | manager_sub_010AB848, manager_sub_010AB9A0 | globals: 0118c458:R | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 10abfc0 | vramEnd | 32 | recompiled | graphFrameBufferReInitialize | iosFree | globals: 0118c460:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10abfe0 | vramInit | 176 | decomp_liftd6_00.cpp | graphFrameBufferReInitialize, graphInit | iosMallocAlign, memprintf | globals: 001e0c88:& 0118c458:&W 0118c460:RW 0118c464:&RW 0118c468:RW 011931c0:& |  |
| 10ac090 | vramAllocByTBPSize | 24 | recompiled | - | manager_sub_010AB848 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ac0a8 | zFog | 856 | sotc_native.cpp | graphFrameFinish | bgDraw, getDrawBufferAddressFBP, getDrawScreenHeight, getDrawScreenWidth, gsReg +9 | globals: 0118c46c:R 0118c470:R 0118c474:R 0118c480:R |  |
| 10ac400 | getZFogDivide | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 0118c46c:& |  |
| 10ac410 | getZFogWidth | 16 | recompiled | - | - | globals: 0118c470:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ac420 | getZFogFarMargin | 16 | recompiled | - | - | globals: 0118c474:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ac430 | getZFogFar | 16 | recompiled | - | - | globals: 0118c480:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ac440 | getZFogScissorMargin | 16 | decomp_liftd4_00.cpp | manager_sub_01098E20 | - | globals: 0118c478:& |  |
| 10ac450 | getBgFogOffset | 16 | recompiled | - | - | globals: 0118c47c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ac460 | setZFogFar | 16 | decomp_liftd6_01.cpp | gamecore_sub_0135A680, gamecore_sub_0135BDF0 | - | globals: 0118c480:W |  |
| 10ac470 | zFogInit | 8 | sotc_prof.cpp | graphInit | - | - |  |
| 10ac478 | getZFogNear | 40 | decomp_liftd2_00.cpp | manager_sub_01098E20 | - | globals: 0118c470:R 0118c474:R 0118c480:R |  |
| 10c2ea0 | dispCollisionFinderFlag | 16 | recompiled | - | - | globals: 0118c4f0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10c2eb0 | dispCollisionKeepAreaFlag | 16 | recompiled | - | - | globals: 0118c4ec:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10cc6f8 | getLocalAccelDispFlag | 16 | recompiled | - | - | globals: 0118c9e4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10cc708 | getScriptSwitchDispSw | 16 | recompiled | - | - | globals: 0118c9e8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10dc000 | charrootdyn_clipNoStopCollisionFinder | 32 | decomp_liftd7_02.cpp | manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxzfit | ClipCollisionFinderCallBack | globals: 010df8f0:& |  |
| 10dd318 | charrootdyn_findGlobalYGeometryPoint | 56 | recompiled | - | ClipCollisionFinder | $a2: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ddb60 | charrootdyn_getLocalYGlobalXZOffsetFromParentGP | 456 | recompiled | manager_sub_010E8560 | GetGeometryPointerPosition | $a0: +0x0:R16/W16/W4 +0x8:W4 +0xc:W4; $a1: +0x0:W16 +0x4:W4; $a3: +0x0:R16 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 13 calls, d9:dv1) |
| 10de280 | charrootdyn_getOffsetGlobalPos | 192 | decomp_liftd10_00.cpp | manager_sub_010E9A50, manager_sub_010EA718 | - | $a0: +0x0:W16; $a1: +0x13c:R4; globals: 0118ca20:&RW 0118ca28:W |  |
| 10e0510 | charrootdyn_debugDispParentGeometryStatus | 72 | recompiled | - | GetCollisionDBLastPointOnCurrentPlane, GetGeometryPointerSurfacePlane | $a0: +0x138:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10e2170 | resetExpressionLocalHeightDiff | 80 | decomp_liftd6_00.cpp | InitCharacterObj | - | $a0: +0x33c:R4 +0x358:R4 |  |
| 10ec2e8 | ExecFogColMan | 1240 | recompiled | gamecore_sub_01345390 | GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix, GetRateInOutEllipticalSphereVolumeWithInverseMatrix, _isysDeleteGroup, foginterp +18 | globals: 001b7b60:& 001e0ed4:R 01215e80:& 01215e84:& 01215ece:R 01215ecf:R 01215ed0:R 01215ed8:R 01215edc:R 01 | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 10ec7c0 | NewCreateFogColClass | 520 | recompiled | - | _isysDeleteGroup, gluGetLookAtPosition, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst | $a0: +0x90:R4/W4; globals: 001e0f40:&R 01193990:&R 01193998:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ecfe0 | InitFogColMan | 112 | decomp_liftd6_01.cpp | gamecore_sub_01345390 | isysInitGroup, memset | globals: 01215e80:& 01215f18:& 01215f1c:& 01215f20:& 0121611c:& 01216120:&W |  |
| 10ed280 | CreateFogColMan | 216 | decomp_liftd6_01.cpp | gamecore_sub_01345390 | _isysCreateObj, isysAddGroupWithLinkParam, memset | globals: 010ed5d8:& 01215e80:& 01215f18:& |  |
| 10ed358 | SetInitFogColClassFromStageData | 80 | decomp_liftd7_03.cpp | gamecore_sub_01367778 | - | $a0: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x18:R4 +0x1c:R1 +0x20:R1 +0x24:R1; globals: 01215e80:& 01215ece:W 0121 |  |
| 10ed440 | UpdateFogCol | 224 | decomp_hot46j.cpp | manager_sub_01108960 | - | $a0: +0x4e:W1 +0x4f:W1 +0x50:W1 +0x58:W4 +0x5c:W4 +0x60:W4 +0x64:W4 +0x68:W4 +0x6c:W4; $a1: +0x0:R4 +0x4:R4 +0 |  |
| 10ed520 | SetFogColAvailableFlag | 8 | decomp_liftd6_00.cpp | SetScriptAnimationObjAccessSwitch, manager_sub_011034E8, manager_sub_01103908 | - | $a0: +0x80:W4 |  |
| 10ed528 | IsEnvelopeFogType | 16 | decomp_hot46j.cpp | manager_sub_01108960 | - | $a0: +0x40:R4 |  |
| 10ed538 | GetFogColMan | 16 | recompiled | - | - | globals: 01215e80:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ed548 | GetFogColClass | 32 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ed568 | ForceFogColFlagEnvelope | 24 | recompiled | manager_sub_01105938 | - | $a0: +0x40:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10ed688 | GetGeoDispClass | 32 | decomp_walk.cpp | DispDispObjGroupModel, auto_end_check_and_check_free_particle_geodisp, calc_light_particle_geodisp, check_free_particle_geodisp, clip_particle_geodisp, create_and_boot_particle_geodisp +4 | - | $a0: +0x8:R1 |  |
| 10ed6a8 | CreateGeoDispObj | 208 | sotc_worker.cpp | initParticleObject | GetLayoutDispObjGroup, InitSortObj, _isysCreateObj, isysAddGroupWithLinkParam, isysSetObjImmediateDeleteFunc | $a2: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4; globals: 010ed778:& 010ed780:& |  |
| 10ee128 | ClipJudgeVolumeInfo | 752 | recompiled | GetVolumeClippedTargetPos, gamecore_sub_01373520 | CheckPositionInsideOfJudgeVolumeInfo, iosAbsVector, manager_sub_010EDAE8, manager_sub_010EDDE8 | $a0: +0x0:R16/W16; $a1: +0x0:R16 +0x50:R4; $a2: +0x0:R16; globals: 001e0f90:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10eed58 | UpdateLayoutClassSimpleGeometry | 1432 | decomp_liftd7_08.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, DispDispObjGroupModel, ExecObjectAnimation, SCRSetLWSGeometry, SCRSetLWSGeometryWithNoParentChange +23 | iosGetScaleInverseVector, iosGetScaleMatrixV, iosScaleMatrixV | $a0: +0x30:W16 +0x134:R4 +0x138:R4 +0x13c:R4 +0x154:R4 +0x158:R4 +0x15c:R4 +0x164:R4 +0x168:R4 +0x16c:R4 +0x17 |  |
| 10ef2f0 | UnparentLayoutObj | 528 | recompiled | - | DeleteTwoWayLink, GetGeometryPointerObject, GetGeometryPointerQuatPos, GetLayoutClass, SetGeometryPointerGlobalState | $a1: +0x0:R16; globals: 001e0f40:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f0b28 | InitAliasLayoutObj | 736 | decomp_liftd6_00.cpp | CreateAliasLayoutObj, CreateAliasScriptLayoutObj, InitAliasScriptLayoutObj | CreateClothChildLayoutObj, GetLayoutDispObjGroup, GetLayoutObjGroup, GetLightClass, GetScriptLayoutClass +4 | $a0: +0x8:R1; $a1: +0xc:R4 +0x28:R4; $a2: +0x8:R1; globals: 001e0fd0:&R 0118caf0:&RW |  |
| 10f0e08 | FlushLayoutObj | 272 | decomp_liftd6_00.cpp | DeleteScriptLwsOrientObjSubObjects, FlushCharacterObj, FlushScriptLayoutObj, manager_sub_010F37B0, manager_sub_010F9480, manager_sub_011031F8 +2 | DeleteTwoWayLink, GetLayoutClass, GetLightClass, GetScriptLayoutClass, ReleaseCollisionWork +3 | $a0: +0x8:R1; globals: 001e0f40:& 001e0fd0:& |  |
| 10f0f18 | FlushLayoutObjImmediate | 504 | decomp_liftd6_00.cpp | FlushCharacterObjImmediate, FlushScriptLayoutObjImmediate, manager_sub_010F3798, manager_sub_0110D2A0, manager_sub_0110D380 | DeleteAllTwoWayLink, FlushClothWork, FlushCollisionWorkImmediate, GetLayoutDispObjGroup, GetLightClass +6 | $a0: +0x8:R1; globals: 010f3668:& |  |
| 10f1110 | GetLayoutObjChildWithName | 368 | recompiled | gamecore_sub_013557D8 | GetLightClass, GetScriptLayoutClass, GetTwoWayLinkData, GetTwoWayLinkNext, dynamic_cast_to_camera_obj +2 | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f1280 | SaveLayoutObjGroupAllGeo | 288 | decomp_liftd8_00.cpp | gStatCheckPointSave, gStatChekcPointSaveHookPlayerHorsePos | GetLayoutObjGroup, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class +3 | - |  |
| 10f13a0 | SetLayoutObjAccessSwitch | 312 | decomp_hot46i.cpp | SCRFuncSetDispFlagGameObj, SCRPlayerPickUp, SCRSetDispFlag, SetClothAccessSwitch, SetClothAccessSwitch_SystemSetting, SetClothWorkDispFlag +15 | GetLightClass, GetScriptLayoutClass, GetTwoWayLinkData, GetTwoWayLinkNext, SetClothAccessSwitch +4 | $a0: +0x8:R1 |  |
| 10f1630 | CreateLayoutObj | 384 | decomp_liftd6_00.cpp | gamecore_sub_0135D280, initLayoutObj, initSeamlessStageObj | CreateClothChildLayoutObj, GetClothWorkSize, GetLayoutDispObjGroup, GetLayoutObjGroup, GetLightClass +8 | $a2: +0xc:R4; globals: 010f3798:& 010f37b0:& |  |
| 10f17b0 | CreateLayoutObjVU0Secure | 400 | decomp_liftd6_00.cpp | initLayoutObjVU0Secure, initSlowObject | CreateClothChildLayoutObj, GetClothWorkSize, GetLayoutDispObjGroup, GetLayoutObjGroup, GetLightClass +10 | $a2: +0xc:R4; globals: 010f3798:& 010f37b0:& |  |
| 10f1940 | GetLayoutObjNum | 112 | recompiled | - | GetLayoutObjGroup, isysGroupForExit, isysGroupForFirst, isysGroupForNext | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f19b0 | GetLayoutDispObjNum | 112 | recompiled | - | GetLayoutDispObjGroup, isysGroupForExit, isysGroupForFirst, isysGroupForNext | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f1a20 | GetLayoutClass | 144 | decomp_liftd7_08.cpp | AI_CheckHorseParent, AI_CheckPathPoint, AI_CheckSightOn, AI_OptFloatFrameSample, AI_PlayerParentFlag, AI_UpdateChildPathWork +665 | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f1ab0 | SetLayoutUserClipFunc | 160 | recompiled | - | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f1b50 | InitLayoutObj | 272 | decomp_liftd6_00.cpp | CreateLightObj, CreateScriptLayoutObj, InitCharacterObj, InitLightObj, InitScriptLayoutObj, create_camera_obj | CreateClothChildLayoutObj, GetLayoutDispObjGroup, GetLayoutObjGroup, GetLightClass, GetScriptLayoutClass +4 | $a1: +0xc:R4 |  |
| 10f1c60 | CreateAliasLayoutObj | 176 | recompiled | - | GetClothWorkSize, InitAliasLayoutObj, _isysCreateObj, isysSetObjImmediateDeleteFunc | $a2: +0xc:R4; globals: 010f3798:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f1d10 | GetLayoutObjWorkSize | 80 | decomp_liftd4_01.cpp | CreateAliasScriptLayoutObj, CreateLightObj, CreateScriptLayoutObj, GetCharacterObjWorkSize, GetLwsOrientWorkSize, GetScriptLayoutObjWorkSize +2 | GetClothWorkSize, manager_sub_010EF500 | $a0: +0xc:R4 |  |
| 10f1e28 | SetLayoutObjSubDispFunc | 184 | decomp_liftd7_03.cpp | SCRSetupArrow1ByPlayer2, nico_playerstart | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f1ee0 | ParentLayoutObj | 224 | decomp_liftd7_08.cpp | ReparentCharacterObj, SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry, charrootdyn_parentHColGp, charrootdyn_resetRootDynamicsWithLowerCollisionForXZ, charrootdyn_resetRootDynamicsWithLowerCollisionForXZFit +26 | AddTwoWayLinkNext, CopyGeometryPointer, DeleteTwoWayLink, DivQuaternion, GetGeometryPointerObject +2 | - |  |
| 10f1fc0 | ForceUnparentLayoutObj | 96 | decomp_liftd6_00.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, GetCharaRootDynamics, ReparentCharacterObj, ResetObject, SCRJumpNearestDeadGeometry +27 | DeleteTwoWayLink, GetLayoutClass, SetGeometryPointerGlobalState | globals: 001e0f40:& 001e0fd0:& |  |
| 10f2020 | ForceDeleteLayoutObjChildLink | 160 | decomp_liftd5_50.cpp | manager_sub_01104BA0 | DeleteAllTwoWayLink, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1; globals: 010f3780:& |  |
| 10f20c0 | ForceParentLayoutObj | 184 | decomp_hot46j.cpp | RecvCharacterObjMail, SCRSetLWSGeometry, SCRSetLWSGeometryWithZY, SendCharaNodeParentSetMsg, manager_sub_01104BA0, manager_sub_01108960 | AddTwoWayLinkNext, CopyGeometryPointer, DeleteTwoWayLink, GetGeometryPointerObject, GetLayoutClass | - |  |
| 10f2178 | AddLayoutObjSubLink | 64 | decomp_liftd5_50.cpp | CreateSubLinkObjByGeometryPointer | AddTwoWayLinkNext, GetLayoutClass, GetSubLinkClass | - |  |
| 10f21b8 | ForceUnparentLayoutObjChildLink | 160 | recompiled | - | DeleteAllTwoWayLink, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1; globals: 010f3670:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f22b8 | UpdateLayoutClassParentGeometry | 472 | decomp_liftd7_08.cpp | charrootdyn_updateCharacterClassParentGeometry, manager_sub_01103AE8, manager_sub_01108960, motrootdyn_parent_from_falldown, nico_clothdrive, nico_minotaur_weaponstart +4 | GetGeometryPointerObject, GetGeometryPointerQuatPos, GetLayoutClass | $a0: +0x124:R4 +0x134:R4 +0x138:R4 +0x13c:R4 +0x19c:R4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 10f2490 | GetLayoutClassObjectName | 72 | recompiled | gamecore_sub_0133D0D0 | modelGetName | $a0: +0x144:R4; globals: 01190000:R 01193a20:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f24d8 | UpdateLayoutClassPastSimpleGeometry | 88 | decomp_liftd7_08.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, SetObjectGeo, SetObjectGeoAndUnparent, doit_game_camera_model, gamecore_sub_0133A398 +13 | CopyMultiMatrix | $a0: +0xa0:R16 +0xb0:W16 +0x134:R4 +0x138:R4 +0x164:R4 +0x168:R4 +0x1b4:R2 +0x1bc:R1 |  |
| 10f2530 | UpdateLayoutClassParentSimpleGeometry | 416 | recompiled | SCRSetLWSGeometry, SCRSetLWSGeometryWithNoParentChange, SCRSetLWSGeometryWithZY, nico_pendulumtest | GetGeometryPointerObject, GetGeometryPointerQuatPos, GetLayoutClass | $a0: +0x124:R4 +0x134:R4 +0x138:R4 +0x13c:R4 +0x19c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f26d0 | UpdateLayoutClassGeometry | 1152 | decomp_layout.cpp | ExecCharaGeometry, InitCharacterObj, ResetCharacterObj4DDynamicsInfo, UpdateCharacterClassGeometry, gamecore_sub_0136DA78, gamecore_sub_0136E618 +5 | iosGetScaleInverseVector, iosGetScaleMatrixV, iosScaleMatrixV | $a0: +0x158:R4 +0x15c:R4 +0x164:R4 +0x168:R4 +0x16c:R4 +0x170:R4 +0x1b4:R2 +0x1bc:R1 |  |
| 10f2b50 | UpdateLayoutClassPastGeometry | 88 | decomp_charageo.cpp | ExecCharaGeometry, InitCharacterObj, ResetCharacterObj4DDynamicsInfo, UpdateCharacterClassPastGeometry, gamecore_sub_0136DA78, gamecore_sub_0136E5F8 +3 | CopyMultiMatrix | $a0: +0xa0:R16 +0xb0:W16 +0x134:R4 +0x138:R4 +0x164:R4 +0x168:R4 +0x1b4:R2 +0x1bc:R1 |  |
| 10f2ba8 | SetLayoutObjAccessSwitchThisObjOnly | 208 | decomp_liftd6_00.cpp | SetScriptAnimationObjAccessSwitch, gamecore_sub_0136B710, gamecore_sub_0136D940, gamecore_sub_0136DA78, gamecore_sub_0136E6F0, manager_sub_011034E8 +1 | GetLightClass, GetScriptLayoutClass, SetClothAccessSwitch, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f2c78 | GetLayoutClassWindGeometry | 312 | decomp_hot46i.cpp | ExecCharaGeometry, ExecObjectAnimation, GetCharaRootDynamics, SetCharacterClassWindDynamics, getClothGeometry, nico_clothdrive | GetWindVector | $a0: +0x134:R4 +0x19c:R4; $a1: +0x0:R16 |  |
| 10f2db0 | SetLayoutClassScaleElem | 144 | decomp_hot46i.cpp | dischargeMain, gamecore_sub_0137C028, manager_sub_01105938, manager_sub_01108960, nico_parentAndScaleSync, updateLayoutGeo +1 | iosGetScaleInverseVector | $a0: +0x90:R4/W16 +0x94:R4 +0x98:R4 +0x9c:W4 +0xa0:W16; $a1: +0x0:R16; globals: 001e0fd0:&R |  |
| 10f2e40 | CheckLayoutObjChainExist | 168 | decomp_liftd6_00.cpp | initAliasLayoutObj, initLayoutObj, initLayoutObjVU0Secure, initScriptCharObj, initScriptLayoutObj | CheckClothChainExist, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f2ee8 | SetLayoutObjDebugResetFunc | 152 | decomp_liftd7_03.cpp | nico_playerstart | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f2f80 | CallLayoutObjDebugResetFunc | 176 | decomp_liftd7_03.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, ResetObject, gamecore_sub_0133A048, gamecore_sub_0133A398, gamecore_sub_0133EF68 | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f3030 | GetLayoutClassCollisionBoundaryInfo | 80 | recompiled | - | - | $a0: +0x0:W16 +0xc:W4; $a1: +0x90:R4 +0x134:R4 +0x184:R4; globals: 001e0f90:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f3080 | SetLayoutObjCollisionEnableFlag | 160 | decomp_liftd6_00.cpp | ChangeCharacterObjCollisionEnableFlag, InitCharacterObj, initScriptCharObj | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f3120 | GetLayoutObjCollisionEnableFlag | 152 | decomp_liftd6_00.cpp | initScriptCharObj | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 10f31b8 | SetLayoutClassCollisionWorkAdr | 8 | decomp_liftd7_03.cpp | RelocateCollisionWork | - | $a0: +0x174:W4 |  |
| 10f31c0 | ChangeLayoutObjCollisionResourceAttribute | 288 | recompiled | ChangeCharacterObjCollisionResourceAttribute | ChangeCollisionResourceAttribute, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f32e0 | SaveLayoutObjGeo | 184 | recompiled | - | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f3398 | GetLayoutRootMatrix | 136 | recompiled | - | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16; $a1: +0x134:R4 +0x13c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f3420 | GetLayoutParentObj | 48 | decomp_liftd3_00.cpp | CollisionObjectJudgeCallbackExceptDescendOneObj, CollisionObjectJudgeCallbackExceptDescendPluralObj, DEBUG_disp_collision_attribute_info_extend, DEBUG_disp_collision_name, DEBUG_get_parent_layout_name, GAMEUTIL_get_parent_object +3 | GetGeometryPointerObject | $a0: +0x124:R4 |  |
| 10f34a0 | GetAliasLayoutObjWorkSize | 48 | recompiled | - | GetClothWorkSize | $a0: +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f4890 | ClipLayoutObjModel | 392 | decomp_walk.cpp | - | GetLayoutClass, iosgluGetCameraContext, modelClipSkin, modelClipSolid | - |  |
| 10f4a18 | manager_sub_010F4A18 | 816 | decomp_light.cpp | manager_sub_010F4FB8 | GetCharacterClass, GetLayoutClass, getSpecularity, iosCalcFColorToBrightness, iosCopyMatrix +3 | - |  |
| 10f4d48 | CalcLightLayoutObj | 624 | decomp_hot43b.cpp | - | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass, SetGeoDispClassLightColMan, glSetLightColor +3 | - |  |
| 10f52c8 | DispDispObjGroupModel | 1320 | decomp_walk.cpp | gamecore_sub_01344F60 | AddSortObj, ClearSortList, GetCharacterClass, GetFirstSortObj, GetGeoDispClass +14 | globals: 0118caf8:W 0118cb00:R 0118cb08:R 0118cb0c:&R |  |
| 10f5820 | InitLayoutObjMatrix | 256 | decomp_group.cpp | gamecore_sub_01346318 | GetLayoutClass, GetLayoutDispObjGroup, isysGroupForExit, isysGroupForFirst, isysGroupForNext | - |  |
| 10f5920 | DispLayoutObjModel | 272 | decomp_walk.cpp | DispWithSwordBlur, DispWithTorchBlur | DispCloth, GetCharacterClass, GetLayoutClass, manager_sub_010F3A08, manager_sub_010F3CF0 +1 | globals: 0118caf4:R 0118caf8:R |  |
| 10f5a30 | initSortDisplayObj | 120 | decomp_liftd7_03.cpp | gamecore_sub_01344B28 | CreateSortList | globals: 0118cb08:&RW 0118cb0c:&R |  |
| 10f5aa8 | deleteSortLayoutObj | 80 | recompiled | - | DeleteSortList | globals: 0118cb08:R 0118cb0c:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f5af8 | GetResetCameraFlagForDisplay | 16 | decomp_liftd4_01.cpp | DispWithSwordBlur, DispWithTorchBlur | - | globals: 0118caf8:R |  |
| 10f5b08 | getSubObjDispSw | 16 | recompiled | - | - | globals: 0118caf4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f5b38 | SetLayoutDisplayFlag | 56 | decomp_liftd7_03.cpp | gamecore_sub_014098A0, gamecore_sub_0140A8A8, gamecore_sub_0140AAF0 | cameraBlurClearIntensityArray, solarFlareReset | globals: 0118cb00:W |  |
| 10f5b70 | GetAmbientBrightnessOfLayoutObject | 80 | decomp_liftd7_03.cpp | gamecore_sub_013FC148, gamecore_sub_013FCFC8 | GetLayoutClass, iosCalcFColorToBrightness | - |  |
| 10f5bc8 | CreateLightColMan | 304 | decomp_liftd6_01.cpp | gamecore_sub_01346420 | GetGameLightClass, GetGlobalLightClass, _isysCreateObj, isysAddGroupWithLinkParam, memset | globals: 010f74e0:& 010f8b50:& 0118cb10:R |  |
| 10f5cf8 | LoopExecPreLightColMan | 616 | decomp_liftd6_00.cpp | manager_sub_010F4FB8 | ForceDelGameLighObj, ForceDelGlobalLighObj, GetGameLightClass, GetGlobalLightClass, iosMemToSprDmaAdrCopy +4 | globals: 01216130:& 70000510:W 70000520:W 7000052c:RW |  |
| 10f6170 | InitStageLightColMan | 488 | decomp_liftd7_01.cpp | manager_sub_010919A0 | CalcColorParamAmbientLight, CalcColorParamDistantLight, ExchangeLWQuaternion, GetQuaternionByDegEulerYXZ, globalLightCalcLightNum +1 | $a0: +0x0:R16; $a1: +0x0:R16; globals: 0118cb10:R |  |
| 10f6358 | GetLightColMan | 16 | decomp_liftd6_00.cpp | AddGaLightMan, AddGlLightMan, DeleteGameLight, DeleteGlobalLight, ForceDelGameLighObj, ForceDelGlobalLighObj +2 | - | globals: 0118cb10:R |  |
| 10f6368 | InitLightColMan | 96 | decomp_liftd6_01.cpp | gamecore_sub_01346420 | isysInitGroup, memset | globals: 01216130:& 01216644:& 01216648:& 0121664c:& 01216654:& 01216658:& |  |
| 10f63c8 | LoopExecAfterLightColMan | 64 | decomp_liftd3_01.cpp | manager_sub_010F4FB8 | iosSprToMemDmaAdrCopy | globals: 0118cb10:W 01216130:& |  |
| 10f6408 | TaskExecLightColMan | 24 | decomp_liftd4_01.cpp | gamecore_sub_01346420 | - | globals: 0118cb10:W 01216130:& |  |
| 10f64c0 | SetGeoDispClassLightColMan | 176 | decomp_liftd6_00.cpp | CalcLightLayoutObj, calc_light_particle_geodisp | ExecGameLight, ExecGlobalLightMan, SetWorkFromGlobalLight, manager_sub_010F5F60 | $a0: +0x10:R16 +0xc0:R1 +0xec:R4; globals: 0118cb10:R |  |
| 10f6570 | getColLightDebugSts | 16 | recompiled | - | - | globals: 0118cb14:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f6580 | GetColLightDebugSts | 16 | recompiled | - | - | globals: 0118cb14:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f6900 | DeleteGameLight | 240 | decomp_liftd6_01.cpp | SubCallDeleteFuncNonISysObjTypeRes, manager_sub_0110C328 | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +1 | - |  |
| 10f69f0 | UpdateClipGameLight | 368 | recompiled | manager_sub_01108960, updateClipGameLightGeo | iosCalcIcolorToBrightness | $a0: +0x0:W16 +0x10:W16 +0x20:R4/W1 +0x21:W1 +0x22:W1 +0x23:W1 +0x24:R4/W4 +0x28:R4/W4 +0x34:W4 +0x38:W1 +0x3c | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f6d48 | ExecGameLight | 1512 | decomp_liftd4_01.cpp | SetGeoDispClassLightColMan | GetImpactWorstWorkLightClass, iosIColToFvec, manager_sub_010F6B60 | $a0: +0x0:R16 +0x14:R4; $a2: +0x520:R4 |  |
| 10f7330 | NewCreateGameLightClass | 432 | recompiled | - | GetLightColMan, _isysDeleteGroup, gluGetLookAtPosition, iosCalcIcolorToBrightness, isysAddGroupWithLinkParam +2 | globals: 001e0f40:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f74e0 | deleteGameLightObj | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f74e8 | GetGameLightClass | 32 | decomp_liftd6_00.cpp | CreateLightColMan, LoopExecPreLightColMan | - | $a0: +0x8:R1 |  |
| 10f7508 | SetGameLightAvailable | 8 | decomp_liftd6_00.cpp | PlayScriptAnimationSequence, manager_sub_011030D0 | - | $a0: +0x39:W1 |  |
| 10f7510 | AddGaLightMan | 328 | decomp_liftd6_01.cpp | manager_sub_010FBAF0, manager_sub_01105938 | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +2 | - |  |
| 10f7658 | ForceGameLightFlagEnvelope | 32 | decomp_liftd6_01.cpp | manager_sub_010FBAF0, manager_sub_01105938 | - | $a0: +0x2c:R4/W4 |  |
| 10f7678 | IsEnvelopeGameLightType | 24 | decomp_hot46j.cpp | manager_sub_01108960, updateClipGameLightGeo, updateGameLightGeo | - | $a0: +0x2c:R4 |  |
| 10f7690 | UpdateGameLight | 328 | decomp_hot46j.cpp | manager_sub_01108960, updateGameLightGeo | iosCalcIcolorToBrightness | $a0: +0x0:W16 +0x10:W16 +0x20:R4/W1 +0x21:W1 +0x22:W1 +0x23:W1 +0x24:R4/W4 +0x28:R4/W4 +0x34:W4 +0x38:W1 +0x3c |  |
| 10f77d8 | CalcColorParamGameLight | 88 | recompiled | - | iosCalcIcolorToBrightness | $a0: +0x20:R4 +0x24:R4 +0x38:W1 +0x3c:W4 +0x40:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f7830 | GameLightCalcOutSideParam | 16 | recompiled | - | - | $a0: +0x28:R4 +0x34:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f78a8 | SetGameLightUserParam | 8 | recompiled | gamecore_sub_01386C60 | - | $a0: +0x50:W4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 2 calls, d10:fv2,d9:dv1) |
| 10f78b0 | SetGameLightUserFunc | 8 | recompiled | gamecore_sub_01386C60 | - | $a0: +0x54:W4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 2 calls, d10:fv2,d9:dv1) |
| 10f78b8 | InitGameLightClass | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f78c0 | GameLightCalcImpact | 64 | recompiled | - | - | $a0: +0x28:R4 +0x40:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f7d88 | AddGlLightMan | 472 | decomp_liftd6_00.cpp | manager_sub_01105938 | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +3 | $a0: +0x4:R4 |  |
| 10f7f60 | DeleteGlobalLight | 296 | decomp_liftd6_00.cpp | SubCallDeleteFuncNonISysObjTypeRes, manager_sub_0110C328 | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +1 | - |  |
| 10f81e8 | GlobalLightConsistencyWorkGrp | 376 | decomp_liftd6_00.cpp | manager_sub_01105938 | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +2 | - |  |
| 10f8728 | ExecGlobalLightMan | 1016 | decomp_liftd6_00.cpp | SetCameraReverbFlag, SetGeoDispClassLightColMan | GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix, GetRateInOutEllipticalSphereVolumeWithInverseMatrix, getAmbientColorC, iosCalcFColorToBrightness +2 | $a0: +0x0:R16 +0x18:R4; $a1: +0x100:R16/W16 +0x110:R4/W4 +0x114:R4/W4 +0x118:R4/W4 +0x1f0:W16 +0x21c:W4 +0x3a0 |  |
| 10f8b20 | GetGlobalLightClass | 32 | decomp_liftd6_00.cpp | CreateLightColMan, LoopExecPreLightColMan | - | $a0: +0x8:R1 |  |
| 10f8b40 | SetGlobalLightAvailable | 16 | decomp_liftd6_00.cpp | PlayScriptAnimationSequence, manager_sub_011030D0 | - | $a0: +0x144:W4 |  |
| 10f8b50 | deleteGlobalLightObj | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f8b58 | ForceDelGlobalLighObj | 136 | recompiled | LoopExecPreLightColMan | GetLightColMan, _isysDeleteGroup, isysAddGroupWithLinkParam | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f8be0 | GlobalLightCalcOutSideParam | 176 | recompiled | - | - | $a0: +0xf0:R4 +0xf4:R4 +0xf8:R4 +0x118:W4 +0x134:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f8c90 | ForceGlobalLightFlagEnvelope | 32 | decomp_liftd6_01.cpp | manager_sub_01105938 | - | $a0: +0x110:R4/W4 |  |
| 10f8cb0 | IsEnvelopeGlobalLightType | 24 | decomp_hot46j.cpp | manager_sub_01108960 | - | $a0: +0x110:R4 |  |
| 10f8cc8 | SetColorParamAmbientLight | 104 | recompiled | - | iosCalcFColorToBrightness | $a0: +0x100:W16 +0x12c:R4/W4 +0x130:W4 +0x140:W4; $a1: +0x0:R16 +0x24:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f8d30 | UpdateGlobalLightInRange | 48 | decomp_hot46j.cpp | manager_sub_01108960 | - | - |  |
| 10f8d60 | UpdateGlobalLightOutRange | 224 | decomp_hot46j.cpp | UpdateOutRangeLightGeo, manager_sub_01108960 | - | $a0: +0xc0:W16 +0xd0:W16 +0xf4:R4 +0xf8:R4 +0x118:W4 +0x134:W4 +0x13c:W4 |  |
| 10f8e40 | UpdateGlobalLightAmbient | 104 | decomp_hot46j.cpp | manager_sub_01108960 | iosCalcFColorToBrightness | $a0: +0x100:W16 +0x12c:R4/W4 +0x130:W4 +0x140:W4 |  |
| 10f8ea8 | UpdateGlobalLightDistant | 232 | decomp_hot46j.cpp | manager_sub_01108960 | iosCalcFColorToBrightness, manager_sub_010F8088 | $a0: +0x30:R1 |  |
| 10f8f90 | IsEnvelopeDistantLightType | 96 | decomp_hot46j.cpp | manager_sub_01108960 | - | $a0: +0x30:R1; $a1: +0x24:R4 |  |
| 10f8ff0 | globalLightCalcLightNum | 216 | decomp_liftd2_00.cpp | InitStageLightColMan | - | $a0: +0x110:R4/W4 +0x120:R4/W4 +0x124:R4/W4 |  |
| 10f90c8 | CalcColorParamDistantLight | 72 | decomp_liftd7_03.cpp | InitStageLightColMan | iosCalcFColorToBrightness | $a0: +0x20:W4 +0x24:R4 +0x28:W4 |  |
| 10f9110 | CalcColorParamAmbientLight | 88 | decomp_liftd7_03.cpp | InitStageLightColMan | iosCalcFColorToBrightness | $a0: +0x12c:R4 +0x130:W4 +0x140:W4 |  |
| 10f9168 | SetWorkFromGlobalLight | 264 | decomp_liftd7_03.cpp | SetGeoDispClassLightColMan | GetNextWorkLightClass | - |  |
| 10f9270 | GetNextWorkLightClass | 64 | recompiled | SetWorkFromGlobalLight | - | $a0: +0x530:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f92b0 | GetImpactWorstWorkLightClass | 80 | decomp_liftd4_01.cpp | ExecGameLight | - | $a0: +0x530:R4 +0x534:R4 |  |
| 10f9300 | GetDirColorFromWorkLight | 56 | recompiled | manager_sub_010F5F60 | - | $a0: +0x0:W16; $a1: +0x0:W16; $a2: +0x10:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f9338 | CreateLightObj | 184 | recompiled | initLightObj | GetLayoutObjWorkSize, InitLayoutObj, _isysCreateObj | $a2: +0x7c:R4 +0x80:R16 +0x90:R16 +0xa0:R4 +0xa4:R4; globals: 010f9480:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f93f0 | GetLightClass | 32 | recompiled | CallLayoutObjDebugResetFunc, ChangeLayoutObjCollisionResourceAttribute, CheckLayoutObjChainExist, CreateLayoutObj, CreateLayoutObjVU0Secure, FlushCollisionWork +24 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f9410 | CreateAmbientLightObj | 112 | decomp_liftd7_03.cpp | initAmbientLightObj | _isysCreateObj | $a2: +0x0:R4 +0x4:R4 +0x8:R4; globals: 010f9528:& |  |
| 10f9498 | InitLightObj | 112 | recompiled | - | InitLayoutObj | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10f9508 | GetAmbientLightClass | 32 | decomp_liftd2_00.cpp | getAmbientColorC | - | $a0: +0x8:R1 |  |
| 10f9530 | InitAmbientLightObj | 48 | recompiled | - | - | $a0: +0x8:R1; $a1: +0x0:R4 +0x4:R4 +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 10fd2b8 | setLayoutClassInitParam | 1184 | decomp_liftd2_00.cpp | arrowManagerInit, initLayoutObj, initLayoutObjVU0Secure | - | $a0: +0x0:W4 +0x7:W4 +0x8:W4 +0xf:W4 +0x10:W4 +0x17:W4 +0x18:W4 +0x1f:W4; $a3: +0x0:R4; globals: 001e0ed4:R 01 |  |
| 10fd758 | initLayoutObj | 440 | decomp_liftd2_00.cpp | CreateClothChildLayoutObj, SCRSetupArrow1ByPlayer2, loadObj, manager_sub_010FBAF0, manager_sub_01105938, player_updateSlipEffect | CheckLayoutObjChainExist, CreateLayoutObj, isysAddGroupWithLinkParam, setLayoutClassInitParam | globals: 001e0ed4:R 0118cb20:R |  |
| 10fd910 | initLayoutObjVU0Secure | 440 | decomp_liftd6_00.cpp | CreateClothChildLayoutObj, init_col_attr_table_segment | CheckLayoutObjChainExist, CreateLayoutObjVU0Secure, isysAddGroupWithLinkParam, setLayoutClassInitParam | globals: 001e0ed4:R 0118cb20:R |  |
| 10fddb0 | initScriptLayoutObj | 1152 | decomp_liftd6_01.cpp | SCRSetupMissile1, SCRSetupWeaponMinotaur, SCRSetupWeaponPlayer, loadObj, manager_sub_01105938 | CheckLayoutObjChainExist, CreateScriptLayoutObj, isysAddGroupWithLinkParam, manager_sub_010FDAC8 | $a1: +0x0:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x28:R4; globals: 001e0ed4:R 0118cb20:R |  |
| 10fe230 | initAliasLayoutObj | 824 | decomp_liftd6_01.cpp | loadObj | CheckLayoutObjChainExist, CreateAliasScriptLayoutObj, GetLayoutClass, isysAddGroupWithLinkParam, manager_sub_010FDAC8 | $a1: +0x0:R4; globals: 001e0ed4:R 0118cb20:R |  |
| 11008a0 | GetLayoutObjGroup | 16 | decomp_liftd6_00.cpp | CreateLayoutObj, CreateLayoutObjVU0Secure, GetLayoutObjNum, InitAliasLayoutObj, InitLayoutObj, SaveLayoutObjGroupAllGeo | - | globals: 0118cb20:R |  |
| 11008b0 | GetLayoutDispObjGroup | 16 | decomp_group.cpp | CreateGeoDispObj, CreateLayoutObj, CreateLayoutObjVU0Secure, DispDispObjGroupModel, FlushLayoutObjImmediate, GetLayoutDispObjNum +4 | - | globals: 0118cb20:R |  |
| 11009a8 | initDispObjResource | 112 | recompiled | - | - | $a0: +0x0:R4; globals: 001e0ed4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1100a18 | initShadowObjResource | 112 | recompiled | - | - | $a0: +0x0:R4; globals: 001e0ed4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1100c60 | SearchLayoutObjDefByName | 256 | decomp_liftd6_00.cpp | CreateClothChildLayoutObj, manager_sub_010BAAF0, manager_sub_0110D618 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 1100d60 | SearchScriptLayoutObjDefByName | 256 | decomp_liftd6_00.cpp | manager_sub_010BAAF0 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 1101108 | initLightObj | 304 | recompiled | loadObj | CreateLightObj, memset | $a1: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4; $a2: +0x14:R4 +0x18:R4 +0x | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1101238 | initScriptSpriteObj | 344 | decomp_liftd6_01.cpp | loadObj, ps2__SCRStartSpriteAnim | CreateScriptSpriteObj | $a1: +0x0:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4 +0x30:R4  |  |
| 1101390 | initScriptSpriteMenuObj | 368 | decomp_liftd6_01.cpp | loadObj, ps2__SCRStartSpriteMenuAnim | CreateScriptSpriteMenuObj | $a1: +0x0:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4 +0x28:R4 +0x2c:R4 +0x30:R4; |  |
| 1101580 | initAmbientLightObj | 208 | decomp_liftd6_00.cpp | loadObj | CreateAmbientLightObj, isysAddGroupWithLinkParam | $a1: +0x8:R4 +0xc:R4; globals: 001e0ed4:R 0118cb20:R |  |
| 1101718 | GetAmbientLightObjGroup | 16 | decomp_liftd2_00.cpp | getAmbientColorC | - | globals: 0118cb20:R |  |
| 1101728 | GetModelObjGroup | 16 | recompiled | - | - | globals: 0118cb20:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1101b70 | loadCdWinTexFile | 224 | decomp_liftd6_00.cpp | - | iosMallocAlign, isysCreateModuleObjExisted, isysGetXffEntryPointPreRelocation, sheetFileRead, texGetWorkSize +2 | globals: 001e0c88:& 01216d00:& 01216d1c:R |  |
| 1101c50 | unloadCdWinTexFile | 88 | recompiled | - | isysRequestDeleteObj, sheetFileGetSize | globals: 01216d00:& 01216d1c:RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11023c8 | deleteAmbientLightObj | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11023e0 | getMemoryInfoLightSw | 16 | recompiled | - | - | globals: 0118cbd4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11084c0 | DebugDispVolumeShapeColor | 408 | recompiled | DebugDispVolumeAll, DebugDispVolumeAllColor, DebugDispVolumeShape | glDisable, glEnable, glLoadMatrixf, iosScaleMatrixV, kernel_sub_001B7B60 +2 | $a0: +0x0:R16 +0x50:R4 +0x54:R4 +0x58:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1108658 | updateVolumeShapePQSGeo | 776 | decomp_hot46j.cpp | StartAnimationSubObjInfinitePart, manager_sub_010FA830, manager_sub_01108960 | ExchangeLWVector, GetLayoutClass, GetMotionNodeQuaternionPosToolCoord, SetQuaternionByAxisRotateVWithNoRegularize | globals: 001e0fb0:& 001e0fe0:& |  |
| 110a8f8 | updateLayoutGeo | 560 | decomp_liftd4_01.cpp | manager_sub_010FBAF0 | GetLayoutClass, GetMotionNodeQuaternionPosToolCoord, SetLayoutClassScaleElem, UpdateLayoutClassPastSimpleGeometry, UpdateLayoutClassSimpleGeometry | $a0: +0x134:R4 +0x13c:R4 |  |
| 110b038 | updateLightGeo | 472 | recompiled | manager_sub_010FA830 | GetLayoutClass, GetMotionNodeQuaternionPosToolCoord | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110b648 | updateGameLightGeo | 232 | recompiled | manager_sub_010FA830 | IsEnvelopeGameLightType, UpdateGameLight | $a0: +0x124:R4 +0x13c:R4 +0x158:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110b730 | updateClipGameLightGeo | 232 | recompiled | manager_sub_010FA830 | IsEnvelopeGameLightType, UpdateClipGameLight | $a0: +0x124:R4 +0x13c:R4 +0x158:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110bb48 | DebugDispVolumeShape | 56 | recompiled | - | DebugDispVolumeShapeColor | globals: 01195010:&R 01195018:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110bb80 | DebugDispVolumeAll | 208 | recompiled | - | DebugDispVolumeShapeColor, GetJudgeVolumeClass, GetJudgeVolumeClassObjectGroup, ScriptAnimationObjOfJudgeVolumeObject, isysGroupForExit +3 | globals: 01195020:&R 01195028:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110bc50 | DebugDispVolumeAllColor | 184 | recompiled | - | DebugDispVolumeShapeColor, GetJudgeVolumeClass, GetJudgeVolumeClassObjectGroup, ScriptAnimationObjOfJudgeVolumeObject, isysGroupForExit +3 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110c140 | getExternalLWSFlag | 16 | recompiled | - | - | globals: 0118ccb8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110c878 | UpdateOutRangeLightGeo | 160 | recompiled | - | UpdateGlobalLightOutRange | $a0: +0x13c:R4 +0x158:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110cd90 | CreateScriptLayoutObj | 312 | decomp_liftd7_03.cpp | initScriptLayoutObj | GetLayoutObjWorkSize, GetScriptAnimationClass, GetScriptObjWorkSize, InitDamageClass, InitLayoutObj +4 | $a2: +0xa0:R4; globals: 0110d250:& 0110d2a0:& |  |
| 110cec8 | CreateAliasScriptLayoutObj | 320 | decomp_liftd6_00.cpp | initAliasLayoutObj, initScriptCharObj | GetLayoutObjWorkSize, GetScriptAnimationClass, GetScriptObjWorkSize, InitAliasLayoutObj, InitDamageClass +4 | $a2: +0xa0:R4; globals: 0110d250:& 0110d2a0:& |  |
| 110d008 | FlushScriptLayoutObj | 80 | decomp_liftd6_01.cpp | manager_sub_0110C2C8 | FlushLayoutObj, GetScriptLayoutClass, flushScriptObj, iosFree | - |  |
| 110d058 | GetScriptLayoutClass | 72 | decomp_liftd7_08.cpp | CallLayoutObjDebugResetFunc, ChangeLayoutObjCollisionResourceAttribute, CheckDamageLetter, CheckLayoutObjChainExist, CreateLayoutObj, CreateLayoutObjVU0Secure +57 | GetScriptAnimationClass | $a0: +0x8:R1 |  |
| 110d0a0 | SetScriptLayoutObjLetterProcMode | 88 | decomp_liftd6_02.cpp | demo_battle, demo_common, demo_end, demo_end2, demo_end3, demo_last_battle1 +5 | GetScriptAnimationClass | $a0: +0x8:R1 |  |
| 110d0f8 | ExecScriptLayoutObj | 56 | decomp_scriptvm3.cpp | gamecore_sub_013CF258 | CheckLetter, GetDamageClass, ResetDamageClass | globals: 01166c98:& |  |
| 110d130 | InitScriptLayoutObj | 184 | decomp_liftd6_01.cpp | CreateScriptAnimationObj | GetScriptAnimationClass, InitDamageClass, InitLayoutObj, initScriptObj | $a0: +0x0:R4 +0x8:R1 |  |
| 110d1e8 | GetScriptLayoutObjWorkSize | 64 | decomp_liftd6_01.cpp | CreateScriptAnimationObj | GetLayoutObjWorkSize, GetScriptObjWorkSize | - |  |
| 110d228 | FlushScriptLayoutObjImmediate | 40 | decomp_liftd6_00.cpp | manager_sub_011031F8 | FlushLayoutObjImmediate, flushScriptObjImmediate | - |  |
| 110d2c8 | InitAliasScriptLayoutObj | 184 | recompiled | - | GetScriptAnimationClass, InitAliasLayoutObj, InitDamageClass, initScriptObj | $a0: +0x0:R4 +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110dc20 | AddSortObj | 264 | decomp_walk.cpp | DispDispObjGroupModel | - | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4; $a1: +0x4:R4 +0x8:W4 +0xc:W4 |  |
| 110dd28 | GetNextSortObj | 176 | decomp_sortlist.cpp | DispDispObjGroupModel, manager_sub_010F4FB8 | - | $a0: +0x0:R4 +0x4:R4 +0x10:R4/W4; $a1: +0x8:R4 |  |
| 110ddd8 | GetFirstSortObj | 80 | recompiled | DispDispObjGroupModel, manager_sub_010F4FB8 | - | $a0: +0x0:R4 +0x4:R4 +0x10:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110de28 | InitSortObj | 16 | decomp_liftd6_00.cpp | CreateGeoDispObj, manager_sub_010EFD08 | - | $a0: +0x0:W4 +0x8:W4 +0xc:W4 |  |
| 110de38 | ClearSortList | 56 | decomp_sortlist.cpp | DispDispObjGroupModel | - | $a0: +0x0:R4 +0x4:R4 +0x10:W4 |  |
| 110de70 | CreateSortList | 216 | decomp_liftd7_03.cpp | initSortDisplayObj | iosMallocAlign | globals: 001e0c88:& |  |
| 110df48 | DeleteSortList | 48 | recompiled | deleteSortLayoutObj | iosFree | $a0: +0x0:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110ecd0 | SpriteMenuIsFadeCheckSelectLayer | 248 | decomp_liftd11_00.cpp | gamecore_sub_013A0000, gamecore_sub_013A2410, gamecore_sub_013AE1E8, gamecore_sub_013AF040, gamecore_sub_01409718, gamecore_sub_014098A0 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 110fd90 | SpriteMenuSetSpriteObjectParam | 576 | decomp_liftd7_09.cpp | gamecore_sub_013F8B58, gamecore_sub_013F9BA8, gamecore_sub_01408910, gamecore_sub_01408F70, gamecore_sub_0140A2C0, gamecore_sub_0140A428 +1 | spriteInitObject, spriteSetBlendMode, spriteSetColor, spriteSetFade, spriteSetPosition +4 | $a1: +0x0:R4 +0x8:R4 +0x10:R4 +0x14:R4; globals: 001e0ed4:R |  |
| 1111998 | GetSpriteMenuClass | 32 | decomp_liftd6_02.cpp | EndMessageScript, ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayOverBGScript, ReplayPauseScript +29 | - | $a0: +0x8:R1 |  |
| 11119b8 | GetScriptSpriteMenuClass | 32 | decomp_liftd2_00.cpp | GetScriptClass | - | $a0: +0x8:R1 |  |
| 11119d8 | SpriteMenuSetPadEventCallback | 48 | decomp_liftd10_00.cpp | gameover | - | $a0: +0x8:R1 |  |
| 1111a08 | SpriteSubMenuSetPadEventCallback | 48 | decomp_liftd10_00.cpp | gamecore_sub_013AE1E8 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1111a38 | SpriteMenuSetDispFlag | 144 | decomp_liftd2_00.cpp | ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayOverBGScript, ReplayPauseScript, ReplayRetryScript +43 | - | $a0: +0x8:R1 |  |
| 1111ac8 | SpriteMenuSetDispFlagNoFade | 176 | recompiled | - | manager_sub_01113BD8 | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1111b78 | SpriteMenuSetDispFlagByHash | 248 | recompiled | gamecore_sub_0139FC98, gamecore_sub_013A0000, gamecore_sub_013A20E8, gamecore_sub_013A2410, gamecore_sub_0140AAF0 | - | $a0: +0x8:R1 | reached (366294 calls; d10:hang1,d7:in_mash2,d7:ride,d7:walk,d8:ng1,d8:tb0,d8:tb4,d8:tb4c,d8:tb4h,d8:tb4r,d8:tb4w,d8:tb4x,d8:tb4y,d8:tbz,d9:ag1,d9:dv1,d9:dv2,d9:dv3,d9:dv4,d9:lg1,d9:sc1,d9:walk9,d9:wl1) |
| 1111c70 | SpriteMenuSetDispFlagAll | 248 | decomp_liftd6_02.cpp | EndMessageScript, ReplayInScript, chooseLangScript, controllerAlertScript, gamecore_sub_013A0000, gamecore_sub_013A2410 +8 | manager_sub_01113BD8 | $a0: +0x8:R1 |  |
| 1111d68 | SpriteMenuSetMenuDispFlag | 160 | recompiled | SetItemMenuDisp | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1111e08 | SpriteMenuGetSpriteMenuClassParam | 24 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1111e20 | SpriteMenuSetLayer | 120 | decomp_liftd6_02.cpp | EndMessageScript, ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayPauseScript, ReplayRetryScript +33 | - | $a0: +0x8:R1 |  |
| 1111e98 | SpriteMenuSetLayerPriority | 72 | decomp_liftd11_00.cpp | gamecore_sub_013AE1E8 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1111ee0 | SpriteMenuSetLayerPriorityAllLayer | 128 | decomp_liftd11_00.cpp | gamecore_sub_013AE1E8, gamecore_sub_0140AAF0, memoryCardLoadScript | - | $a0: +0x8:R1 |  |
| 1111f60 | SpriteMenuGetLayerStatus | 48 | decomp_liftd7_04.cpp | gamecore_sub_0140AAF0, gameover | - | $a0: +0x8:R1 |  |
| 1111f90 | SpriteMenuGetLayerByHash | 120 | decomp_liftd7_04.cpp | gamecore_sub_013AD738, gamecore_sub_01408F70, gamecore_sub_0140A2C0, gamecore_sub_0140A428 | - | $a0: +0x8:R1 |  |
| 1112008 | SpriteMenuGetLayerNoByHash | 128 | decomp_liftd7_04.cpp | ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayOverBGScript, ReplayPauseScript, ReplayRetryScript +40 | - | $a0: +0x8:R1 |  |
| 1112088 | SpriteMenuGetPartsParamByHash | 200 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112150 | SpriteMenuGetPartsParamByHashStr | 272 | decomp_liftd7_04.cpp | gamecore_sub_01408F70, gamecore_sub_0140A2C0, gameover | MakeStrHashValue | $a0: +0x8:R1 |  |
| 1112260 | SpriteMenuGetPartsDefByHash | 216 | recompiled | gamecore_sub_013FF9A8, timeAttackInScript, timeAttackTimeScript | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112338 | SpriteMenuGetPartsDefByHashStr | 288 | decomp_liftd7_04.cpp | gamecore_sub_01408F70, gamecore_sub_0140A428 | MakeStrHashValue | $a0: +0x8:R1 |  |
| 1112458 | SpriteMenuGetPartsDefValueByHash | 216 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112530 | SpriteMenuGetPartsDefValueByHashStr | 288 | recompiled | gamecore_sub_013AD208, gamecore_sub_013AD738 | MakeStrHashValue | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112650 | SpriteMenuSetPartsDefValueByHash | 216 | recompiled | SetItemMenuDisp | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112728 | SpriteMenuSetPartsDefValueByHashStr | 304 | decomp_liftd7_04.cpp | bootmccheck, gamecore_sub_013A0000, gamecore_sub_013AC758, gamecore_sub_013AD208, gamecore_sub_013AD738 | MakeStrHashValue | $a0: +0x8:R1 |  |
| 1112858 | SpriteMenuSetPartsDefValueByHashStrWithoutErrorCheck | 320 | decomp_liftd7_04.cpp | bootmccheck, gamecore_sub_0139FF08, gamecore_sub_013A2318 | MakeStrHashValue | $a0: +0x8:R1 |  |
| 1112998 | SpriteMenuSetSpriteMenuSoundSwitchByHash | 224 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112a78 | SpriteMenuSetSpriteMenuSoundSwitchByHashStr | 296 | recompiled | - | MakeStrHashValue | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112ba0 | SpriteMenuGetSelectLayer | 24 | decomp_liftd11_00.cpp | gamecore_sub_013AD860, gamecore_sub_013ADB50, gamecore_sub_013ADDA0, title_options | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112bb8 | SpriteMenuGetPadEvent | 24 | decomp_liftd11_00.cpp | gamecore_sub_013ADDA0, gamecore_sub_01409718 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112bd0 | SpriteMenuGetProgressiveOffset | 64 | decomp_liftd11_00.cpp | gamecore_sub_01408F70, gamecore_sub_0140A428 | getDrawScreenHeight | - |  |
| 1112c10 | SpriteMenuGetMenuIdByPartsId | 120 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112c88 | SpriteMenuGetMenuIdByHash | 208 | decomp_liftd7_04.cpp | ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayPauseScript, ReplayRetryScript, SelectOmakeItem +17 | - | $a0: +0x8:R1 |  |
| 1112d58 | SpriteMenuGetMenuIdByHashStr | 272 | recompiled | - | MakeStrHashValue | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112e68 | SpriteMenuGetSelectMenuIdBySpriteMenuClass | 56 | decomp_liftd7_04.cpp | bootmccheck, gamecore_sub_013A0000, gamecore_sub_013A2410 | - | $a0: +0x4:R4 +0xc:R4 |  |
| 1112ea0 | SpriteMenuGetSelectMenuId | 80 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112ef0 | SpriteMenuSetSelectMenuId | 96 | decomp_liftd11_00.cpp | ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayPauseScript, ReplayRetryScript, gamecore_sub_013ACD18 +11 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112f50 | SpriteMenuGetSelectSubMenuId | 80 | decomp_liftd11_00.cpp | gamecore_sub_013ADB50 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1112fa0 | SpriteMenuSetSelectSubMenuId | 232 | decomp_liftd11_00.cpp | gamecore_sub_013AC758, gamecore_sub_013ACD18, gamecore_sub_013ACFB0, gamecore_sub_013ADB50, gamecore_sub_013ADDA0 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113088 | SpriteMenuGetSelectMenuMode | 80 | decomp_liftd11_00.cpp | gamecore_sub_013AC758 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11130d8 | SpriteMenuSetSelectMenuMode | 88 | recompiled | gamecore_sub_013ADB50 | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113130 | SpriteMenuSetDefaultSelectMenu | 72 | decomp_liftd7_04.cpp | gameover, titlemenu | - | $a0: +0x8:R1 |  |
| 1113178 | SpriteMenuSetSliderValue | 232 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113260 | SpriteMenuSetSliderValueByHash | 200 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113328 | SpriteMenuSetSliderValueByHashStr | 288 | decomp_liftd11_00.cpp | gamecore_sub_013AE1E8 | MakeStrHashValue | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113448 | SpriteMenuSetCheckBoxByPartsId | 104 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11134b0 | SpriteMenuSetCheckBoxByHash | 240 | recompiled | CheckSelectDisp, SelectOmakeItem, SetItemMenuDisp | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11135a0 | SpriteMenuSetNumPartsFormatByHash | 232 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1113688 | SpriteMenuSetNumPartsFormatByHashStr | 304 | decomp_liftd7_04.cpp | bootmccheck | MakeStrHashValue | $a0: +0x8:R1 |  |
| 11137b8 | SpriteMenuSetNumPartsFormatByPartsParam | 40 | decomp_liftd7_04.cpp | gamecore_sub_0139F470, gamecore_sub_0139FAA0 | - | $a1: +0x18:R4/W4 |  |
| 1113848 | SpriteMenuIsFadeCheckAllLayer | 128 | recompiled | EndMessageScript, ReplayInScript, gamecore_sub_013FF6B0, gamecore_sub_01400EB0, gamecore_sub_0140A8A8, gamecore_sub_0140AAF0 +4 | SpriteMenuIsFadeCheckLayer | $a0: +0x8:R1 | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 11138c8 | SpriteMenuIsFadeCheckLayer | 136 | decomp_liftd2_00.cpp | SpriteMenuIsFadeCheckAllLayer, bootmccheck, bootscelogo | - | $a0: +0x8:R1 |  |
| 1113950 | SpriteMenuGetFadeByLayer | 112 | decomp_liftd2_00.cpp | gamecore_sub_01409DD0, gamecore_sub_0140A6A8 | - | $a0: +0x8:R1 |  |
| 11139c0 | DeleteScriptSpriteMenuObj | 80 | decomp_liftd7_04.cpp | - | flushScriptObj | $a0: +0x8:R1 |  |
| 1113a10 | CreateScriptSpriteMenuObj | 216 | decomp_liftd7_03.cpp | initScriptSpriteMenuObj | GetScriptObjWorkSize, _isysCreateObj, initScriptObj, isysSetObjImmediateDeleteFunc, manager_sub_01111300 +1 | $a2: +0x28:R4; globals: 011139c0:& 01113d00:& |  |
| 1113b00 | spriteMenuCheckSelectPushPad | 72 | recompiled | st_gameflow | - | globals: 0118cd88:R 0118d688:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1114018 | InitSpriteObj | 344 | decomp_liftd8_00.cpp | CreateScriptSpriteObj | iosMallocAlign, manager_sub_01113D18 | $a0: +0x0:R4 +0x8:R1; $a1: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x20:R4 +0x24:R4 +0x28 |  |
| 1114170 | GetSpriteClass | 32 | decomp_liftd8_00.cpp | spritedisplay, spritedisplaytrigger, spriteselweapon, spriteweaponsight | - | $a0: +0x8:R1 |  |
| 1114190 | FlushSpriteObj | 48 | recompiled | - | - | $a0: +0x8:R1 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11141c0 | DeleteScriptSpriteObj | 120 | decomp_liftd7_04.cpp | - | flushScriptObj | $a0: +0x8:R1 |  |
| 1114238 | GetScriptSpriteClass | 32 | decomp_liftd2_00.cpp | GetScriptClass | - | $a0: +0x8:R1 |  |
| 1114258 | GetSpriteObjWorkSize | 32 | recompiled | - | - | $a0: +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1114278 | GetScriptSpriteObjWorkSize | 64 | recompiled | - | GetScriptObjWorkSize | $a0: +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11142b8 | CreateScriptSpriteObj | 232 | decomp_liftd8_00.cpp | initScriptSpriteObj | GetScriptObjWorkSize, InitSpriteObj, _isysCreateObj, initScriptObj, isysSetObjImmediateDeleteFunc | $a2: +0x8:R4 +0x38:R4; globals: 011141c0:& 01114478:& |  |
| 11143a0 | ExecScriptSpriteObj | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1114858 | ViewIntervalClip | 664 | decomp_liftd10_00.cpp | CharaCtrlAIControl, execObjectWithNonTaskSwitch, gamecore_sub_013CF258, gamecore_sub_013CF4E0, gamecore_sub_013CFCA0, ps2___execScrLwsOri +1 | GetCharacterClass, GetLayoutClass, GetScreenRadius, iosgluGetCameraContext | globals: 001e0ed4:R |  |
| 1114af0 | InitViewIntervalClip | 32 | decomp_liftd6_00.cpp | InitCharacterObj | - | $a0: +0x1164:W4 +0x1168:W4 +0x116c:W4 |  |
| 1114b10 | FlushRequestViewIntervalClip | 40 | decomp_liftd7_04.cpp | execObjectWithNonTaskSwitch, gamecore_sub_013CF258 | GetCharacterClass | - |  |
| 1118f48 | GetLayoutObjCollisionDBWithTargetAttributeJudgeFunc | 768 | recompiled | GetGeometryPointerWithCollisionAttributeJudgeFunc | GetLayoutClass | $a0: +0x0:R4/W4 +0x4:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1119248 | getLayoutClassCollisionIndepSurfBoundary | 384 | decomp_liftd9_90.cpp | CheckCollisionInsideOfSurfaceXZ, ClipCollisionDirect, ClipCollisionDirectCallBack, ClipCollisionDirectCallBackDoubleSide, ClipCollisionDirectCallBackObjAry, ClipCollisionDirectCallBackObjAryDoubleSide +11 | EntryModifiedManager | $a0: +0x9c:R4 +0x15c:R4 +0x174:R4 +0x184:R4 |  |
| 1119de0 | GetCollisionEdgePointerGlobalVerticalPlane | 1272 | decomp_liftd7_04.cpp | GetCollisionEdgePointerHVPlane | CompletePlaneD, EntryModifiedManager, GetLayoutClass, GetPlaneFittingPoint, GetPlaneFittingPointForUnnormalizedPlane +3 | $a0: +0x0:R4/W4 +0x4:R4/W4; $a1: +0x0:R16/W16; $a2: +0x0:R16/W16; $a3: +0x0:R4 +0x4:R4 +0x8:R4; globals: 001e0 |  |
| 111b410 | GetGeometryPointerWithCollisionDBDataGlobalPoint | 712 | decomp_liftd8_00.cpp | ConvertGeometryPointerZNormalMode, LinkSecureParentInfoByDBPos, gamecore_sub_013FC450 | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x30:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1; $a2: +0x0:R16 |  |
| 111b6d8 | GetGeometryPointerWithCollisionEdgeGlobalPoint | 792 | decomp_liftd4_01.cpp | manager_sub_010D48B8, manager_sub_010D5308 | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x30:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4; $a2: +0x0:R4 +0x4:R4 +0x |  |
| 111b9f0 | ClipCollisionDB | 1672 | decomp_liftd9_90.cpp | ClipCollisionDBDoubleSide, ClipCollisionDirect, ClipCollisionDirectCallBack, ClipCollisionDirectCallBackDoubleSide, ClipCollisionDirectCallBackObjAry, ClipCollisionDirectCallBackObjAryDoubleSide +12 | EntryModifiedManager, GetCollisionDBSurfBoundary, GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x30:W4 +0x40:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0 |  |
| 111ecf0 | ClipCollisionFinderWithKeep4DSlopeLimit | 544 | decomp_liftd8_00.cpp | SlideCollisionFinder4D, SlideCollisionFinder4DCB, charrootdyn_HslidecollisionCB, charrootdyn_slidecollisionCB | manager_sub_01118B88, manager_sub_0111C078, manager_sub_0111C810, manager_sub_0111DDC0 | $a0: +0x0:R16/W16 +0x10:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 111ef10 | ClipCollisionFinderWithKeep4DSlopeLimitCB | 640 | sotc_worker.cpp | SlideCollisionFinder4DCB, charrootdyn_HslidecollisionCB, charrootdyn_slidecollisionCB | manager_sub_01118B88, manager_sub_0111C078, manager_sub_0111C810, manager_sub_0111DDC0 | $a0: +0x0:R16/W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1121838 | DebugDispCollisionDBEdge | 576 | recompiled | DebugDispCollisionDBEdgeAngle | GetLayoutClass, glBegin, glBlendFunc, glColor4f, glEnable +9 | $a0: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1121a78 | DebugDispCollisionDBEdgeAngle | 432 | recompiled | - | DebugDispCollisionDBEdge, GetLayoutClass, iosInitKPrintfContext3D, kprintfContextCentering, manager_sub_01114C70 +1 | $a0: +0x0:R4 +0x4:R4; globals: 011954c0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1121c28 | DebugDispCollisionEdge | 824 | recompiled | gamecore_sub_0133D0D0 | GetLayoutClass, glBegin, glBlendFunc, glColor4f, glEnable +10 | globals: 01195498:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1121f60 | DebugDispCollisionFinder | 2048 | recompiled | gamecore_sub_0133AFA0, manager_sub_01137D28 | GetCollisionDBEdgeInfo, GetCollisionEdgePointerOtherSideDBData, GetLayoutClass, glBegin, glBlendFunc +12 | $a0: +0x0:R4 +0x4:R4; globals: 01195498:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1122760 | DebugDispManualEdgeCollisionDB | 432 | recompiled | DebugDispManualEdgeCollisionFinder | GetCollisionDBEdgeInfo, GetLayoutClass, kernel_sub_001B7B60, manager_sub_01114C70, manager_sub_01116108 | globals: 001e0f00:& 01195498:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1122910 | DebugDispCollisionFinderKeep | 328 | recompiled | gamecore_sub_0133AFA0 | glBegin, glBlendFunc, glEnable, glEnd, iosDlPopLayer +3 | $a0: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1123d90 | ClipEdgeLinkDBFinder | 216 | decomp_liftd3_01.cpp | gamecore_sub_01391E30, gamecore_sub_01431738 | manager_sub_0111F430, memset | $a0: +0x0:R16; $a2: +0x0:R16; $a3: +0x20:R4 +0x24:R4 |  |
| 11240b0 | DebugDispCollisionDB | 160 | recompiled | gamecore_sub_0133A048, gamecore_sub_0133AFA0 | kernel_sub_001B7B60, manager_sub_011214D0 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1124150 | DebugDispCollisionDBShine | 120 | recompiled | gamecore_sub_0133AFA0 | kernel_sub_001B7B60, manager_sub_011214D0 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11241c8 | dispVuCollisionDB | 264 | recompiled | dispVuColiDispWork | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:R4 +0x4:R4; $a1: +0x0:W16 +0x10:W16 +0x20:W16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11242d0 | DebugDispManualEdgeCollisionFinder | 152 | recompiled | - | DebugDispManualEdgeCollisionDB | $a0: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1124660 | ClipCollisionVariableXZSurfaceCB | 552 | recompiled | - | GetLayoutClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext, manager_sub_01117340 | $a0: +0x0:W4 +0x30:W4 +0x40:W16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1124888 | ClipCollisionFinderWithVariableKeepCB | 488 | decomp_collcensus.cpp | camera_collision_check_with_keep, check_camera_position_float_by_collision, get_camera_collision_data, get_camera_position_float_by_hit_collision | manager_sub_0111C078, manager_sub_0111C810, manager_sub_0111F190 | $a0: +0x0:R16/W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1124a70 | ClipCollisionFinderWithVariableKeep4DCB | 536 | decomp_liftd4_01.cpp | camera_collision_check_with_keep, get_camera_collision_data | ClipCollisionDB, manager_sub_01118B88, manager_sub_0111C078, manager_sub_0111C810 | $a0: +0x0:R16/W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1125190 | GetCollisionDBVertexPoint | 240 | recompiled | CheckCollisionInsideOfSurfaceXZ | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a1: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1125c40 | ClipCollisionDBDoubleSide | 128 | recompiled | gamecore_sub_013FBBB0 | ClipCollisionDB | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1125cc0 | ClipCollisionFinderCallBack | 264 | decomp_liftd7_08.cpp | character_ikcolsearch, charrootdyn_checkIYPlane3DWithPos, charrootdyn_checkIYplane3D, charrootdyn_checkIYplane4D, charrootdyn_checkUnderWater, charrootdyn_clipNoStopCollisionFinder +20 | ClipCollisionDB | $a0: +0x0:W4 +0x4:W4 +0x40:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1125dc8 | ClipCollisionFinder | 224 | decomp_liftd7_08.cpp | GetCharaVCollisionStatus, charrootdyn_checkParentGPPlaneDir, charrootdyn_checkZplane, charrootdyn_findGlobalYGeometryPoint, charrootdyn_findLocalYGeometryPoint, fittingwithrot +17 | ClipCollisionDB | $a0: +0x0:W4 +0x4:W4 +0x40:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1125ea8 | ClipCollisionFinder4D | 232 | recompiled | - | ClipCollisionDB, manager_sub_01118B88 | $a0: +0x0:W4 +0x4:W4 +0x40:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1125f90 | ClipCollisionFinderWithKeep | 256 | decomp_hot44b.cpp | manager_sub_010E9A50, pendulum, xz | manager_sub_01129988 | $a0: +0x0:W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1126090 | ClipCollisionFinderWithKeep4D | 256 | recompiled | manager_sub_010E4590 | manager_sub_011298A8 | $a0: +0x0:W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 7 calls, d9:dv1) |
| 11262d0 | ClipCollisionFinderWithKeep_VWallOnly | 280 | recompiled | manager_sub_010E9578 | manager_sub_0111DDC0 | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x20:R4 +0x24:R4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 229 calls, d9:dv1) |
| 11263e8 | ClipCollisionFinderWithKeepSlopeLimit | 512 | recompiled | charrootdyn_HslidecollisionCB, charrootdyn_getNipPlaneStatus | manager_sub_0111C078, manager_sub_0111C810, manager_sub_0111DDC0 | $a0: +0x0:R16/W16 +0x10:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 11265e8 | ClipCollisionFinderWithKeepSlopeLimitCB | 592 | decomp_collcensus.cpp | charrootdyn_HslidecollisionCB, gamecore_sub_01436E88 | manager_sub_0111C078, manager_sub_0111C810, manager_sub_0111DDC0 | $a0: +0x0:R16/W16 +0x10:W16; $a2: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 1126838 | ClipCollisionFinderFarPlane | 224 | recompiled | - | ClipCollisionDB | $a0: +0x0:W4 +0x4:W4 +0x40:R16; $a1: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1126918 | ClipCollisionFinderFarPlane4D | 248 | recompiled | - | ClipCollisionDB, manager_sub_01118B88 | $a0: +0x0:W4 +0x4:W4 +0x40:R16; $a1: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1126a10 | ClipCollisionFinderFarPlaneWithKeep | 256 | recompiled | - | manager_sub_01129988 | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1126b10 | ClipCollisionFinderFarPlaneWithKeep4D | 256 | recompiled | - | manager_sub_011298A8 | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R16; $a3: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1127088 | ClipCollisionVariableXZSurface | 504 | decomp_liftd4_01.cpp | gamecore_sub_0136E6B0, gamecore_sub_013B0EB8, manager_sub_01104BA0 | GetLayoutClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext, manager_sub_01117798 | $a0: +0x0:W4 +0x30:W4 +0x40:W16 |  |
| 1127280 | ClipCollisionVariableXZSurfaceNoBottomCheck | 504 | recompiled | manager_sub_0113A5A8 | GetLayoutClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext, manager_sub_01117B58 | $a0: +0x0:W4 +0x30:W4 +0x40:W16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1127b28 | GetCollisionEdgePointerVertexPoint | 288 | decomp_liftd9_90.cpp | GetCollisionEdgeLinkDBVWallFinder, ReconstructCollisionEdgeLinkDBVWallFinder, charrootdyn_edgepointstab, manager_sub_010D4C48, manager_sub_010D5308, manager_sub_010E22F8 +3 | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 1127c48 | GetCollisionEdgePointerVertexPoint3 | 312 | decomp_liftd4_01.cpp | GetCollisionEdgeLinkDBVWallFinder, ReconstructCollisionEdgeLinkDBVWallFinder, charrootdyn_getParentedEdgeInfo, manager_sub_010D48B8, pendulum | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 1127e68 | GetCollisionDBEdgeVertexPoint | 288 | decomp_liftd7_04.cpp | gamecore_sub_013B7AE8, gamecore_sub_013B8438 | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4; $a1: +0x0:R4 +0x4:R4 |  |
| 1127f88 | GetCollisionDBEdgeVertexPoint3 | 312 | recompiled | - | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11289f0 | DebugDispCollisionDBColor | 152 | recompiled | gamecore_sub_0133AFA0 | kernel_sub_001B7B60, manager_sub_011214D0 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1128cd0 | getCollisionVirtualWallDispFlag | 16 | recompiled | - | - | globals: 0118ce0c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1129068 | GetColMultiDepVtxInfoVertexNormal | 80 | recompiled | - | - | $a0: +0x174:R4; $a1: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11290b8 | GetColMultiDepVtxInfoVertexNormalWork | 80 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4; $a1: +0x174:R4; $a2: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1129628 | MakeGeometryPointerGlobalInfo | 8 | recompiled | - | - | $a0: +0x30:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1129898 | MakeCollisionPointGlobalInfo | 16 | recompiled | - | - | $a0: +0x30:W4 +0x40:W16; $a1: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 112ca30 | ClipCollisionDirectCallBackOneObj | 2912 | decomp_hot47j.cpp | Check_InVariableXZSurfJudgeFunc_LowerSurface, GetControlCheckStatusFromGP, gamecore_sub_014051D8, gamecore_sub_01431460, gamecore_sub_01434BB8, gamecore_sub_0144B048 +9 | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +6 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 |  |
| 112d590 | ClipCollisionDirectCallBack | 3056 | decomp_liftd7_08.cpp | AI_GetGeometryPointerOfLayout, ClipCollisionDirectExceptObj, ClipCollisionDirectForParticle, GetInitWallFunc, GetRayVolumeUncollisionPoint, GexInitCommonFunc +70 | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +9 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 |  |
| 112e180 | ClipCollisionDirectCallBackObjAry | 3032 | decomp_liftd7_08.cpp | chkEstimatCollisionCore, gamecore_sub_01391E30, gamecore_sub_013CA508 | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +6 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 |  |
| 112ed58 | ClipCollisionDirect | 2808 | decomp_liftd6_02.cpp | ColCont2_Update, DebugTestWideAreaSearch, GetCollisionWideAreaSearch, ReparentCharacterObj, SCRConditoinPoseidonPlayerRayChack, SCRFuncKameSetTemporaryTarget +19 | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +9 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 |  |
| 112f850 | ClipCollisionDirectCallBackObjAryDoubleSide | 3136 | recompiled | - | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +6 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1130490 | ClipCollisionDirectCallBackDoubleSide | 3168 | decomp_liftd9_90.cpp | SCRFuncCheckBoneColAndSendDamage, gamecore_sub_0143F2C8, getHitPosWayCollision, nico_Meet2Main, pathDetectCollision, updateFastLegCtrlWork +1 | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +9 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 |  |
| 11310f0 | ClipCollisionDirectCallBackOneObjDoubleSide | 2912 | recompiled | - | ClipCollisionDB, EntryModifiedManager, GetArbitraryDirectionInverseMatrix, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary +6 | $a0: +0x0:W4 +0x4:W4 +0x40:R16/W16; $a2: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1131c50 | ClipCollisionDirectExceptObj | 48 | recompiled | - | ClipCollisionDirectCallBack | globals: 01131d70:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11328f8 | SetGeometryPointerGlobalState | 24 | decomp_liftd10_00.cpp | FlushLayoutObj, ForceUnparentLayoutObj, GetCharaRootDynamics, SendDamageBoxLetter, SendDamageCylinderLetter, SendDamageLetter +13 | - | $a0: +0x0:W16 +0x10:W16 +0x30:W4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1132970 | SetGeometryPointerGlobalStatePointer | 40 | decomp_liftd2_00.cpp | gamecore_sub_014121B0 | - | $a0: +0x0:W4 +0x4:W4 +0x10:W16 +0x20:W16 +0x30:W4; $a3: +0x0:R16 |  |
| 1132998 | SetGeometryPointerGlobalStatePosPointer | 40 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x10:W16 +0x20:W16 +0x30:W4; $a3: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1132c88 | SetGeometryPointerNodePosStateGlobalRot | 40 | decomp_liftd2_00.cpp | manager_sub_01167C18 | - | $a0: +0x0:W4 +0x4:W4 +0x10:W16 +0x20:W16 +0x30:W4; $a3: +0x0:R16 |  |
| 11333b8 | ClipPoint | 80 | decomp_hot46i.cpp | updateObjectGeo, updateParticleGeo | - | $a0: +0x0:R16; $a1: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16 |  |
| 11340f8 | GetPlaneGlobalYFittingPoint | 72 | recompiled | - | - | $a0: +0x0:W16 +0x4:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4; $a2: +0x0:R16/R4 +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1135358 | GetTurnYAngleFromGlobalDir | 376 | decomp_ctrlturn.cpp | GetCharaVCollisionStatus, charrootdyn_getCharaFront_PlaneTopDifferenceXZAngle, charrootdyn_getGPPlaneInfo, charrootdyn_getGPPlaneInfoByCollisionDBData, charrootdyn_getGPPlaneInfoWithVPlane, charrootdyn_setLandingStatus +6 | iosGetATan2fExact, iosGetStabXZVector | $a0: +0x0:R16; $a1: +0x0:R16 |  |
| 11354d0 | GetTurnYLimitAngleFromGlobalDir | 592 | decomp_liftd4_02.cpp | manager_sub_010CD518 | SetQuaternionByAxisRotateVWithNoRegularize, iosGetATan2fExact, iosGetStabXZVector | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16; globals: 001e0f40:&R 001e0fb0:& |  |
| 1136550 | CheckEllipticalSphereVolumeShapeWithInverseMatrix | 368 | recompiled | - | iosGetCosf | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4; $a3: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11366c0 | CheckEllipticalSphereVolumeShape | 408 | recompiled | CheckPositionInsideOfJudgeVolumeInfo | iosGetCosf, iosInverseMatrix | $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4; globals: 001e0fc0:&R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1136858 | CheckEllipticalCylinderVolumeShapeWithInverseMatrix | 424 | recompiled | - | iosGetCosf | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4; $a3: +0x0:R4 +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1136a00 | CheckEllipticalCylinderVolumeShape | 480 | decomp_liftd8_00.cpp | CheckPositionInsideOfJudgeVolumeInfo | iosGetCosf, iosInverseMatrix | $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4; globals: 001e0fc0:&R |  |
| 1139ca0 | create_particle_texture_and_get_id | 296 | decomp_walk.cpp | add_particle_segment | - | $a3: +0x0:W4 +0x4:W4 +0x8:W4; globals: 001e0ed4:R 0118d048:& 0118d050:&R 0118d05c:&R |  |
| 113d988 | disp_particle_emitter | 624 | sotc_worker.cpp | manager_sub_01140ED0 | get_particle_parameter_blend_disp, get_world_matrixSR, manager_sub_0113DBF8, manager_sub_0113DEA0, manager_sub_0113E218 +6 | $a0: +0x118:R4 +0x16c:R4 +0x198:R4; $a1: +0xb0:R4 +0xd4:R4 +0x120:R4 |  |
| 113f220 | disp_particle_manager | 336 | sotc_worker.cpp | gamecore_sub_01345110 | getDrawScreenHeight, getDrawScreenWidth, iosUnitMatrix, iosgluGetCameraContext | globals: 0118d000:&RW 0118d010:RW 0118d014:W 0118d020:RW 0118d030:RW 0118d034:W |  |
| 113f378 | get_top_emitter_list_from_particle_geodisp | 48 | sotc_worker.cpp | doit_particle_manager, manager_sub_01139F10, manager_sub_01140748 | GetGeoDispClass | - |  |
| 113f3a8 | create_and_boot_particle_geodisp | 176 | sotc_worker.cpp | manager_sub_01140748 | GetGeoDispClass, get_fast_get_malloc_partition, initParticleObject, manager_sub_01140B70 | - |  |
| 113f458 | check_free_particle_geodisp | 72 | recompiled | - | GetGeoDispClass, isysRequestDeleteObj, manager_sub_0113A7B8 | $a0: +0x0:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113f4a0 | auto_end_check_and_check_free_particle_geodisp | 96 | sotc_worker.cpp | doit_particle_manager | GetGeoDispClass, isysRequestDeleteObj | $a0: +0x0:R4/W4 |  |
| 113f500 | doit_particle_geodisp | 48 | sotc_worker.cpp | doit_particle_manager | GetGeoDispClass, manager_sub_0113AAE0 | - |  |
| 113f530 | clip_particle_geodisp | 208 | sotc_worker.cpp | - | GetGeoDispClass, getZ, iosgluGetCameraContext, manager_sub_01140C10 | - |  |
| 113f600 | calc_light_particle_geodisp | 168 | recompiled | - | GetGeoDispClass, SetGeoDispClassLightColMan, manager_sub_01140C80 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113f6a8 | disp_particle_geodisp | 88 | sotc_worker.cpp | - | GetGeoDispClass, manager_sub_0113ACB8 | - |  |
| 113fa88 | get_main_particle_effect_lod_from_sheet | 8 | decomp_liftd7_08.cpp | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8, get_particle_camera_follow_attr_from_effect_id, is_loop_particle | - | $a0: +0x48:R4 |  |
| 113fa90 | get_sub_particle_effect_lod_from_sheet | 8 | decomp_liftd7_08.cpp | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8 | - | $a0: +0x4c:R4 |  |
| 113fa98 | get_top_particle_effect_from_lod | 8 | decomp_liftd7_08.cpp | gamecore_sub_013FB270, gamecore_sub_013FC148, gamecore_sub_013FCCE0, gamecore_sub_013FCFC8, is_loop_particle | - | $a0: +0xd4:R4 |  |
| 113faa0 | get_particle_effect_from_lod | 32 | recompiled | - | - | $a0: +0xd4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fac0 | reinit_particle_effect_lod | 24 | recompiled | - | init_particle_effect_lod | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fad8 | quit_particle_texture_user_by_id | 120 | recompiled | - | - | globals: 0118cfd0:& 0118d048:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fb50 | set_particle_texture_by_id | 56 | recompiled | - | texTransRequest | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fb88 | getPtclDisp | 16 | recompiled | - | - | globals: 0118cff0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fb98 | get_particle_printf_disp_level | 16 | recompiled | - | - | globals: 0118cfd8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fbb8 | is_proper_particle_texture_id | 56 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 113fbf0 | create_particle_effect_lod | 80 | sotc_worker.cpp | create_and_init_particle_effect_lod | iosUnitMatrix, malloc_from_root_partition | globals: 01195840:& |  |
| 113fc40 | set_except_npe_file_data_to_particle_effect_lod | 88 | sotc_worker.cpp | create_and_init_particle_effect_lod | - | $a0: +0xd0:R4 +0xd4:R4 +0xd8:W4 |  |
| 113fc98 | init_particle_effect_lod | 304 | sotc_worker.cpp | create_and_init_particle_effect_lod, reinit_particle_effect_lod | get_polygon_scale_rotation_matrix, get_radiate_range_coefficient | $a0: +0x50:R4 +0x54:R4 +0x7c:R4 +0x80:R4 +0xd0:R4 +0xd4:R4 +0x120:W4 +0x124:W4 +0x128:W4 +0x12c:W4 +0x130:W4 + |  |
| 113fdc8 | is_emitter_need_disp | 88 | sotc_worker.cpp | manager_sub_01140C10 | manager_sub_0113B1D8 | $a0: +0x124:R4 +0x140:R4 |  |
| 113fe20 | check_free_particle_effect_lod | 256 | recompiled | - | iosFree | $a0: +0xd0:R4 +0xd4:R4; globals: 0118cfd0:& 0118d048:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11401f8 | constructor_particle_effect_lod_except_npe | 48 | sotc_worker.cpp | manager_sub_01144E90 | iosUnitMatrix | $a0: +0xd0:W4 +0xd4:W4 +0xd8:W4 +0xdc:W4 |  |
| 1140228 | add_particle_texture_user | 16 | recompiled | - | - | $a0: +0x4:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1140238 | create_and_init_particle_effect_lod | 392 | sotc_worker.cpp | manager_sub_011405B8 | create_particle_effect_lod, init_particle_effect_lod, iosFree, set_except_npe_file_data_to_particle_effect_lod, set_npe_file_data_to_particle_effect_lod | globals: 0118cfd0:& 0118d048:& |  |
| 11403c0 | get_particle_lod_base_size | 56 | recompiled | manager_sub_01141150 | GetScreenRadiusParticle, iosgluGetCameraContext | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11403f8 | set_act_particle_lod_select_flag | 16 | recompiled | - | - | globals: 0118d040:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1140408 | set_disp_particle_collisin_debug_flag | 16 | recompiled | - | - | globals: 0118d044:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1140548 | get_particle_geodispobj_group | 16 | recompiled | - | - | globals: 01216e00:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1140558 | get_set_limit_gl_draw_particle_flag | 16 | recompiled | - | - | globals: 0118cff8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1140568 | create_particle_texture | 80 | recompiled | - | - | globals: 001e0ed4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11414e8 | ClipCollisionDirectForParticle | 48 | recompiled | - | ClipCollisionDirectCallBack | globals: 01123520:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11426a8 | set_npe_file_data_to_particle_effect_lod | 10072 | sotc_worker.cpp | create_and_init_particle_effect_lod | manager_sub_01144E00, manager_sub_01144E38, manager_sub_01144E50, manager_sub_01144E58, manager_sub_01144E70 +3 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 + |  |
| 1145ba8 | get_particle_parameter_blend_disp | 200 | recompiled | disp_particle_emitter | - | $a0: +0x0:W4; $a1: +0xd4:R4; $a2: +0xd0:R4 +0xd4:R4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 7268 calls, d9:dv1) |
| 114a5e8 | mouseDrawCursor | 440 | recompiled | - | gsReg, gsRegBegin, gsRegEnd, iosGetSecondFromTCount, iosGetTCountExtension +1 | globals: 0118d634:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 114ad98 | mouseDispCursor | 48 | recompiled | windowSetMouseStatus | iosGetSecondFromTCount, iosGetTCountExtension | globals: 0118d634:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 114fd00 | spriteDraw | 376 | recompiled | mouseDrawCursor | gsReg | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 114fe78 | spriteDrawUVSensitive | 352 | recompiled | manager_sub_01152680, windowSprite | gsReg | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 114ffd8 | spriteDrawNoTex | 288 | recompiled | manager_sub_01152680, windowSpriteNoTex | gsReg | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1150a38 | windowSpriteMenuArrow | 576 | recompiled | manager_sub_0114F738 | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +3 | $a0: +0x198:R4 +0x19c:R4 +0x26c:R4 +0x270:R4; globals: 0118fa90:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1153b40 | windowDraw | 1344 | recompiled | gamecore_sub_013463D0 | getDrawScreenHeight, getDrawScreenWidth, iosDlGetWorkBufId, iosDlLockObj, iosDlPopLayer +10 | globals: 0118fa90:& 0118fa94:R 0118faa0:&RW 0118faa4:R 0118fab4:R 0118fab8:R 0118fad4:R | D11: lifted, exact (trace check), slower in both liftd11 A/Bs: cut |
| 1155b60 | windowDisplayMode | 16 | recompiled | - | - | globals: 0118fad4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1155b70 | windowFontColor | 40 | recompiled | manager_sub_01100090, manager_sub_0114D5F8, manager_sub_0114EE10, manager_sub_0114F8C8, manager_sub_0114FA18 | - | $a0: +0x1f0:W8 +0x1f8:W8 +0x200:W8 +0x208:W8; $a1: +0x0:R8 +0x8:R8 +0x10:R8 +0x18:R8 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1155b98 | windowSpriteNoTex | 360 | recompiled | manager_sub_0114D5F8, manager_sub_0114EE10, manager_sub_0114F588, manager_sub_0114F8C8, windowPrintfBackBoard | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +1 | $a0: +0x198:R4 +0x19c:R4 +0x26c:R4 +0x270:R4; $a1: +0x0:R4 +0x7:R4 +0x8:R4 +0xf:R4; $a2: +0x0:R4 +0x4:R4 +0x8: | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1155d00 | windowSprite | 472 | recompiled | - | gsReg, gsRegBegin, gsRegEnd, iosDlPopLayer, iosDlPushLayer +3 | $a0: +0x198:R4 +0x19c:R4 +0x26c:R4 +0x270:R4; $a1: +0x0:R4 +0x7:R4 +0x8:R4 +0xf:R4; $a2: +0x0:R4 +0x4:R4 +0x8: | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1156338 | windowSetTex | 16 | decomp_liftd7_03.cpp | loadCdWinTexFile | - | globals: 0118fa90:W |  |
| 1159318 | InitExcelAutoModFile | 376 | recompiled | - | LoaderSysFClose, iosJumpRecoverPoint, iosSPrintf, manager_sub_011590D8 | globals: 0121d72c:&RW 0121d730:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 115ba60 | sheetFsGetStageTexSegIndex | 88 | decomp_liftd6_01.cpp | sheetMgrStageTexLoad, sheetMgrStageTexUnload | - | globals: 0121ec00:R |  |
| 115bab8 | sheetFsAddStageTexSegIndex | 32 | decomp_liftd6_01.cpp | sheetMgrStageTexLoad | - | globals: 0121ebf8:R |  |
| 115bad8 | sheetFsDecStageTexSegIndex | 32 | decomp_liftd6_01.cpp | sheetMgrStageTexUnload | - | globals: 0121ebf8:R |  |
| 115baf8 | sheetFsExistTexSegIndexCallBack | 160 | recompiled | manager_sub_0115E4C0 | - | globals: 0121ebf8:&R 0121ebfc:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 115bb98 | sheetFsSetStageTexSegTexSegDef | 24 | decomp_liftd6_01.cpp | manager_sub_0115E268, manager_sub_0115E4C0 | - | globals: 0121ebf8:R |  |
| 115bbb0 | sheetFsGetStageTexSegTexSegDef | 24 | decomp_liftd7_04.cpp | manager_sub_0115E268 | - | globals: 0121ebf8:R |  |
| 115c008 | sheetFsLoadStageTex | 416 | decomp_liftd4_02.cpp | sheetFileLoadStageTex | iosCdStStart, iosCdStStop, iosJumpRecoverPoint, manager_sub_0115C2C0, strcmp | globals: 0118fe14:R 011966a0:& 011966b0:&R 011966b3:R 011966b4:R 011966b5:R 0121ec18:R 0121ec20:R |  |
| 115d0d8 | sheetFileLoadStageTex | 56 | decomp_liftd6_01.cpp | manager_sub_0115E4C0 | sheetFsLoadStageTex | globals: 0115d158:& 0121ec5c:W 0121ec60:W |  |
| 115ed10 | sheetMgrAddSegLoadList | 24 | decomp_liftd4_02.cpp | manager_sub_0115C500 | - | $a0: +0x4c:W4; globals: 0121ec88:&RW |  |
| 115ed28 | sheetMgrGetSegLoadHeader | 16 | decomp_liftd4_02.cpp | manager_sub_0115C500 | - | globals: 0121ec88:R |  |
| 115f340 | sheetMgrStageTexLoad | 288 | decomp_liftd6_01.cpp | gamecore_sub_0135E468 | SignalSema, WaitSema, iosJumpRecoverPoint, sheetFsAddStageTexSegIndex, sheetFsGetStageTexSegIndex | globals: 0121eca0:R 012228a8:&RW 012228ac:W 012228b0:& 012228c0:&W 012228c4:RW |  |
| 115f460 | sheetMgrStageTexUnload | 288 | decomp_liftd6_01.cpp | gamecore_sub_0135E4F0 | SignalSema, WaitSema, iosJumpRecoverPoint, sheetFsDecStageTexSegIndex, sheetFsGetStageTexSegIndex | globals: 0121eca0:R 012228a8:&RW 012228ac:W 012228b0:& 012228b8:&W 012228bc:RW |  |
| 115fec0 | sheetExecMake | 176 | recompiled | - | nicosysdExec, nicosysdSendDataStart, nicosysdSendDataWrite, strlen | globals: 001e0c88:& 0115ab80:& 01196a48:&R 01196a4f:R 01196a50:R 01196a51:R 01196a52:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 115ff70 | sheetExecCommit | 176 | recompiled | - | nicosysdExec, nicosysdSendDataStart, nicosysdSendDataWrite, strlen | globals: 001e0c88:& 0115ab80:& 01196a70:&R 01196a77:R 01196a78:R 01196a79:R 01196a7a:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160020 | sheetExecUpdate | 152 | recompiled | - | nicosysdExec, nicosysdSendDataStart, nicosysdSendDataWrite, strlen | globals: 001e0c88:& 0115ab80:& 01196a58:&R 01196a5f:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11600b8 | sheetExecClean | 168 | recompiled | - | nicosysdExec, nicosysdSendDataStart, nicosysdSendDataWrite, strlen | globals: 001e0c88:& 0115ab80:& 01196a60:&R 01196a67:R 01196a68:R 01196a69:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11601c0 | toggleDebPcWinWindow | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160570 | update_camera_context | 1424 | decomp_liftd8_01.cpp | doit_cameraman_manager | get_camera_direction, get_camera_position, get_camera_up_direction, get_clip_matrix, get_clip_matrix128 +16 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 +0x140:R16/W16 +0x150:R16/W16 +0x160: |  |
| 1160b00 | GetScreenRadius | 288 | decomp_liftd2_01.cpp | AI_OptFloatFrameSample, ViewIntervalClip, gamecore_sub_013E5FF0, manager_sub_010B2318 | - | $a1: +0x0:R16 |  |
| 1160c58 | SetDefaultViewContext | 112 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0x10:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160cc8 | SetViewContext | 64 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160d08 | create_camera_context | 64 | recompiled | - | constructor_camera_context, iosMallocAlign | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160d48 | release_camera_context | 40 | recompiled | - | iosFree | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160d70 | constructor_camera_context | 152 | decomp_liftd7_05.cpp | create_camera_context, init_camera_context_manager, init_cameraman_manager | iosGetCurrentDC | - |  |
| 1160e08 | GetScreenRadiusParticle | 120 | recompiled | get_particle_lod_base_size | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160e80 | init_camera_context_manager | 64 | sotc_worker.cpp | initmanager | constructor_camera_context | globals: 0118fe4c:W 0122a8f0:W 0122a970:& |  |
| 1160ec0 | push_camera_context | 88 | decomp_liftd7_05.cpp | init_cameraman_manager | SetVSMatrix | globals: 0118fe4c:&RW |  |
| 1160f18 | pop_camera_context | 80 | recompiled | - | SetVSMatrix | globals: 0118fe4c:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1160f68 | get_cur_camera_context | 32 | sotc_worker.cpp | gluGetCameraSpaceXYZ, gluGetLookAtPosition, gluUpdateCameraContext, iosgluGetCameraContext | - | globals: 0118fe4c:R |  |
| 1162df0 | set_camera_obj_screen_size | 24 | decomp_liftd3_01.cpp | setAspect | set_screen_size | $a0: +0x250:R4 |  |
| 1162e08 | dynamic_cast_to_camera_obj | 40 | decomp_layout.cpp | CallLayoutObjDebugResetFunc, ChangeLayoutObjCollisionResourceAttribute, CheckLayoutObjChainExist, CreateLayoutObj, CreateLayoutObjVU0Secure, FlushCollisionWork +25 | - | $a0: +0x8:R1 |  |
| 1162f08 | get_camera_obj_layout_class | 8 | decomp_layout.cpp | CallLayoutObjDebugResetFunc, ChangeLayoutObjCollisionResourceAttribute, CheckLayoutObjChainExist, CreateLayoutObj, CreateLayoutObjVU0Secure, FlushCollisionWork +27 | - | - |  |
| 1162f10 | get_clip_screen | 8 | decomp_liftd2_01.cpp | gamecore_sub_013447C0, gamecore_sub_013451C0, gamecore_sub_0141BE60, get_camera_parameter_cameraman, set_camera_parameter_and_update_cameraman, set_camera_parameter_cameraman +1 | - | $a0: +0x250:R4 |  |
| 1163170 | disp_camera_region | 728 | recompiled | disp_object_animation_camera_region_group | DEBUG_disp_region_object, GetLayoutClass, is_have_region_camera_region, kernel_sub_001B7B60 | $a0: +0x4:R4 +0x10:R4 +0x14:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1164280 | create_clip_screen | 176 | decomp_liftd2_01.cpp | create_camera_obj | iosMallocAlign, set_scissor_screen_size, set_screen_size, set_view_angle, update_clip_screen | - |  |
| 1164330 | check_free_clip_screen | 56 | recompiled | manager_sub_01162F78 | iosFree | $a0: +0x0:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1164368 | set_screen_size | 256 | decomp_liftd7_05.cpp | create_clip_screen, set_camera_obj_screen_size | getDispScreenHeight, getDispScreenWidth, getDrawScreenHeight | $a0: +0x0:R4/W4 +0x4:W4 +0x8:W4; globals: 0118fe68:&W |  |
| 1164468 | set_screen_wide_and_progressive | 96 | decomp_liftd7_05.cpp | setAspect | - | globals: 0118fe64:W |  |
| 11644c8 | set_view_angle | 8 | decomp_liftd4_02.cpp | create_clip_screen, set_camera_parameter_and_update_cameraman, set_camera_parameter_cameraman | - | $a0: +0xc:W4 |  |
| 11644d0 | set_scissor_screen_size | 32 | decomp_liftd2_01.cpp | create_clip_screen, gamecore_sub_013447C0, gamecore_sub_013451C0 | - | - |  |
| 11644f0 | get_screen_width | 8 | decomp_liftd2_01.cpp | update_camera_context | - | $a0: +0x0:R4 |  |
| 11644f8 | get_screen_height | 8 | decomp_liftd4_02.cpp | update_camera_context | - | $a0: +0x4:R4 |  |
| 1164530 | get_view_angle | 8 | decomp_liftd2_01.cpp | gamecore_sub_0141BE60, get_camera_parameter_cameraman | - | $a0: +0xc:R4 |  |
| 1164538 | get_dist_from_eye_to_screen | 32 | decomp_liftd4_02.cpp | update_camera_context | manager_sub_01164858 | $a0: +0x4:R4 +0xc:R4 |  |
| 1164558 | get_near_clip_z_clip_screen | 24 | decomp_liftd4_02.cpp | update_camera_context | - | - |  |
| 1164570 | get_far_clip_z_clip_screen | 24 | decomp_liftd4_02.cpp | update_camera_context | - | - |  |
| 1164588 | get_z_buf_min_clip_screen | 16 | decomp_liftd4_02.cpp | update_camera_context | - | - |  |
| 1164598 | get_z_buf_max_clip_screen | 24 | decomp_liftd4_02.cpp | update_camera_context | - | - |  |
| 11645b0 | create_original_clip_matrix | 352 | recompiled | - | iosGetTanf, iosUnitMatrix | $a0: +0x0:W4 +0x14:W4 +0x28:W4 +0x2c:W4 +0x38:W4 +0x3c:W4; globals: 0118fe64:R 0118fe68:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1164710 | update_clip_screen | 88 | decomp_liftd3_01.cpp | create_clip_screen, gamecore_sub_013447C0, gamecore_sub_013451C0, update_camera_obj | manager_sub_01164858, manager_sub_011648D0, manager_sub_011649C0, manager_sub_01164A10 | $a0: +0x4:R4 +0xc:R4 |  |
| 1164848 | get_screen_size_scale_screen_matrix | 16 | recompiled | - | - | globals: 0118fe6c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1164c70 | get_clip_matrix | 8 | decomp_liftd1_03.cpp | update_camera_context, update_transform_matrix_set | - | - |  |
| 1164c78 | get_clip_matrix128 | 8 | sotc_prof.cpp | update_camera_context | - | - |  |
| 1164c80 | get_clip_matrix256 | 8 | sotc_prof.cpp | update_camera_context | - | - |  |
| 1164c88 | get_clip_matrix_fovignore | 8 | sotc_prof.cpp | update_camera_context | - | - |  |
| 1164c90 | get_screen_matrix | 8 | decomp_liftd1_03.cpp | update_camera_context, update_transform_matrix_set | - | - |  |
| 1164c98 | get_screen_matrix128 | 8 | decomp_liftd1_03.cpp | update_camera_context, update_transform_matrix_set | - | - |  |
| 1164ca0 | get_screen_matrix256 | 8 | decomp_liftd1_03.cpp | update_camera_context, update_transform_matrix_set | - | - |  |
| 1164ca8 | get_scissor_clip_matrix | 16 | decomp_liftd4_02.cpp | update_camera_context, update_transform_matrix_set | - | - |  |
| 11669b0 | SendVolumeShapeDamageLetter | 496 | recompiled | manager_sub_0110C918 | SetGeometryPointerGlobalState, create_letter, init_letter, memcpy, memset +1 | globals: 01165c88:& 0118fe74:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 11670a0 | getDamageEnableTargetLayoutClass | 104 | recompiled | - | GetCharacterClass, GetScriptLayoutClass | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1169a68 | SequenceKickWindVolumeShape | 184 | recompiled | StartAnimationSubObjInfinitePart | CreateSubLinkObjByGeometryPointer, GetGeometryPointerObject, SetSubLinkObjDeleteFuncInfo, boot_wind_emitter_by_judge_volume, isysRequestDeleteObj | globals: 01169b20:& 01169b58:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 116a080 | getLayoutDataHeader | 24 | decomp_liftd8_01.cpp | initISysPathObj | - | $a0: +0x28:R4 |  |
| 116a098 | getLayoutPointData | 60 | decomp_liftd2_01.cpp | gamecore_sub_013EF378, getSmallAnimalSaveTypePos, initISysPathObj | - | $a0: +0x0:R4 |  |
| 116a310 | qsort | 2544 | recompiled | gamecore_sub_01415A48, manager_sub_0113ACB8, qsort | qsort | $a0: +0x0:R1/R8/W1/W8 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 128 calls, d9:dv1) |
| 133bcd0 | DebugGetGlobalDiffWith2ndController | 536 | recompiled | DebugGetCameraWithManual, DebugGlobalMoveWith2ndController | GetLayoutClass, GetMoveVectorOnWV, get_main_cameraman_camera_obj, iosPadGetXZInputL, memset | globals: 0118d680:& 0118e618:R 0118e66f:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 133bee8 | DebugGlobalMoveWith1stController | 536 | recompiled | gamecore_sub_0141E400 | GetLayoutClass, GetMoveVectorOnWV, get_main_cameraman_camera_obj, iosPadGetXZInputR, memset | $a0: +0x0:W16; globals: 001e0db0:& 014657c0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 133c100 | DebugPrintf3D_Context | 112 | recompiled | - | kprintfContextCentering, vsprintf | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 133c230 | disphalfarc | 728 | recompiled | gamecore_sub_0133CBE8 | glBegin, glBlendFunc, glColor4f, glColor4fv, glEnd +5 | globals: 001e0fd0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 133c508 | disphalfarcpillar | 1760 | recompiled | - | glBegin, glBlendFunc, glColor4fv, glEnd, glVertex3f +5 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1340fc0 | DebugGlobalMoveWith2ndController | 112 | recompiled | gamecore_sub_0141E400, gamecore_sub_01441A90 | DebugGetGlobalDiffWith2ndController | $a0: +0x0:W16; globals: 014657c0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341030 | DebugDispItem | 168 | recompiled | - | iosSetKPrintfColorContext, kprintfContext | globals: 01467528:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13410d8 | DebugDispPosition | 88 | recompiled | - | glColor4f, kernel_sub_001B7B60 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341130 | DebugDispPositionColor | 80 | recompiled | - | glColor4f, kernel_sub_001B7B60 | $a1: +0x0:R4 +0x4:R4 +0xc:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341180 | DebugDispTargetStatusForce | 32 | recompiled | - | GetCharacterClass | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13411a0 | DebugPrintf3D_InitContext | 56 | recompiled | - | iosInitKPrintfContext3D | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341708 | getDispCollisionPerfFlag | 16 | recompiled | - | - | globals: 014653fc:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341758 | is_disp_debug_object_info | 16 | recompiled | - | - | globals: 01465408:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341768 | getCollisionDispMode | 16 | recompiled | - | - | globals: 0146540c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341808 | dispgpinfo | 264 | recompiled | - | GetGeometryPointerPosition, glColor4f, iosInitKPrintfContext3D, kernel_sub_001B7B60, kprintfContextCentering | globals: 01467528:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1341c08 | getDispGpsMarker | 16 | recompiled | - | - | globals: 01465598:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342688 | GAMEUTIL_disp_col_plane_info_debug | 480 | recompiled | GAMEUTIL_disp_col_plane_info_filter_debug | fptodp, kColor, kPrintf | $a0: +0x8:R4 +0x20:R4 +0x24:R4 +0x28:R4 +0x30:R4 +0x34:R4 +0x38:R4 +0x50:R4; globals: 01467b48:& 01467b58:& 01 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342b80 | GAMEUTIL_set_sight_on_angle_vel_ratio | 16 | recompiled | - | - | globals: 01465618:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342b90 | GAMEUTIL_get_sight_on_angle_vel_ratio_int | 16 | recompiled | - | - | globals: 01465618:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342ba0 | GAMEUTIL_get_sight_on_angle_vel_ratio_float | 176 | decomp_liftd7_08.cpp | gamecore_sub_013B3050 | GAMEUTIL_get_dist_ratio, GAMEUTIL_get_interp_value | globals: 01465618:R |  |
| 1342cb0 | GAMEUTIL_set_game_camera_free_mode_angle_vel_ratio | 16 | recompiled | - | - | globals: 01465624:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342cc0 | GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_int | 16 | recompiled | - | - | globals: 01465624:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1342cd0 | GAMEUTIL_get_game_camera_free_mode_angle_vel_ratio_float | 176 | decomp_liftd10_00.cpp | gamecore_sub_01422E90 | GAMEUTIL_get_dist_ratio, GAMEUTIL_get_interp_value | globals: 01465624:R |  |
| 1342e40 | GAMEUTIL_get_obj_from_stagelayout_sheet | 96 | recompiled | CAM_get_base_space_posi_and_axisXYZ, CAM_get_parent_obj_by_base_space | - | $a0: +0x0:R4; globals: 001e0ed4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1343c58 | GAMEUTIL_constructor_move_angle | 120 | decomp_liftd7_05.cpp | constructor_nico_yaw_pitch_cntrl | GAMEUTIL_reset_move_angle | - |  |
| 1343cd0 | GAMEUTIL_reset_move_angle | 8 | decomp_liftd7_05.cpp | GAMEUTIL_constructor_move_angle | - | $a0: +0x0:W4 |  |
| 1343cd8 | GAMEUTIL_set_parameter_move_angle | 64 | decomp_liftd7_06.cpp | set_offset_pitch_parameter_nico_yaw_pitch_cntrl, set_offset_yaw_parameter_nico_yaw_pitch_cntrl | - | $a0: +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 |  |
| 1343d18 | GAMEUTIL_begin_move_angle | 8 | decomp_liftd7_06.cpp | begin_nico_yaw_pitch_cntrl | - | $a0: +0x0:W4 |  |
| 1343d20 | GAMEUTIL_update_and_get_move_angle | 240 | decomp_liftd7_06.cpp | gamecore_sub_01347CF0 | GAMEUTIL_get_interp_value_ratio_pow, GAMEUTIL_get_new_velocity, GAMEUTIL_get_velocity_by_diff_move | $a0: +0x0:R4/W4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 |  |
| 1343e10 | GAMEUTIL_get_cur_velocity_move_angle | 8 | decomp_liftd7_05.cpp | gamecore_sub_01346818, gamecore_sub_013469D0 | - | $a0: +0x0:R4 |  |
| 1343ef0 | GAMEUTIL_set_enable_side_change_angle_look_at_vs_offset | 8 | decomp_liftd2_01.cpp | gamecore_sub_01417ED8 | - | $a0: +0x50:W4 |  |
| 13440b8 | GAMEUTIL_disp_col_plane_info_filter_debug | 248 | recompiled | - | GAMEUTIL_disp_col_plane_info_debug, fptodp, kPrintf | $a0: +0x64:R4 +0x6c:R4 +0x70:R4 +0x74:R4; globals: 01467bb8:& 01467bd8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1344688 | getSightOnAngleVelRatio | 16 | recompiled | - | - | globals: 01465618:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13446b8 | getFreeModeRevAngleVelRatio | 16 | decomp_liftd7_08.cpp | gamecore_sub_013AE848, optionMenuParamSetDefault, optionMenuParamSetLoadData | - | globals: 01465624:& |  |
| 1345190 | getCullingScreenSize | 16 | recompiled | - | - | globals: 014659d0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13451a0 | getScissoringScreenSize | 16 | recompiled | - | - | globals: 014659d4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13476e0 | get_max_min_clip_base_yaw_nico_yaw_pitch_cntrl | 224 | recompiled | - | GAMEUTIL_increase_yaw, GAMEUTIL_increase_yaw_limit_clip, gamecore_sub_01347E40, gamecore_sub_01347EE8 | $a0: +0x104:R4 +0x108:R4 +0x10c:R4 +0x110:R4 +0x128:R4 +0x12c:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13477c0 | get_max_min_clip_base_pitch_nico_yaw_pitch_cntrl | 104 | recompiled | - | GAMEUTIL_increase_pitch_limit_clip, gamecore_sub_01347F88, gamecore_sub_01347FC8 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1348398 | updateVuColiDispWork | 208 | recompiled | gamecore_sub_01341E80 | GetCollisionDB, GetLayoutClass, get_cur_camera_target, get_main_cameraman, glColor4f +1 | globals: 011df410:&R 011df414:R 011df418:& 011df42c:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1348468 | dispVuColiDispWork | 432 | recompiled | gamecore_sub_01341E80 | dispVuCollisionDB, iosDlPopLayer, iosDlPushLayer, iosUnitMatrix, memset +8 | globals: 011df410:& 011df418:&R 011df41c:R 011df424:R 011df42c:R 01467e00:&R 01467e08:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1348618 | initVuColiDispWork | 64 | recompiled | gamecore_sub_01341E80 | InitCollisionFinder | globals: 001e0c88:& 011df410:&W 011df414:W 011df418:& 011df42c:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134bb00 | DisplayCaptionTable | 248 | recompiled | - | SetScriptObjDeleteCallBackFunc, gcCreateScriptObj, iosMallocAlign | globals: 001e0c88:& 0134b6f8:& 0134c040:& 01465b34:&RW 01465b38:&R 01467e68:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134bbf8 | DisplayCaptionTableChild | 288 | decomp_liftd8_01.cpp | gamecore_sub_0134A8B8, gamecore_sub_013695A0 | SetScriptObjDeleteCallBackFunc, _gcCreateChildScriptObj, gcCreateScriptObj, iosMallocAlign | globals: 001e0c88:& 0134b6f8:& 0134c040:& 01465b34:&RW 01465b38:&RW 01467e80:& |  |
| 134bd18 | DisplayCaptionTableBlock | 336 | decomp_liftd8_01.cpp | gamecore_sub_0145A678 | SetScriptObjDeleteCallBackFunc, _gcCreateChildScriptObj, gcCreateScriptObj, iosMallocAlign | globals: 001e0c88:& 0134b6f8:& 0134c040:& 01465b34:&RW 01465b38:&RW 01467ea8:& |  |
| 134be68 | DisplayCaptionTableBlockWithDlLayer | 288 | recompiled | st_gameflow | SetScriptObjDeleteCallBackFunc, gcCreateScriptObj, iosMallocAlign | globals: 001e0c88:& 0134b6f8:& 0134c040:& 01465b34:&RW 01465b38:&R 01467ea8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134c608 | deleteSeamlessCommonLayout | 104 | recompiled | - | stagemanager_unloadstage | $a0: +0xc:R4 +0x10:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134c670 | initSeamlessCommonLayout | 104 | recompiled | - | - | $a0: +0xc:R4 +0x10:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134df50 | scrSetFadeColorBlack | 24 | recompiled | - | setFadeColor | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 134df68 | scrSetFadeColorWhite | 24 | recompiled | - | setFadeColor | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13535b0 | gStatGObjDispOn | 16 | recompiled | - | - | $a0: +0x10:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13535c0 | gStatGObjDispOff | 24 | recompiled | - | - | $a0: +0x10:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13535d8 | gStatGObjDispChk | 16 | decomp_liftd6_01.cpp | gamecore_sub_01360068 | - | $a0: +0x10:R4 |  |
| 13535e8 | gStatGobjDispGflagChk | 80 | decomp_liftd6_01.cpp | SCRConditionGameFlagCheckByGameObj, chkGameFlagForGameObj, gamecore_sub_01360068 | _gameFlagChk | $a0: +0x8:R4 +0x10:R4 |  |
| 1353638 | gStatGobjDispGflagSet | 72 | recompiled | SCRFuncSetGameFlagByGameObj, nico_getOmakeItem, nico_setOmakeItemFlag, setGameFlagForGameObj | _gameFlagCls, _gameFlagSet | $a0: +0x8:R4 +0x10:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1353680 | gStatGobjDispGflagCls | 80 | recompiled | - | _gameFlagCls, _gameFlagSet | $a0: +0x8:R4 +0x10:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1353e00 | gstatBootMcSetExistNicoData | 24 | decomp_liftd7_07.cpp | st_mcBootChk | - | globals: 01465dd8:W 01465ddc:W |  |
| 1353ef8 | gstatBootMcSetExistIcoData | 16 | decomp_liftd7_07.cpp | gamecore_sub_0136F2A0 | - | globals: 01465de0:W |  |
| 1356630 | getStageLayoutGeometryPointer | 72 | decomp_liftd6_01.cpp | gamecore_sub_01360068, gamecore_sub_013EF378, nico_createPlayerParachute | SetGeometryPointerNodePosState, gamecore_sub_013557D8 | globals: 001e0f40:& |  |
| 135da20 | SetSeamlessZFogDist | 168 | recompiled | - | - | globals: 011e3eec:W 01465e5c:&RW 01465e60:RW 01465ec0:R 01465ef0:R 01465ef4:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 135dac8 | getSeamlessStageHighModelDispDist | 16 | recompiled | - | - | globals: 01465e54:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 135dad8 | getSeamlessStageMiddleModelDispDist | 16 | recompiled | - | - | globals: 01465e58:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 135dae8 | getSeamlessStageHighModelDispDistDefault | 16 | decomp_liftd6_01.cpp | st_opflow | - | globals: 01465e64:& |  |
| 135daf8 | getSeamlessStageMiddleModelDispDistDefault | 16 | decomp_liftd6_01.cpp | st_opflow | - | globals: 01465e68:& |  |
| 135dd68 | getStageLayout | 208 | recompiled | - | - | globals: 01465e80:& 01465e88:R 01465ec8:R 01465ecc:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1360508 | initlayout | 992 | decomp_liftd6_02.cpp | _initstage, initstage | GetLayoutClass, gStatSetGObjAdrs, gamecore_sub_01360068, unloadObj | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R 001e0fd0:&R 014663a8:R |  |
| 1365230 | GetStageContext | 544 | decomp_liftd8_01.cpp | GetStageAreaNumber, GetStageAreaNumberByPos, GetStageCenter, GetStageContextByPos, GetStageName, GetStageNumberByPos +2 | iosSPrintf | $a0: +0x0:W2 +0x2:W1 +0x4:R4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:R4/W4 +0x1c:R4/W4 +0x20:W4 +0x24:W4 +0x30 |  |
| 1365cc0 | GetStageContextByPos | 72 | decomp_liftd6_01.cpp | st_stageboundary | GetStageContext | $a0: +0x4:W4 +0x20:R4; globals: 014663a8:R 014663ac:R |  |
| 1365d08 | GetWorldLayoutSheetSegHeader | 16 | decomp_liftd3_02.cpp | gamecore_sub_01367778, gamecore_sub_01367890 | - | globals: 014663c4:R |  |
| 1366858 | stagemanager_getNbWorldLayoutDatas | 56 | recompiled | gamecore_sub_01407E88 | - | globals: 014663a8:R 014663ac:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1366ce0 | getWorldLayoutHead | 16 | recompiled | gamecore_sub_01407E88 | - | globals: 014663ac:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1366cf0 | initStageLayout | 8 | sotc_prof.cpp | - | - | - |  |
| 1366cf8 | deleteStageLayout | 8 | sotc_prof.cpp | - | - | - |  |
| 1366d00 | initWorldLayout | 16 | decomp_liftd7_07.cpp | - | - | globals: 014663c4:W |  |
| 1366d10 | deleteWorldLayout | 16 | recompiled | - | - | globals: 014663c4:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1375188 | GetVolumeClippedTargetPos | 448 | recompiled | gamecore_sub_0140B428, gamecore_sub_0140DAE0 | ClipJudgeVolumeInfo, GetCharacterClass, GetJudgeVolumeClassObjectGroupById, ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject, isysGroupForExit +2 | $a0: +0x0:W16; $a2: +0x0:R4; $a3: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c2c0 | getDebugDispType | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c308 | getDebugViewDisp | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c310 | GetDebugDispType | 8 | recompiled | SCRFuncHorseBossCircumferenceMain, SCRFuncHorseEscapeFromBossMain, SCRFuncSetAttainForParallelTrans, SCRFuncSetAttainForPreParallelTrans, gamecore_sub_0140B8B0 | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c328 | GetDebugViewDisp | 8 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c6e8 | SCRGetXZAngle | 32 | recompiled | SCRFuncYamoriBSendFallKey, nico_cerberusEye, physicalOp | iosGetATan2fExact | $a0: +0x0:R4 +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c708 | SCRGetYZAngle | 32 | recompiled | nico_cerberusEye | iosGetATan2fExact | $a0: +0x4:R4 +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137c9b8 | SCRGetAngle | 168 | recompiled | IsTouchableObj | iosGetACosfExact, iosGetStabXZVector | $a0: +0x0:R16; $a1: +0x0:R16; $a2: +0x0:R16 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 137def8 | getexpressiondatapoint | 16 | decomp_liftd2_01.cpp | nico_horseBodyExpression, nico_kameBodyExpression, nico_kirinBodyExpression, nico_leoBodyExpression | - | - |  |
| 1384ef8 | GetMeshObjInfo | 56 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1388688 | getWeakGlowSpeed | 16 | recompiled | - | - | globals: 014666e4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1388698 | getWeakGlowTime | 16 | recompiled | - | - | globals: 014666e8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13886a8 | getTrigGlowTime | 16 | recompiled | - | - | globals: 014666ec:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 138ebb8 | doit_game_camera_model | 224 | recompiled | - | GetLayoutClass, GetQuaternionFromMatrix, UpdateLayoutClassPastSimpleGeometry, UpdateLayoutClassSimpleGeometry, get_game_camera_WV_matrix +6 | globals: 01465650:& 014657d0:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1399088 | drawbar | 1896 | recompiled | - | glBegin, glBlendFunc, glColor4fv, glDisable, glEnable +5 | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x20:R16 +0x38:R4 +0x3c:R4 +0x40:R4/W4 +0x44:R4/W4 +0x48:R4 +0x4c:R4 +0x | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13997f0 | SpriteInitStatusClut | 688 | recompiled | - | texGetCSM1Idx | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1399aa0 | SpriteSetStatusClut | 1520 | recompiled | - | texGetCSM1Idx | $a1: +0x0:R4 +0x8:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139a198 | SpriteSetStatus | 128 | recompiled | - | - | $a0: +0x0:R4 +0x8:R4/W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ab50 | MotSpdCtrl_StartExt | 184 | recompiled | MotSpdCtrl_Start | GetCharacterClass | $a0: +0xc:W4 +0x10:R1 +0x14:R4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 1 calls, d9:dv1) |
| 139e058 | SCRLeoTorch0_Disp | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139e1a0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139e078 | SCRLeoTorch1_Disp | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139e240:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139e098 | SCRLeoTorch2_Disp | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139e2e0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139e0b8 | SCRLeoTorch3_Disp | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139e380:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139e878 | setNowLoadingMessageDisplayFlag | 24 | decomp_liftd7_08.cpp | demo_op1, gamecore_sub_01350280, gamecore_sub_01388948, gamecore_sub_013AB3D0, gamecore_sub_01402EA8, gamecore_sub_0140A8A8 +5 | set_nowloadingflag | - |  |
| 139e998 | setNowLoadingEffectDisplayFlagHokoraBack | 200 | recompiled | st_jumphokoraflow | gcCreateScriptObj, scrRun | globals: 001e0c88:& 0139e5c8:& 01466710:&RW 01466714:W 01466718:W 0146671c:&RW 01468b78:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139eba8 | nico_mammothArmRightTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466720:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ebe0 | nico_mammothArmRightTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466720:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ec20 | nico_mammothArmLeftTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466738:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ec58 | nico_mammothArmLeftTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466738:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ec98 | nico_mammothLegRightTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466750:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ecd0 | nico_mammothLegRightTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466750:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ed10 | nico_mammothLegLeftTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466768:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ed48 | nico_mammothLegLeftTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466768:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 139ed88 | nico_mammothAllTexEnd | 160 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466768:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13a87d0 | SCRConditionNargaStruggleStart | 416 | recompiled | - | ClipCollisionDirectCallBack, GetLayoutClass, getColiGroupByUserPathWork, getRealColJudgeFunc, getSCRDataCtrlWork +1 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ac108 | nico_SmokemanBodyTexEnd | 56 | decomp_liftd8_01.cpp | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466890:& |  |
| 13cfa90 | SCRSetDispFlag | 88 | decomp_liftd7_08.cpp | bossStatInitCloudLayout, demo_op2, gamecore_sub_013AB3D0 | SetLayoutObjAccessSwitch, SetScriptAnimationObjAccessSwitch | $a0: +0x8:R1 |  |
| 13d2b60 | AI_GetGeometryPointerOfLayout | 648 | decomp_liftd7_08.cpp | KiteGroundInitFunc | ClipCollisionDirectCallBack, GetDifferencialQuaternionWithNoRegularize, iosGetRandom, iosGetTableCos, iosGetTableSin +1 | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 +0x20:W8 +0x28:W8 +0x30:W8 +0x38:W8; $a1: +0x0:R16; $a2: +0x0:R16; glob |  |
| 13d2de8 | AI_CheckNoAngleVec | 768 | recompiled | AI_GetQuaternionNodeDirTarget | - | $a0: +0x0:R16/R4 +0x4:R4 +0x8:R4; $a1: +0x0:R16/R4 +0x4:R4 +0x8:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13d5600 | AI_CheckViewInCameraClipVolume | 240 | decomp_liftd3_02.cpp | AI_OptFloatFrameSample, gamecore_sub_013E5FF0 | get_WVC_matrix, get_main_cameraman_camera_obj, get_transform_matrix_set | $a1: +0x0:R16 |  |
| 13e7ce0 | Kite_SetSpecialPathLayout | 24 | decomp_liftd2_01.cpp | gamecore_sub_013E1190 | AI_SetAnimalLayout | - |  |
| 13f5e90 | nico_snakeBellyFrontTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466ac8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5ec8 | nico_snakeBellyFrontTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466ac8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5f08 | nico_snakeBellyCenterTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466ae0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5f40 | nico_snakeBellyCenterTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466ae0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5f80 | nico_snakeBellyBackTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466af8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5fb8 | nico_snakeBellyBackTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466af8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f5ff8 | nico_snakeAllTexEnd | 136 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466af8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f6338 | spritedisplaytrigger | 328 | recompiled | - | GetSpriteClass, iosFree, isysGetThisSubThread, scrRun | $a0: +0x0:R4 +0x8:R4; globals: 01465650:& 014657b8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f6480 | spritedisplay | 304 | recompiled | checkpoint_alert, r1_alert | GetSpriteClass | globals: 014657b8:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f6700 | ScrSpriteParamSetup | 56 | decomp_liftd7_08.cpp | EndMessageScript, ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayOverBGScript, ReplayPauseScript +23 | iosFree | $a0: +0x0:W4 +0x7:W4 +0x8:W4 +0xf:W4; $a1: +0x0:R4 +0x7:R4 +0x8:R4 +0xf:R4 |  |
| 13f6738 | ScrSpriteParamEnd | 64 | decomp_liftd7_08.cpp | EndMessageScript, ReplayEndScript, ReplayInScript, ReplayOverAlertScript, ReplayOverBGScript, ReplayPauseScript +18 | - | $a0: +0x0:R4 +0xc:R4 |  |
| 13f6778 | spritemenudisplaytriggerpad_group | 64 | recompiled | - | GetSpriteMenuClass, isysGetThisSubThread, scrRun | globals: 01466b1c:&RW | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13f95a0 | initWeaponSpriteStatus | 112 | decomp_liftd7_08.cpp | initWeapon | - | globals: 011f8628:&W 011f862c:W 011f8630:W 011f8634:W 011f8638:W 011f863c:W 011f8640:W 011f8644:W 011f8648:& 0 |  |
| 13f9610 | setWeaponSpriteStatus | 160 | decomp_liftd7_08.cpp | selectWeaponBefore, selectWeaponNext, setIsWeapon, setSelectWeapon | gamecore_sub_013F67B8 | globals: 011f8628:& 014657c8:R 01466b84:&RW 01466b88:&RW |  |
| 13fa800 | spriteselweapon | 304 | decomp_liftd7_08.cpp | - | GetSpriteClass, ScrSpriteParamSetup, gamecore_sub_013F9910, getDemoBarFade, getDrawScreenHeight +3 | globals: 011f85b0:& 011f85d4:R 01466b28:R 01466b2c:R |  |
| 13fdd68 | DispWithSwordBlur | 2520 | decomp_liftd3_03.cpp | - | CompletePlaneD, DispLayoutObjModel, GetLayoutClass, GetPointDistanceFromPlane, GetResetCameraFlagForDisplay +19 | - |  |
| 13fe740 | DispWithTorchBlur | 2520 | recompiled | - | CompletePlaneD, DispLayoutObjModel, GetLayoutClass, GetPointDistanceFromPlane, GetResetCameraFlagForDisplay +19 | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff198 | getMinAngle | 16 | recompiled | - | - | globals: 01466bf0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff1a8 | getMaxAngle | 16 | recompiled | - | - | globals: 01466bf4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff1b8 | getMaxLineAngle | 16 | recompiled | - | - | globals: 01466bf8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff208 | getMinAngleWeak | 16 | recompiled | - | - | globals: 01466c0c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff218 | getMaxAngleWeak | 16 | recompiled | - | - | globals: 01466c10:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff228 | getMaxLineAngleWeak | 16 | recompiled | - | - | globals: 01466c14:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff238 | getMaxFogMulVal | 16 | recompiled | - | - | globals: 01466c18:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff248 | getFogChangeSpeed | 16 | recompiled | - | - | globals: 01466c1c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff348 | getSwordGlowDistMin | 16 | recompiled | - | - | globals: 01466cf4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff358 | getSwordGlowDistMax | 16 | recompiled | - | - | globals: 01466cf8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff368 | getCollisionGlowSpeed | 16 | recompiled | - | - | globals: 01466d08:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 13ff378 | getCollisionGlowTime | 16 | recompiled | - | - | globals: 01466d0c:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1400840 | SetItemMenuDisp | 480 | recompiled | gamecore_sub_013AE8F8, gamecore_sub_01400EB0 | MakeStrHashValue, SpriteMenuGetLayerNoByHash, SpriteMenuGetMenuIdByHash, SpriteMenuSetCheckBoxByHash, SpriteMenuSetMenuDispFlag +4 | globals: 001b7b60:& 001e0ed4:R 0146bd30:& 0146bd38:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 14028b8 | CheckSelectDisp | 224 | recompiled | - | MakeStrHashValue, SpriteMenuSetCheckBoxByHash, _omakeSelectFlagChk, getGameHardMode | globals: 001b7b60:& 001e0ed4:R 0146bd30:& 0146bd38:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1406520 | nico_eelAllTexEnd | 208 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466e40:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1406b70 | nico_eelHigeTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466e58:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1406ba8 | spriteweaponsight | 792 | decomp_liftd7_09.cpp | - | GetSpriteClass, ScrSpriteParamSetup, iosFTOI0Vector, iosgluGetCameraContext, isysGetThisSubThread +1 | globals: 014657fc:R 01466e70:R 01466e80:&R 01466e90:&R 01466ea0:R |  |
| 140ae28 | SetMapLogDispMode | 16 | recompiled | - | - | globals: 01466ebc:W | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140ae68 | getLogLineColorR | 16 | recompiled | - | - | globals: 01466ec0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140ae78 | getLogLineColorG | 16 | recompiled | - | - | globals: 01466ec4:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140ae88 | getLogLineColorB | 16 | recompiled | - | - | globals: 01466ec8:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140ae98 | getLogLineColorA | 16 | recompiled | - | - | globals: 01466ecc:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d7b0 | nico_yamoriArmRightTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467100:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d7e8 | nico_yamoriArmRightTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467100:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d828 | nico_yamoriArmLeftTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467118:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d860 | nico_yamoriArmLeftTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467118:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d8a0 | nico_yamoriLegRightTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467130:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d8d8 | nico_yamoriLegRightTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467130:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d918 | nico_yamoriLegLeftTexEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467148:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d950 | nico_yamoriLegLeftTexStart | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467148:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140d990 | nico_yamoriAllTexEnd | 160 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467148:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140da30 | nico_yamoriAllTexStart | 168 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01467148:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 140fea0 | constructor_camera_default_angle | 248 | decomp_liftd7_09.cpp | gamecore_sub_0141FF70 | CAM_reset_slope_cntrl, GAMEUTIL_constructor_interp_ratio_cntrl, GAMEUTIL_reset_status_manager, constructor_aim_angle_interp_cntrl, constructor_camera_target_visible_angle_manager +1 | $a0: +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 +0x44:W4 +0x50:W16 +0x80:W4 +0x350:W4 +0x354:W4 +0x780:W4 +0 |  |
| 140ff98 | doit_camera_default_angle | 680 | decomp_liftd7_09.cpp | gamecore_sub_01420238 | CAM_doit_slope_cntrl, CAM_reset_slope_cntrl, GAMEUTIL_doit_interp_ratio_cntrl, GAMEUTIL_forward_timer_still_one_ratio_time_interp_ratio_cntrl, GAMEUTIL_get_cur_status_status_manager +13 | $a0: +0x30:R4 +0x34:R4 +0x38:W4 +0x3c:W4 +0x40:R4 +0x44:R4; $a1: +0x0:R4 +0xf38:R4 +0xf3c:R4 |  |
| 14109f8 | get_cur_attr_camera_default_angle | 24 | decomp_liftd7_09.cpp | CAM_get_update_pitch_by_view_cut_avoid_cntrl, doit_camera_target_lost_angle | GAMEUTIL_get_cur_status_status_manager | - |  |
| 1410a10 | is_safety_camera_default_angle | 40 | decomp_liftd7_09.cpp | gamecore_sub_014114D0, gamecore_sub_01421588, gamecore_sub_01421A68, gamecore_sub_01422E90 | GAMEUTIL_get_cur_status_status_manager | - |  |
| 1410a38 | is_must_reach_normal_default_angle | 88 | decomp_liftd7_09.cpp | CAM_doit_subjective_target_visibility_and_update_cam_data, doit_camera_target_lost_angle, gamecore_sub_01410240, gamecore_sub_01421588 | CAM_get_ground_contact_obj, GAMEUTIL_is_boss_obj | $a0: +0x0:R4 |  |
| 14124e8 | disp_camera_vs_ofst_cntrl_position | 280 | recompiled | - | kernel_sub_001B7B60 | globals: 011fb5d8:&R 011fb5dc:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1414748 | get_upper_limit_clip_default_pitch_camera_target_approach | 80 | decomp_liftd7_09.cpp | gamecore_sub_01410240 | GAMEUTIL_get_cur_status_status_manager | - |  |
| 1414bf8 | doit_camera_target_lost_angle | 440 | decomp_liftd7_09.cpp | gamecore_sub_01420238 | CAM_act_ray_check_for_camera, CAM_act_warp_order_camera_warp_cntrl, CAM_get_target_riding_obj, CAM_is_acting_view_cut_avoid_cntrl, GAMEUTIL_get_cur_status_status_manager +7 | $a0: +0x0:R4/W4 +0x20:R4; $a1: +0x90:R4; $a2: +0x1360:R4 |  |
| 1414db0 | constructor_camera_target_lost_angle | 56 | decomp_liftd7_09.cpp | gamecore_sub_0141FF70 | - | $a0: +0x0:W4 +0x10:W16 +0x20:W4 +0x24:W4 +0x30:W16; globals: 001e0fc0:&R 001e0fd0:&R |  |
| 1414de8 | get_yaw_pitch_camera_target_lost_angle | 120 | decomp_liftd7_09.cpp | gamecore_sub_01410240 | gamecore_sub_01415388 | $a0: +0x0:W4; $a1: +0x0:W4; $a2: +0x0:R4 |  |
| 1414e60 | get_attr_camera_target_lost_angle | 8 | decomp_liftd7_09.cpp | CAM_get_update_pitch_by_view_cut_avoid_cntrl | - | $a0: +0x0:R4 |  |
| 1414e68 | is_exist_camera_target_lost_angle | 16 | recompiled | - | - | $a0: +0x0:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1415690 | begin_and_update_get_yaw_pitch_camera_target_visible_angle_manager | 552 | recompiled | gamecore_sub_01410240 | GAMEUTIL_get_difference_yaw_and_yaw, GAMEUTIL_is_between_yaw_and_yaw, gamecore_sub_01415A48, gamecore_sub_01415AB8, gamecore_sub_01415BB8 | $a1: +0x0:W4; $a2: +0x3d0:W4 +0x3d4:R4/W4 +0x3d8:W4; $a3: +0xa94:R4 | D11: lifted, not reached at top level in the trace runs (earlier call counts: 111 calls, d9:dv1) |
| 14158b8 | is_exist_camera_target_visible_angle | 8 | recompiled | - | - | $a0: +0x0:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 14158c0 | constructor_camera_target_visible_angle_manager | 64 | decomp_liftd7_09.cpp | constructor_camera_default_angle | reset_camera_target_visible_angle_manager, set_parameter_camera_target_visible_angle_manager | - |  |
| 1415900 | set_parameter_camera_target_visible_angle_manager | 112 | decomp_liftd7_09.cpp | constructor_camera_target_visible_angle_manager | - | $a0: +0x3dc:R4/W4 +0x3e0:W4 +0x3e4:W4 +0x3e8:W4 |  |
| 1415970 | reset_camera_target_visible_angle_manager | 88 | decomp_liftd7_09.cpp | constructor_camera_target_visible_angle_manager, gamecore_sub_01410D58 | - | $a0: +0x0:W4 +0x3d0:W4 +0x3d4:W4 +0x3d8:W4; globals: 001e0fd0:&R |  |
| 14159c8 | doit_camera_target_visible_angle_manager | 120 | decomp_liftd6_02.cpp | gamecore_sub_01410D58 | gamecore_sub_014154F0 | $a0: +0x3e4:R4; globals: 001e0fd0:&R |  |
| 1415a40 | end_get_yaw_pitch_camera_target_visible_angle_manager | 8 | decomp_liftd7_09.cpp | gamecore_sub_01410240 | - | $a0: +0x3d0:W4 |  |
| 14167d0 | CAM_doit_aim_angle_rot | 760 | decomp_liftd7_09.cpp | CAM_get_update_pitch_by_view_cut_avoid_cntrl, CAM_get_update_yaw_by_view_cut_avoid_cntrl, act_pitch_rot_camera_target_approach, gamecore_sub_01421588, gamecore_sub_01426D00 | CAM_is_inside_of_region_aim_angle_rot, GAMEUTIL_doit_interp_ratio_cntrl, GAMEUTIL_get_difference_pitch_and_pitch, GAMEUTIL_get_difference_yaw_and_yaw, GAMEUTIL_get_interp_ratio_interp_ratio_cntrl +7 | $a0: +0x0:R4 +0x4:R4/W4 +0x8:R4/W4 +0xc:R4 +0x20:R4/W4 |  |
| 141a170 | CAM_set_clip_max_rot_vel_aim_rot_common | 24 | recompiled | - | - | $a0: +0xc:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 141a1f8 | CAM_init_aim_angle_rot | 160 | decomp_liftd7_09.cpp | constructor_camera_target_approach, doit_camera_target_approach, gamecore_sub_01420EE8, gamecore_sub_01421A68, gamecore_sub_014257B0, gamecore_sub_01425808 +10 | GAMEUTIL_constructor_interp_ratio_cntrl, GAMEUTIL_set_act_max_zero_clip_flag_interp_ratio_cntrl, gamecore_sub_0141B450 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:W4 |  |
| 141a298 | CAM_reset_aim_angle_rot | 48 | decomp_liftd7_09.cpp | reset_pitch_rot_cntrl_camera_target_approach | GAMEUTIL_reset_interp_ratio_cntrl, gamecore_sub_0141B480 | - |  |
| 141a2c8 | CAM_is_inside_of_region_aim_angle_rot | 48 | decomp_liftd7_09.cpp | CAM_doit_aim_angle_rot, gamecore_sub_0141B488 | GAMEUTIL_is_inside_of_region_pitch, GAMEUTIL_is_inside_of_region_yaw | $a2: +0x0:R4 |  |
| 141a760 | CAM_set_target_display_flag | 88 | recompiled | - | GetLayoutClass | $a0: +0x0:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 141c718 | doit_camera_view_angle_cntrl | 552 | decomp_liftd6_02.cpp | gamecore_sub_0141EA78 | doit_double_spring_interp_cntrl, gamecore_sub_0141C9B8, gamecore_sub_0141C9E8, get_cur_using_lws_anime_mode_base_class, is_view_angle_exist_lws_anime_mode_base +2 | $a0: +0x1c:R4 +0x3c:R4 |  |
| 141c940 | constructor_camera_view_angle_cntrl | 112 | decomp_liftd7_09.cpp | gamecore_sub_0141FF70 | constructor_double_spring_interp_cntrl, reset_double_spring_interp_cntrl | - |  |
| 141c9b0 | get_view_angle_camera_view_angle_cntrl | 8 | decomp_liftd7_09.cpp | gamecore_sub_01417ED8, gamecore_sub_014235D0, gamecore_sub_01425F78, init_camera_pause_rise | - | $a0: +0x1c:R4 |  |
| 141e630 | get_debug_camera_view_angle | 16 | recompiled | - | - | globals: 01467250:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 141e640 | get_debug_camera_view_angle_pointer | 16 | recompiled | - | - | globals: 01467250:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 141e650 | is_disp_game_camera_model_ok | 16 | recompiled | doit_game_camera_model | - | globals: 01467254:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 141e660 | is_disp_game_camera_model_ok_pointer | 16 | recompiled | - | - | globals: 01467254:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1425518 | is_view_angle_zoom_game_camera | 48 | decomp_liftd3_03.cpp | gamecore_sub_0141C9E8 | GAMEUTIL_is_push_or_down_1p_button | globals: 01467278:R |  |
| 142da48 | execPointListSort | 600 | decomp_liftd6_02.cpp | SCRConditionCheckWayOrientAttainPlayer, gamecore_sub_01405DB8, gamecore_sub_0142FB60, updateUserPathWork | getPointDataByAreaIdx | $a0: +0x3b8:R4 +0x3c50:R4; $a1: +0x0:W4; $a2: +0x0:R16; globals: 012010c8:& |  |
| 1431f40 | setAngleStabilizeYPathWork | 8 | decomp_liftd4_03.cpp | createUserPathWork, initScriptDataSystem | - | $a0: +0xec:W4 |  |
| 1431f58 | setExecRayChkPathWork | 8 | decomp_liftd4_04.cpp | createUserPathWork, gamecore_sub_01390A90 | - | $a0: +0x384:W4 |  |
| 1432828 | setRouteMinAngle | 40 | decomp_liftd4_03.cpp | SCRFuncSetNextRouteMaxAngle, createUserPathWork, initScriptDataSystem, nico_SmallKiteAdvertiseStart | iosGetCosf | - |  |
| 1432850 | setSearchRouteMaxAngle | 88 | decomp_liftd4_04.cpp | createUserPathWork, initScriptDataSystem, nico_SmallKiteAdvertiseStart | iosGetCosf | $a0: +0x3b4:W4 |  |
| 1432ee0 | chkAttainExecLayout | 40 | decomp_liftd6_02.cpp | SCRConditionAttainRange, gamecore_sub_01439FC8, gamecore_sub_01443250, gamecore_sub_01443E70, gamecore_sub_01445030, getCalcAttackPassagePoint2 +6 | chkAttainExecOffsetData | globals: 001e0f90:& |  |
| 1437a60 | _setExecBaseMoveWork | 8 | decomp_liftd4_04.cpp | gamecore_sub_0138F3B0, gamecore_sub_0138F5B0, gamecore_sub_01390238, guardMainCommon, nico_guardAStart, nico_shamanStart +2 | - | $a0: +0x4:W4 |  |
| 1437b80 | _getExecBaseMoveWork | 8 | recompiled | - | - | $a0: +0x4:R4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1437e20 | calcMoveTurnAngle2 | 56 | recompiled | - | calcMoveAngle | $a0: +0x1c:R4 +0xa8:R4; globals: 0146c808:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1438188 | moveCtrlClipCollisionDirect | 360 | recompiled | gamecore_sub_01447DC0 | ClipCollisionDirectCallBack | $a3: +0x400:R4; globals: 014383f0:& | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1440f48 | setRunStopTurnAngleFastLegCtrlWork | 24 | recompiled | updateScriptDataSystem | - | $a0: +0x4c:W4 | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1445c90 | calcMoveAngle | 320 | decomp_liftd4_04.cpp | calcAngleTime, calcMoveTurnAngle2 | getRatioFromInterp | $a0: +0x0:W4 |  |
| 1445dd0 | calcAngleTime | 248 | decomp_liftd10_00.cpp | calcTurnTime2, gamecore_sub_0143ABA8, gamecore_sub_0143AF28, gamecore_sub_014499A0 | calcMoveAngle | - |  |
| 14460a8 | chkMoveAngle | 120 | recompiled | gamecore_sub_0143AF28 | - | - | D11: lifted, not reached at top level in the trace runs (earlier call counts: 13915 calls, d9:dv1) |
| 1446120 | chkMoveAngle2 | 48 | recompiled | - | - | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1446868 | adjustAngleForLimit | 40 | decomp_liftd7_08.cpp | gamecore_sub_0143AF28, gamecore_sub_0143B988, gamecore_sub_0144B048, nico_aimIK, nico_sheathScript | - | - |  |
| 1446f80 | adjustAnglePI | 152 | decomp_liftd7_08.cpp | SCRFuncPoseidonParallelTrans, gamecore_sub_01391E30, gamecore_sub_01394A08, gamecore_sub_013CDED0, gamecore_sub_01449540, nico_BeardNext +3 | - | - |  |
| 144f8d0 | SCRFuncSetNextRouteMaxAngle | 72 | recompiled | - | getSCRDataCtrlWork, setRouteMinAngle | - | D11: lifted, not reached at top level in the trace runs (earlier call counts: 7 calls, d9:dv1) |
| 144fbc8 | SCRFuncSetAttainMyselfPosAndGlobalY | 96 | recompiled | - | GetLayoutClass, getSCRDataCtrlWork, setAttainmentPos | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1450998 | SCRFuncSetDispFlagGameObj | 112 | decomp_liftd8_02.cpp | - | SetLayoutObjAccessSwitch | globals: 001e0ed4:R |  |
| 1451058 | SCRFuncStartStageLayoutLWS | 184 | recompiled | - | ps2__SCRStartLWS, setGameFlagForGameObj | globals: 001e0ed4:R | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1456860 | SCRConditionCheckAngleToNextPoint | 472 | decomp_liftd8_02.cpp | - | GetLayoutClass, getPassageWork, getSCRDataCtrlWork, iosGetATan2fExact | - |  |
| 14577a0 | SCRConditionMyselfOverGlobalY | 64 | recompiled | - | GetLayoutClass | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 14577e0 | SCRConditionMyselfLessGlobalY | 64 | recompiled | - | GetLayoutClass | - | never called in any call-count run (D7-D10 routes, D11 menu / attract) |
| 1458030 | SCRConditionModeLapsedTime | 56 | decomp_liftd8_02.cpp | - | getSCRDataCtrlWork | - |  |
| 1459670 | SCRConditionEyeModeLapsedTime | 56 | recompiled | - | getSCRDataCtrlWork | - | D11: lifted, not reached at top level in the trace runs (earlier call counts: 60 calls, d9:dv1) |
| 1459a50 | SCRConditionChkDisplayOmakeCompleteAlert | 32 | decomp_liftd7_11.cpp | - | - | globals: 01466884:R |  |
| 145c118 | getInterestExec | 16 | decomp_liftd7_07.cpp | SCRConditionHorseInterestExec | - | $a0: +0x0:R4 |  |
