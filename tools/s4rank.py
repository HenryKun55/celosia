#!/usr/bin/env python3
"""s4rank.py <run.syms.txt> [ms_per_frame]: round S4's display / draw-prep items of a sampler dump (a symbol per line,
"<share>% <symbol>", the game thread's samples: build/agent-scratch/s4/tools/s4groups.py --dump), grouped, as a share of
the game thread (and ms a frame when the game thread's ms a frame is given). docs/display_s4.md section 2."""
import re, sys
groups = {
    'screen passes (own code)': r'reflectionBegin|lift_d1::(shadowDraw|shadowReset|bgEnd|depthOfField|waterRefractionBegin|bgDraw|zFog|manager_sub_0109FB08|solarFlareReset|manager_sub_010F37C8|cameraBlur|refractionBegin|bossDeathEffectBegin)\(|zFog_0x10ac0a8|bgDraw_0x1085258|bloomDisplay|decomp_hot48a::pass|screenBlur_0x',
    'gsReg packets (gsReg, hook, census, begin / end, spriteNoTexFast, spriteTexFast hook)': r'decomp_hot44g::gsReg\(|sotc_native::gsRegHook|gsRegCensus|gsRegEnd|gsRegBegin|decomp_leaves::sprite\(|spriteTexFastHook|gsRegLean',
    'layer stack (hooks + natives)': r'pushLayerHook|popLayerHook|pushLayerLean|popLayerLean|decomp_leaves::(pushLayer|popLayer)\(',
    '2D assembler (feed2D, vertex2D, emit2D, sprite family, imm quads)': r'feed2D|vertex2D|emit2D|spriteDrawSprite|immFeed128|quad117',
    'palettes read back (hook + kick)': r'readChainPalette|kickCopy',
    'model hook + mds lane (game thread)': r'modelDispHook|mdsPartWalk|mdsPlanPackets|mdslane::join|hookScrolls127|queueNextDraw|packetListHash|modelDispState|syncFrame',
    'walk / clip / sort / light natives': r'decomp_walk::|decomp_light::|decomp_dl::|decomp_layout|decomp_modelclip|decomp_pointsort|decomp_hot43b|decomp_prefetch|decomp_group|decomp_ambient|decomp_hot44a::clipKeepTest',
    'textures (request, reset, anim, VRAM)': r'texTransRequest|decomp_texreset|texResetFast|vramReset|vramAlloc|textureRecord|decomp_texanim|decomp_hot46f|decomp_texframe',
    'callGuest dispatch (all callers)': r'sotc_decomp::callGuest',
    "S4's own code": r's4disp::',
}
ms = float(sys.argv[2]) if len(sys.argv) > 2 else 0.0
tot = {k: 0.0 for k in groups}
for line in open(sys.argv[1]):
    m = re.match(r'\s*([\d.]+)% (.*)', line)
    if not m:
        continue
    for k, rx in groups.items():
        if re.search(rx, m.group(2)):
            tot[k] += float(m.group(1))
            break
for k, v in sorted(tot.items(), key=lambda x: -x[1]):
    print(f'  {v:6.2f}%' + (f'  {v * ms / 100:6.3f} ms' if ms else '') + f'  {k}')
