# Collision and physics: a map for S2

Written in Decomp D9 (2026-11-15) for round S2, the redesign of collision and physics for the Vita. It covers what the
code is, where its data lives, who calls it, what it costs on the game thread, which parts are host code already, and
what is still recompiled. Every address is GAMECORE's. Addresses below 0x1000000 inside GAMECORE code are relocation
placeholders (`lui 0x1e` / `0x1b` + offset), so the real target has to be read from the live instruction words
(sotc_decomp::relocated).

Sources: the recompiled code's MIPS comments (build/runner), the hand natives and their headers (decomp_collision.cpp,
coll115.h / coll119.h / coll123.h / coll125.h, decomp_finder.cpp, decomp_skinfinder.cpp, decomp_hot43a / 43c / 44a / 44b
/ 46g / 47c / 47d / 47j / 48c, decomp_probe84.cpp, decomp_collcensus.cpp, decomp_rest81.cpp), docs/decomp.md (the rounds
cited in each row), D9's access-pattern pass (build/agent-scratch/d9/access.py: each function's argument offsets and
globals, read in linear order, a heuristic) and call graph (direct `jal`s only: calls through tables and function
pointers are not seen), D9's call counts with first-call frames (`SOTC_CALL_COUNT_FIRST`) and D9's fallback counts
(`SOTC_FALLBACK_COUNT`).

## What "Collision and physics" is

tools/decomp_status.py puts a function in this area by its name (`collision|coll|hit|rigid|phys|dyn|ground|fit|swim|
pendulum|volume`) or by its file (decomp_collision.cpp). That gives 319 functions. They are four different things:

| part | where | what it does |
|---|---|---|
| **The collision library** | 0x1111000-0x1136F00 (ClipCollision*, GetCollisionDB*, the finder, the edge links, the modified managers, the volumes) | The world's surfaces and the queries against them: a segment against a group of objects (ClipCollisionDirectCallBack and its tree walk), a sphere's contact list (the "finder": GetCollisionDBCallBack fills it, ClipCollisionFinder* test it), surface attributes, planes, boundaries and edges, the per-frame caches. Most of the hot path is host code already (below). |
| **The characters' root dynamics** | 0x10D4000-0x10EBA00 (charrootdyn_*, motrootdyn_*, pendulum, parentpointxzfit, edgexz, swim, norootdynamics_*) | How a character's root moves against the world, mode by mode: xz (walking), xzfit (fitted to slopes), falldown, edge point (hanging from a ledge), pendulum (hanging and swinging), parent point (standing or climbing on a moving body), chain hang, swim. A mode change is a motrootdyn_start*_from_* function, called through the motion data's mode table. Wander, Agro, the colossi and the animals all use it. |
| **Gameplay users** | scattered (SCR*, GAMEUTIL_*, the AI's path tests, the camera's checks) | The scripts' segment tests and volumes, the fights' hit tests (SCRConditionSendDamageToCollisionObject*, WeaponHitAttr, SwordFlareHitColAttr), the AI's way / path collision, the camera's filters. |
| **Debug and test** | scattered | The dynamics test object (nico_pendulumtest, pendulum_test, dynamicstest and its tuning getters), debug draw modes, the wide-area search. |

For S2, **the collision library and the root dynamics are the subsystem**. The gameplay users are its clients.

## How collision runs in a frame

1. **The characters' frame** (ExecCharaGeometry, native, decomp_charageo.cpp) calls GetCharaRootDynamics (native,
   decomp_hot47d.cpp) for each character. By the root's mode it runs that mode's step (xz, xzfit, edge point, pendulum,
   parent point ...). A step asks the library for a contact list around the root (GetCollisionDBCallBack: the finder is
   filled object by object through the skinned finder 0x1116958 and the tree walk 0x1128F38), then tests it:
   ClipCollisionFinder / ...CallBack (the landing ray, the IK feet), ClipCollisionFinderWithKeep* (the slide's keep pass,
   the slope probe), the edge links (GetCollisionEdgeLinkDBVWallFinder: walls and edges nearby). CalcRootFitting (lifted,
   D4) fits the root to the result.
2. **Segment tests** come from the camera (camera_collision_check_direct, its filters), the scripts
   (SCRGetCollisionCallBack), the AI's path checks (pathDetectCollision*, widePathCollisionDirect), the water probes and the
   characters' own rays. Each is ClipCollisionDirectCallBack: a segment against every object of an isys group (object
   filter, bound cylinder, part cylinder, part filter, the part's tree, leaves, ClipCollisionDB per surface).
3. **The caches.** Planes, boundaries and vertices of moving surfaces are made on demand into per-part caches (the
   modified managers) and flushed once a frame (FlushCollisionWork / FlushCollisionWorkByWorkStruct). fin125 keeps a host
   copy of the finder lists' surfaces across frames; coll115 keeps host copies of the trees' leaves.

## The hand-written natives that touch collision

"recompiled(addr)" below is sotc_decomp::recompiled(): the generated function, or the lifted fallback copy when a decomp
round registered one for that address (trace-checked; served while that round's switch is on). D9 counted at run time
which fallbacks actually run (`SOTC_FALLBACK_COUNT`). Every native can also be left out alone with
`SOTC_DECOMP_SKIP=<table name>`. A "guest call" is sotc_decomp::callGuest through the function table (so it reaches another
native when one is registered there).

### How the switches work

All switches are read once (sotc_decomp.h `switchOn`). Default on (`SOTC_X=0` turns it off) unless listed in the off set:
find84, ptcl91, early95, obj96, charlod78, ptcl79, oan106, flush119, walk119, part119, view119, gaze122, texr128, ctl126,
seg123, pose136 (`SOTC_X=1..3` turns those on). The collision-relevant group switches:

| switch | default | what it puts back (from sotc_decomp.h comments and the entries' code) |
|---|---|---|
| SOTC_CAMCOLL | on | round 39: the camera wrappers, GetArbitraryDirectionInverseMatrix, the object filters, the camera / gamecore id filters, the vertex fetcher's full form, EntryModifiedManager's entry |
| SOTC_COLLPROBE | on | round 40: SCRGetCollisionCallBack, the water probes, GetCollisionDBPlane, moveCtrlWallJudgeFunc's siblings 013CB8E8 / AI_JudgeColAttrFunc |
| SOTC_LEAVES | on | round 42: FlushCollisionWorkByWorkStruct, FlushCollisionWork, character_ikcolattrfilter |
| SOTC_HOTLIST | on | round 43: the slope probe 0111C810, the segment / triangle test 0116A0D4 |
| SOTC_HOTLIST44 | on | round 44: clipKeep's full test 0111C078 (clipKeepTest), movedPoint 01118B88, keepThenSlope 01129988 |
| SOTC_HOTLIST46 | on | round 46: ClipCollisionDirectCallBackObjAry, ClipCollisionFinderWithKeep4DSlopeLimitCB |
| SOTC_HOTLIST47 | on | round 47: GetCollisionDBCallBack, the ground probe 01118340, GetCharaRootDynamics (decomp_hot47d), charrootdyn_slideExceptionJudgeCB, GetCharaRootDynamicsAfter |
| SOTC_HOTLIST48 | on | round 48: GetCollisionDBEdgeInfoNext3, the finder tree node 01115D00 |
| SOTC_FASTNATIVE / 2 / 3 | on | rounds 49 / 54 / 58: the A9 fast paths inside the natives (off = round 48's code paths, still native) |
| SOTC_COLL71 | on | round 71: the root dynamics' id filters native, ClipCollisionFinderWithKeep native, the finder's backward duplicate search |

Rows marked "always native" have no switch of their own: off is only possible with SOTC_DECOMP_SKIP.

### The table

| guest address | guest name | host function | file | switch | default | what it does | fallback to recompiled(addr) |
|---|---|---|---|---|---|---|---|
| 0x112D590 | ClipCollisionDirectCallBack | decomp_collision::directCallBack (body directCallBackBody, or obj125Run when obj125 is on) | decomp_collision.cpp | always native (obj125 / seg80 / seg123 / coll115 / walk101 / skin101 / obj101 inside it switch paths) | on | A segment against every object of an isys group: object filter, the object's bound cylinder in the segment's frame, each part's cylinder and part filter, the part's tree (coll115, the native walk, or the old recompiled walkers 0x112ACD0 / 0x112B890 as guest calls), one-by-one surfaces, skinned surfaces; a hit shortens the segment; result record 0x50 bytes | Never. The old tree walkers, GetArbitraryDirectionInverseMatrix, isysGroupForFirst / Next / Exit, the guest filters and 0x1124368 (CalcCollisionMultiDepSurfBoundary) are guest calls |
| 0x1130490 | ClipCollisionDirectCallBackDoubleSide | decomp_collision::directDoubleSide | decomp_collision.cpp | always registered (for the memo) | on | Runs the original code under seg80 / seg123's memo; no native body | Always (the body is recompiled(0x1130490)); the memo may answer instead |
| 0x112E180 | ClipCollisionDirectCallBackObjAry | decomp_hot46g::objAry (or obj125Run<true>) | decomp_hot46g.cpp | SOTC_HOTLIST46 | on | The same test over an array of objects (the colossi's bodies); guest-exact transcription, or obj125's host loop | Only when SOTC_HOTLIST46=0 |
| 0x1131C80 | collisionNode (manager_sub_01131C80) | decomp_collision::entry<node> | decomp_collision.cpp | always native; SOTC_FASTNATIVE picks round 48's form; walk101 / coll115 inside | on | The tree's node visitor: the side the segment starts on first | Never |
| 0x112C498 / 0x112C610 | collisionLow / collisionHigh | decomp_collision::entry<low> / entry<high> | decomp_collision.cpp | always native | on | The node's two sides (segment clipped at the split); from the object-array test's four call sites (ra 0x112E600 / 624 / 650 / 674) coll115's host walk is tried first | Never |
| 0x112BE58 | collisionLeaf | decomp_collision::entry<leaf> (leafT) | decomp_collision.cpp | always native; leaf101, coll63 inside | on | A leaf's surfaces: boundary cylinder (NEON rejection on the Vita), surface and id filters (native when known, else guest calls), ClipCollisionDB | Never (unknown filters are guest calls) |
| 0x111B9F0 | ClipCollisionDB | decomp_collision::clipCollisionDB (clipDBT) | decomp_collision.cpp | always native | on | One surface: the plane (made if not cached), the two plane distances, the crossing point, the boundary sphere, the barycentric u / v; fills the hit record | When GetLayoutClass would compute the class by a call (layoutClass() false): recompiled from the start |
| 0x1119248 | getLayoutClassCollisionIndepSurfBoundary | decomp_collision::indepSurfBoundary | decomp_collision.cpp | always native | on | An independent surface's world boundary sphere from the part's cache | When the boundary's cache flag is clear (not made this frame): recompiled (lifted D7 copy while SOTC_LIFTD7 is on) |
| 0x11193C8 | GetCollisionDBSurfBoundary | decomp_collision::surfBoundaryEntry | decomp_collision.cpp | always native | on | A surface's boundary (centre, r^2) for either kind | Class computed by a call, or a skinned surface whose boundary is not cached: recompiled |
| 0x1125B30 | GetCollisionDBAttributeAdr | decomp_collision::attributeAdr | decomp_collision.cpp | always native | on | The surface's attribute record address (DB +0x34 table, index from the surface's list entry or the face table) | Class computed by a call |
| 0x11257A0 | GetCollisionDBPlane | decomp_collision::collisionPlane | decomp_collision.cpp | SOTC_COLLPROBE | on | The plane's address in the part's cache, made first when its flag is clear | Switch off; any of the four jal targets (GetLayoutClass, 01114C70, 01116108, EntryModifiedManager) not the expected one; class computed by a call |
| 0x111C078 | clipCollisionKeep | decomp_collision::clipKeep, rest in decomp_hot44a::clipKeepTest | decomp_collision.cpp, decomp_hot44a.cpp | always native (the rest under SOTC_HOTLIST44) | on | A moving sphere's keep test against one surface: plane and the first test (start at least f12 in front: v0 2) natively, the rest by clipKeepTest | Class computed by a call or any case past the first test go to clipKeepTest, which is native; recompiled only when SOTC_HOTLIST44=0 |
| 0x1114C70 | collisionVerticesIndep | decomp_collision::verticesIndepEntry | decomp_collision.cpp | SOTC_CAMCOLL | on | A surface's three world vertices, made into the part's vertex cache when not cached (packed int16 or float, short or long ids) | Only with SOTC_CAMCOLL=0 |
| 0x11478B8 | EntryModifiedManager | decomp_collision::entryModifiedEntry | decomp_collision.cpp | SOTC_CAMCOLL | on | Appends an index to a cache's modified list and sets its flag byte | Switch off, or the manager / its list / its flag byte outside main RAM |
| 0x1147798 | FlushModifiedManager | decomp_collision::flushModified | decomp_collision.cpp | always native | on | Clears every listed flag byte, count to 0; moves fin125's epochs | Index size not 1 / 2 / 4 (the original's error report) |
| 0x1124410 | FlushCollisionWorkByWorkStruct | decomp_collision::flushWork (flushWorkBody) | decomp_collision.cpp | SOTC_LEAVES (flush119 inside, off) | on | Flushes each part's four modified lists (+0, +0x30, +0x48, +0x18) and the work's +0x28 / +0x40 / +0x58 / +0x70 by the two flags, clears +0x1c | Only with SOTC_LEAVES=0 |
| 0x10F1D60 | FlushCollisionWork | decomp_collision::flushCollisionWork | decomp_collision.cpp | SOTC_LEAVES | on | The object's layout class by its type, then the tail jump into the work flush | Switch off; jal targets not the expected ones; a type table entry other than the six known |
| 0x1125DC8 / 0x1125CC0 | ClipCollisionFinder / ClipCollisionFinderCallBack | decomp_collision::finderClip<false> / <true> (finderClipBody) | decomp_collision.cpp | always native; fin125 / fin138 / view119 inside | on | A segment against a finder's surface list (the landing test, the IK foot search); the CallBack form asks an id filter | A negative count (min of the finder's two counts): recompiled |
| 0x1125F90 | ClipCollisionFinderWithKeep | decomp_hot44b::finderWithKeep | decomp_hot44b.cpp | SOTC_COLL71 | on | The slide's keep pass: keepThenSlope per entry; fin125 / view119 predict first returns | Switch off, or a negative count |
| 0x1129988 | keepThenSlope (manager_sub_01129988) | decomp_hot44b::keepThenSlope | decomp_hot44b.cpp | SOTC_HOTLIST44 | on | clipKeep, then the slope probe when it misses | Only when SOTC_HOTLIST44=0 |
| 0x111EF10 | ClipCollisionFinderWithKeep4DSlopeLimitCB | decomp_hot46g::finderSlopeCB | decomp_hot46g.cpp | SOTC_HOTLIST46 | on | A moving sphere against kept planes (movedPoint, 0111DDC0) then kept surfaces (movedPoint, clipKeep, slope probe), with a hit callback | Only when SOTC_HOTLIST46=0 |
| 0x111C810 | manager_sub_0111C810 (slope probe) | decomp_hot43c::slopeProbe | decomp_hot43c.cpp | SOTC_HOTLIST | on | A surface against its concave neighbours across its edges (edge test 0116A0D4, the vertex normal cache) | Only when SOTC_HOTLIST=0 |
| 0x116A0D4 | manager_sub_0116A0D4 | decomp_hot43a::segmentTriangle | decomp_hot43a.cpp | SOTC_HOTLIST | on | A segment against a triangle (VU0 macro code) | Only when SOTC_HOTLIST=0 |
| 0x1118B88 | manager_sub_01118B88 (movedPoint) | decomp_hot44a::movedPoint | decomp_hot44a.cpp | SOTC_HOTLIST44 | on | A probe point moved with a surface (its nearest vertex's motion, or the part's matrix) | Only when SOTC_HOTLIST44=0 |
| 0x1128F38 | collisionFinderWalk | decomp_finderwalk::finderWalk | decomp_collision.cpp | always native | on | The finder's tree walk (sphere against the split tree), leaves by a guest call of the leaf callback; records splits for ground85 | Never |
| 0x1115798 / 0x1115A50 | collisionFinderLeaf / collisionFinderLeaf16 | decomp_collision::finderLeaf<true> / <false> | decomp_collision.cpp | always native (pf135, coll71 inside) | on | A finder leaf: each surface's boundary against the sphere (NEON scan), plane distance under the radius, id filter, duplicate search, EntryCollisionFinder (guest call) | Never. The comment still says an uncached plane sends the leaf to the recompiled code; the code now makes the plane natively |
| 0x112A478 | CheckCollisionFinderExistData | decomp_collision::checkFinderExistData | decomp_collision.cpp | always native | on | Whether a finder already holds a surface from index a1 on | Never |
| 0x112A250 | EntryCollisionFinder | decomp_collision::entryFinder | decomp_collision.cpp | always native | on | Stores an id pair and links it at the object's work +0x10 list; count up | Below capacity and the class computed by a call: recompiled |
| 0x1116958 | skinFinder (manager_sub_01116958) | decomp_finder::finder | decomp_finder.cpp | always native | on | The skinned finder step: each part's spheres against the query, surfaces or triangles to EntryCollisionFinder after the callbacks; guest-exact transcription (frame, VU0 registers) | Never |
| 0x1116108 | skinFinderCorners | decomp_skinfinder::corners | decomp_skinfinder.cpp | always native | on | A skinned triangle's three corners into the bone / blend vertex caches (EntryModifiedManager inline) | A blend list with no entry (the original would run away) |
| 0x1115D00 | manager_sub_01115D00 (finder tree node) | decomp_hot48c::treeNode | decomp_hot48c.cpp | SOTC_HOTLIST48 | on | One node of a finder tree walk by tree kind (old 16-byte, or 8-byte walks), leaves by callback | Switch off, or an entry sp not 8-aligned |
| 0x1124C88 | GetCollisionDBCallBack (the ground fill) | decomp_hot47j::collisionDB | decomp_hot47j.cpp | SOTC_HOTLIST47 (ground85 / find84 inside) | on | Fills a finder from a group's objects (finder step per object, the undo for script character classes) | Only when SOTC_HOTLIST47=0 |
| 0x1118340 | manager_sub_01118340 (ground probe) | decomp_hot47j::groundProbe | decomp_hot47j.cpp | SOTC_HOTLIST47 | on | Whether a triangle reaches down to the object's ground (a 1000-unit probe down through ClipCollisionDirectCallBackOneObj) | Only when SOTC_HOTLIST47=0 |
| 0x111A938 | GetCollisionDBEdgeInfoNext3 | decomp_hot48c::edgeInfoNext3 | decomp_hot48c.cpp | SOTC_HOTLIST48 | on | The surfaces next to a surface's three edges (16-bit or 32-bit link records) | Only when SOTC_HOTLIST48=0 |
| 0x1117EA0 | manager_sub_01117EA0 (water probe) | decomp_collision::waterProbe | decomp_collision.cpp | SOTC_COLLPROBE | on | Is the segment's end over a water triangle with a point under it | Switch off; jal target not GetLayoutClass; class by call; a triangle that needs the ray check (p1y or p0y under its height) |
| 0x1117340 | manager_sub_01117340 (water surface) | decomp_collision::waterSurface | decomp_collision.cpp | SOTC_COLLPROBE | on | The segment's crossing of a water triangle's height | Switch off; jal target; class by call; a triangle filter with a crossing possible; a crossing inside a triangle (recompiled from the start; lifted D6 copy while SOTC_LIFTD6 is on) |
| 0x11278F0 | IsWaterSurfaceExistAround | probe84::waterAround | decomp_probe84.cpp | SOTC_PROBE84 | on | The camera's water search answer kept (0.25 units, 10 frames), caller ra 0x113A740 only | Every other caller, the switch off, the worker lane, out pointer / point outside RAM, and every miss of the memo run the game's code |
| 0x137D1A0 | SCRGetCollisionCallBack | decomp_collision::scrCollision | decomp_collision.cpp | SOTC_COLLPROBE | on | The scripts' segment test: ClipCollisionDirectCallBack with the script's id filter, CopyGeometryPointer of a hit | Switch off, or either jal target not the expected one |
| 0x140EE60 | camera_collision_check_direct | decomp_collision::cameraCheckDirect | decomp_collision.cpp | SOTC_CAMCOLL | on | The camera's wrapper: judge data, the move / visible group, object and id filters, then ClipCollisionDirectCallBack | Switch off, or any of the five jal targets not the expected one |
| 0x140EFC0 | camera_invisible_collision_check_direct_by_exception_obj | decomp_collision::cameraInvisible | decomp_collision.cpp | SOTC_CAMCOLL | on | The same for the exception-object form | Switch off or a jal target differs |
| 0x1133548 | GetArbitraryDirectionInverseMatrix | decomp_quat::invMatrix | decomp_quat.cpp | SOTC_CAMCOLL | on | The segment's frame (direction matrix, inverse, length in f0) | Switch off, or any pointer (or the 0x200 bytes below sp) outside RAM |
| 0x1BE898 | iosAbsVector | decomp_quat::absVector | decomp_quat.cpp | SOTC_CAMCOLL | on | abs of a vector (one per near part in the original loop; inline in obj125) | Switch off, or either pointer outside RAM |
| 0x11288D0, 0x1128918, 0x140FAB8, 0x1434FC0, 0x13D54F0 | the object filters (ExceptPluralObj, ExceptDescendPluralObj, not-a-character x2, not-the-player) | decomp_collision::objectFilterEntry<0..4> | decomp_collision.cpp | SOTC_CAMCOLL | on | Pure reads of the judge list / object type / a global | Switch off; ExceptDescend when a parent needs GetLayoutParentObj or the class needs a call; the two not-a-character filters when their jal is not GetCharacterClass |
| 0x1123C60, 0x10D41E0, 0x1434C00, 0x11292B0, 0x14336A0 | CameraLeave, charaSurfaceFilter, gamecoreSurfaceFilter, collisionNotKind28Filter, pathJudgeFunc | decomp_collision::filterEntry<0..4> | decomp_collision.cpp | always native | on | Id filters: reads of the surface's attribute bits | When the class is computed by a call (the attribute lookup fails) |
| 0x1123A58 | CollisionPlaneJudgeCallbackCameraVisibility | decomp_collision::cameraVisibilityEntry | decomp_collision.cpp | always native | on | Id filter (attribute bits) | Same as above |
| 0x1123850, 0x13B54C8, 0x1444800, 0x1395830, 0x13958B8, 0x110C790 | CameraMove, playerPlaneJudgeFuncCameraFilter, gamecore_sub_01444800, 01395830, 013958B8, manager_sub_0110C790 | filterEntry<5..10> | decomp_collision.cpp | SOTC_CAMCOLL | on | Id filters (attribute bits) | Switch off, or the attribute lookup fails |
| 0x13CB8E8, 0x13D5510 | gamecore_sub_013CB8E8, AI_JudgeColAttrFunc | filterEntry<11>, <12> | decomp_collision.cpp | SOTC_COLLPROBE | on | Id filters (attribute bits) | Switch off or lookup fails |
| 0x14383F0 | moveCtrlWallJudgeFunc | filterEntry<13> | decomp_collision.cpp | always native | on | The wall filter: the plane's slope in degrees (acosf through the table) against a1 + 0x40 | A plane not cached (47 of 44 500 calls on round 40's route) or the lookup failing: recompiled |
| 0x1374F08 | character_ikcolattrfilter | filterEntry<14> | decomp_collision.cpp | SOTC_LEAVES | on | The IK feet's id filter | Switch off or lookup fails |
| 0x10DF8F0 | manager_sub_010DF8F0 | filterEntry<15> | decomp_collision.cpp | SOTC_COLL71 | on | The slide callback's filter: 1 unless attribute bits 26..30 are 0x30000000 | Switch off or lookup fails |
| 0x10DC808 | charrootdyn_slideExceptionJudgeCB | decomp_hot47c::slideExceptionJudgeCB (table); also inline in idFilter under coll71 | decomp_hot47c.cpp, decomp_collision.cpp | SOTC_HOTLIST47 (table), SOTC_COLL71 (inline) | on | The landing's filter (attribute kind and bits 26..30) | Table entry: only with SOTC_HOTLIST47=0 |
| 0x10D9FC0 | GetCharaRootDynamics | rest81::rootDynamics (a probe wrapper) over decomp_hot47d::rootDynamics | decomp_rest81.cpp, decomp_hot47d.cpp | SOTC_HOTLIST47 | on | The character's root dynamics step by kind (the walkers' collision callers) | Only with SOTC_HOTLIST47=0 |
| 0x10DAE30 | GetCharaRootDynamicsAfter | decomp_hot47c::rootDynamicsAfter | decomp_hot47c.cpp | SOTC_HOTLIST47 | on | The root after the dynamics step | Only with SOTC_HOTLIST47=0 |
| 0x10BF8F0 | ExecCharaGeometry | decomp_charageo::execCharaGeometry | decomp_charageo.cpp | SOTC_MOTION | on | A character's frame; calls GetCharaRootDynamics, CalcRootFitting (0x10E21C0) and the IK as guest calls | not read in detail |

Not hand-written natives but in the area: CalcRootFitting (0x10E21C0) is a lifted function (SOTC_LIFTD4); so are
ClipCollisionDirectCallBackOneObj (D5), ClipCollisionDirect (D7), FinderWithKeep4DSlopeLimit (D8), 0111F688 (D4),
FinderWithVariableKeepCB (D7), FinderWithVariableKeep4DCB (D4), FinderWithKeepSlopeLimitCB (D7),
CalcCollisionMultiDepSurfBoundary 0x1124368 (D5), 0111DDC0 (D4), 01129258 (D2), DeleteCollisionFinder (D7 fallback copy),
and the guest's contact controllers ColCont_* / ColCont2_* (0x139A3D0-0x139AA50, D2 / D3 / D6 / D7). Still recompiled
(census list, no native or lift found before D9): ClipCollisionDirectExceptObj 0x1131C50, ClipCollisionDirectForParticle 0x11414E8,
manager_sub_011298A8, FinderWithKeepSlopeLimit 0x11263E8, the old tree walkers 0x112ACD0 / 0x112B890. SCRGetCollision
0x137D110 is lifted in D9. D9 also gives six of these natives lifted fallback copies (decomp_liftd9_90.cpp, SOTC_LIFTD9):
GetCollisionDBAttributeAdr, GetCollisionDBSurfBoundary, GetCollisionDBPlane, EntryCollisionFinder, moveCtrlWallJudgeFunc
and ClipCollisionDirectCallBackDoubleSide, so the "fallback" column above now means the lifted copy for them.

decomp_walk.cpp has no collision code (its "clip" functions are view culling: ClipLayoutObjModel, modelClipSolid). The
walkers' collision (round 71) lives in decomp_collision.cpp, decomp_hot44b.cpp and decomp_hot47c.cpp.

## The host-side systems built so far

| system | switch, default | owns | exact? | measured (round) |
|---|---|---|---|---|
| Native traversal and kept lookups (round 10-22) | none | Static "chains" per lane (plane table, boundary table, attribute record of a (class, part)) tagged by the guest-run epoch g_guestRuns: any entry from guest code or guest call starts a new epoch | Bit-exact (difftest 0 mismatches, rounds 22 / 39 / 40) | Shrine altar, throttled Vita3K 444 MHz: 94.2 -> 68.6 ms a frame (Collision 2026-10-21). Round 22: collision family 11% -> 9.35% of the game thread (Vita3K host profile); the walk out 4% faster at 222 MHz |
| NEON fast paths (rounds 28 / 49 / 54 / 58) | SOTC_FASTNATIVE(2/3), on | No data; plain NEON where every lane is finite and below 2^61 (the clamps are the identity) | Bit-exact (SOTC_COLLISION_FASTCHECK / SOTC_FASTNATIVE_CHECK: 0 mismatches in millions, round 28) | Round 28: no change Vita3K can see (it does not model NEON-to-ARM stalls); A9 gain unmeasured, estimated 1-2 ms in a fight |
| coll63: the leaf's plane test first | SOTC_COLL63, on | none | Exact (SOTC_COLL63_VERIFY: 2.14 M Mac + 0.86 M Vita3K checks, 0 differ) | Vita3K: game thread -0.6..-0.8 ms in the attract demo's flyover; no change in the title world (round 63) |
| coll71: root dynamics filters native, ClipCollisionFinderWithKeep native, backward duplicate search | SOTC_COLL71, on | none | Exact (0 mismatches) | Altar ~0.5 ms of the game thread (Vita3K 222, three runs); plain unchanged (round 71) |
| seg80: segment answers reused | SOTC_SEG80, on, but off under coll115 unless SOTC_SEG80=1 | 48 LRU slots keyed by caller, group, filters, user's six judge words; ends within 1 cm; at most 5 frames old; a separate path pool for probe84 | Not exact by design (blind to movers for up to 5 frames) | Altar +1.5..+2.2 ms, route 6 ~+1 ms, riding ~0 (Vita3K 222, round 80). Under coll115 (round 115): reuse off costs nothing riding / altar, ~0.5 ms on route 6's plain |
| probe84 | SOTC_PROBE84, on | Camera water search answer (0.25 units, 10 frames); AI path tests' own seg80 pool | Tolerance (memo) | Altar +0.76..+1.49 ms, route 6 ~+0.3, attract unchanged (Vita3K 222, round 84) |
| ground85 (+ sel96) | SOTC_GROUND85, on (SOTC_SEL96, on) | Per still object a patch of entered surfaces (sphere, plane, splits) from a step run with radius +1 unit, kept up to 60 frames; a host view of the list for the following clips | Designed to enter the same surfaces in the step's order; SOTC_GROUND85_CHECK compares the sets; exactness figures not in the sources read | Altar +1.0..+1.3 ms (five of six A/Bs), route 6 and attract unchanged (Vita3K 222, round 85) |
| census | SOTC_COLL_CENSUS, off (diagnostic) | Wraps 54 collision table entries; times outermost queries by (function, ra), counts repeats | n/a | Round 63: title world ~5 ms a frame, ~800 queries from ~40 call sites; round 80: altar segment tests ~4 ms; round 113: riding 5.4-6.7 ms, segment test ~4.8 ms |
| walk101 / leaf101 / skin101 / obj101 | SOTC_WALK101 / LEAF101 / SKIN101 / OBJ101, on | walk101: host stack of second sides (64 deep), host qword ends; leaf101: two clamped matrices per lane kept per epoch; obj101: the segment matrix's rows kept | Bit-exact (checks 0 differences in 0.3-2.2 M each) | Sampler: decomp_collision 18.96 -> 17.02% of the game thread with memos off, 10.95 -> 10.08 with them on; A/B within noise (round 101) |
| Per-thread natives (round 108) | lanes always on; nat108 / snap108 need SOTC_CHAR104=1 (char104 default 0 on the Vita since round 121) | One epoch, one set of kept matrices and chains per lane (g_guestRuns / g_guestRunsW); memos (seg80, probe84, find84, g85, ground85 capture, coll115, seg123, fin125) lane 0 only; kNat108 lists the collision natives allowed on the worker | Check: 0 differing live words in 129 batches | Worker natives worth 5-11 ms a frame of worker time at the altar; char104 still costs 9 ms at the altar, 5.6 on route 6 (Vita3K, round 108); char104 lost 0.4-1.1 ms on the console (round 121) |
| coll115: segment walk and leaves on host data | SOTC_COLL115, on | Per model (keyed by tree address) leaf blocks: 4 surfaces a group, SoA cx cy cz r nx ny nz d (32 bytes a surface), ids; budget SOTC_COLL115_MB = 2 MB (riding up to ~1.7 MB; console log ~1.2 MB, 51 models, round 123) | Answers the original's hit / no hit (candidates go to the original ClipCollisionDB); piece ends in plain floats: hit points within ~0.1-0.4 mm, one surface tie per ~500 000 walks, two edge hits in 250 000-550 000 walks; ends not small go to the original walk | Riding +0.4..+1.7 ms game thread, even at shrine / altar / route 6 (Vita3K, round 115). Per query riding 191-262 -> 125-149 us; walks 29-55 -> 13-15 us. Console (round 119's log): walks 17 vs 41 us, but queries 150 vs 110 us because seg80 is off with it |
| walk119 / part119 / view119 (coll119) / flush119 | all off by default | view119: per finder list a frame-long view of plane and flag addresses per entry | Exact | Each touched call 2-9% faster in-run; ~0.1-0.15 ms a frame of Vita3K time; frame A/B within noise; left off for the console to decide (round 119) |
| seg123: validated segment memo | SOTC_SEG123, off since round 131 | Kept answers per query; per-object fingerprints once a frame (256 entries; groups up to 48 objects); eps 1 cm (code default 0.01; the header still says 1 mm), age 60; callers keeping under 1/4 of 32 bypass for 96 | Not exact: 0 hit / no hit in 22 000-31 000 kept answers, other surface 2.8%, points up to 12.9 mm (round 123); round 131: 3 hit / no hit, 12 other surface, up to 44 mm in 8 783 | Route 6 plain: 142 vs 185 us a query (~0.55 ms a frame); riding slightly worse (round 123). Off since round 131 (gallop loss) |
| fin125: finder lists as a host surface cache | SOTC_FIN125, on (off in rounds 131-134, on again in round 135) | 512 slots of ~190 bytes (96 KB), open addressing 8 probes, LRU; per surface keyed by its two id words: plane, boundary sphere, first vertex, ClipCollisionDB's triangle terms; epochs: global, per collision work (512-entry table), worker | Bit-exact since round 135 (754 378 clips compared, 0 differences); round 125's p1 (plain floats) and round 131 (stale attribute address, replaced slots) were not | Per clip riding 30.2 -> 24.5 us (-19%) (round 125); collision family 12.50 -> 9.93% of the game thread riding (sampler); round 135: 36.07 -> 33.45 us with full validation; console: no sign of cost, A/B could not resolve it (round 138) |
| fin138: validate138 | SOTC_FIN138, on | Per slot a Meta of the chain links' values (type, partTab, t2, t2v8, rv, index); ~17 addresses prefetched together | Exact by construction (a differing link remakes); check: 0 unsafe in 2 350 s | Console reading via SOTC_FIN138_LOG; no number in the sources read (round 138) |
| obj125: object / part loop in host locals | SOTC_OBJ125, on | No persistent data; state in locals; cylinder in plain floats with radius +0.1% +1 mm | Answers the original's (0 differences in ~540 000 queries; round 131 again 292 246 queries, 0); the switch comment calls it "port, not exact" because of the conservative pre-tests | Per query riding 143.3 -> 126.6 us (-12%) (round 125); sampler: object / part loop 2.99 -> 2.03% |
| pf134 / pf135 / pfc141 | on | Prefetch only (obj125's next object, finder scan 4 surfaces ahead, next part record; pfc141 notes for the helper thread) | Exact (no writes) | Console: finderLeaf<true> 3.80 -> 1.26% of busy samples between r134 and r135 (attributed to pf135, round 138) |
| cammemo (round 65) | Tuning::sacCamColl, off | The camera checks' answers in odd frames | Not exact (frame skip) | not measured in the sources read |

Notes on coll115's limits (round 115): game thread only; trees with +0x10 set or a DB kind < 3 still use the recompiled
walkers; the class must have a uniform scale relative to the leaf's matrix (else the original); stack deeper than 128
goes to the original walk (deepest seen 15).

## Data (offsets as the code and its comments state them)

**Surface id (8 bytes)** (decomp_hot44a.cpp, decomp_hot43c.cpp, decomp_collision.cpp): +0 the object (layout id), +4 a
word: bit 31 set = an independent surface (vertices through 01114C70, kind 2 in a hit record), clear = a mesh / skinned
surface (vertices through 01116108, kind 3); bits 24-30 the part index; bits 0-23 the surface index. (Some headers say
"skinned" for the bit-31 case; the code is as stated here.)

**Object and layout class.** Object +8 the type byte; GetLayoutClass (0x10F1A20): types 9-22 index a table at
0x1194070: 0x10F1A58 -> object + 0x20, 0x10F1A98 -> object + 0x90, 0x10F1AA0 -> none, anything else is computed by a call
(the natives then fall back). Object +9 a walk count, +0xA bit 0 hidden (isysGroupForNext). Class fields read by the
natives: +0x90 the scale (the bound radius multiplier), +0xA0 a scale vector (the local hit point), +0x134 the position,
+0x158 the per-part matrices into the tree's space, +0x15C the per-part world matrices (0x40 each), +0x164 the per-part
world-to-local matrices, +0x16C / +0x170 part matrices (movedPoint), +0x174 the collision work, +0x184 the collision record
(+0x20 the DB), +0x194 the parent class, +0x1B8 / +0x1BB two enable bytes, +0x180 the cloth (decomp_hot46g.cpp,
decomp_collision.cpp, decomp_charageo.cpp).

**Collision DB** (class +0x184 -> +0x20): +0 the kind (int; < 3: the 8-byte tree of 0x112B890), +4 the part count, +8 the
parts (0x90 bytes each), +0x14 / +0x18 blend lists and the face records (0x40 each; +0x3C the attribute index of a skinned
face) used by 01116108, +0x1C the tree, +0x2C the bounding radius, +0x34 the attribute table (16-byte records; a 64-bit
word read by the filters), +0x38 / +0x3C the water triangles (count, table of 0x20 bytes: +4 height, +8 / +0xC origin xz,
+0x10..+0x1C two more xz). The ground probe reads +0x3C as ids (kind < 4) and +0x40 as a table (decomp_hot47j.cpp); the
two readings of +0x3C are not reconciled in the sources.

**Part record** (0x90 bytes): +4 the vertices (0 = no surfaces), +8 a byte: short (16-bit) ids, +9 a byte: packed int16
vertices (x 2^-15 x scale + offset), +0xC the scale / offset record (+0 offset, +0x10 scale), +0x10 the surface count,
+0x14 the surface list (short ids: 0x14 bytes a surface, +0 attribute index u16, +2..+6 three vertex ids, +8 three edge
link pairs; long ids: 0x34 bytes, +0..+8 vertex ids, +0xC three 12-byte edge records (link word, surface, part), +0x30
attribute index), +0x1C / +0x20 the part's sphere (radius, centre), +0x50 the skinned triangle count, +0x54 the skinned
index list (into DB +0x18), +0x58 the triangles' spheres, +0x5C / +0x60 the skinned group's sphere (decomp_collision.cpp
verticesFull / attributeAddress, decomp_hot46g.cpp, decomp_hot48c.cpp, decomp_finder.cpp).

**Tree** (DB +0x1C; decomp_collision.cpp header): +0 the surface ids (u16), +4 the leaves (u32: count in the low byte, the
first id's index above it; the finder's 0x1115A50 leaves are u16 count + u16 first), +8 the nodes, +0xC the root's index,
+0x10 non-zero = the old 16-byte node format (walked by 0x112ACD0). Node (8 bytes): +0 the split (float, times the split
scale), +4 a word: bits 0-1 the axis, bits 2-15 the low child (a leaf when bit 16 is set), bits 17-30 the high child (a leaf
when bit 31 is set).

**The segment test's parameters (the leaf's t1, built at sp + 0xD0 by the object loop):** +0 the object, +4 its class, +8
the boundary cylinder's height (the segment length), +0xC the matrix into the tree's space, +0x10 the matrix into the
cylinder's space (the segment's inverse matrix at the loop's sp + 0x40), +0x14 the split scale, +0x18 the side mode (1
both, 2 back, else front), +0x1C a surface filter, +0x20 an id filter, +0x24 the user word. The loop's own frame is 0x270
bytes (ClipCollisionDB's 0xF0 per call); the leaf's frame 0xD0 (sp, sp + 0x10 the segment in tree space, +0x20 / +0x30 the
surface's frame words read by the surface filter).

**Hit record (0x50 bytes):** +0 / +4 the surface id, +8 / +0xC u / v, +0x10 the local hit point of an independent surface
(through class +0x164 and x class +0xA0), +0x30 the kind (2 independent, 3 mesh), +0x40 the hit point.

**Collision work** (class +0x174): +0x10 the head of the finder entries' two-way links (EntryCollisionFinder), +0x1C a flag
cleared by the flush, +0x24 the per-part caches (0x60 bytes a part: +0x00 the vertex cache, +0x18 a fourth list, +0x30
the plane cache, +0x48 the boundary cache), +0x28 / +0x40 / +0x58 / +0x70 the work-wide caches (the skinned vertices'
blend cache at +0x28, the skinned planes at +0x58, the skinned boundaries at +0x70; the flush's flags pick +0x28 / +0x40
or +0x58 / +0x70).

**Modified manager (each cache)** (EntryModifiedManager / FlushModifiedManager): +0 the count, +8 the flag bytes (one per
entry: set = made this frame), +0xC the index size (1 / 2 / 4), +0x10 the index list, +0x14 the cached values (16 bytes
each: a vertex, a plane n.xyz / w = -n.v0, or a boundary centre / radius). Flushing clears the listed flags; the next
query remakes the value. This is the game's per-frame cache that every fin125 / view119 / coll115 lifetime rule tracks.

**Finder** (EntryCollisionFinder, CheckCollisionFinderExistData, ClipCollisionFinder): +0 the count, +4 the capacity (the
smaller of the two bounds the list), +8 the keep index (GetCollisionDBCallBack), +0xC the ids (8 bytes each), +0x10 the
links (12 bytes each: +0 / +4 next / prev, +8 the id's address). **Finder query** (the finder leaf's a0): +0 the finder, +4
the object's id, +8 its layout class, +0xC / +0x30 the filters' words, +0x18 the radius, +0x20 the centre, +0x28 a boundary
filter, +0x2C an id filter, +0x34 the first new index.

**Keep results:** ClipCollisionFinderWithKeep's a0: +0 the point, +0x10 the plane, +0x20 the id pair. The 4D slope form's
kept planes: +0x20 count, +0x24 records of 0x14 bytes.

**Judge data (six words)** (ConstructorCollisionObjectJudgeCallbackData 0x1128898, judgeData()): +0 the count, +4.. the
objects to leave out; the object filters read it as the user word; seg80 / seg123 key on all six words.

**Geometry pointer:** SCRGetCollisionCallBack copies 64 bytes (CopyGeometryPointer 0x11327B0) on a hit; its layout is not
stated. GetGeometryPointerCollisionDB is named by the ground probe; not stated further.

**isys groups:** entries at 0x21B900, 24 bytes: +0 the object, +0xC a "being deleted" byte, +0xE the next index (u16)
(groupNext in decomp_collision.cpp). The collision object group seen by round 108 is 0x146588C.

**fin138's chain (coll125.h, round 138):** object +8 -> class (+0x174 work, +0x184 record); a mesh surface: record +0x20,
its +8, the slot list and the index; an independent one: the work's part table; then the flag and slot bases, the vertex
cache list, the five flags and the plane / boundary / three vertex qwords.

**Water work block** (01117340): at 0x118CE20 (relocated): +0 origin, +0x10 / +0x20 edges, +0x30 height, +0x40 crossing.

## Cost: what earlier rounds measured

| where | figure | source |
|---|---|---|
| Console, field riding / shrine | collision 15.9% of the game thread = 9.6 ms (of 60.5) riding; 11.0% = 4.1 ms at the shrine (Vita3K sampler shares times the console's cpu game) | round 113 |
| Console, segment queries | 150 us each with coll115 vs 110 us without (seg80 answering repeats); walks 17 vs 41 us; 3.7 vs 1.4 walks a query | round 119's console log, read in round 123 |
| Console, round 124 log | segment queries 157 us, ~8.6 a frame (1.36 ms); finder clips 15.3 us, ~88 a frame (1.35 ms); walks 17 us | round 125 |
| Console profile (r132, riding) | directCallBack (obj125's loop) 3.6% | round 134 |
| Console profile (r134) | finderLeaf<true> 3.35%, directCallBack 3.23%, finderClip<true> + <false> 2.0% | round 135 |
| Console, r134 vs r135 whole runs | collision 7.38 -> 5.67 ms a frame; finderClip<true> 1.19 -> 0.89%, <false> 0.88 -> 0.60%, clipKeep 0.49 -> 0.29%, finderLeaf<true> 3.80 -> 1.26%; prof129 windows 3.73-7.42 ms a frame following the scene | round 138 |
| Console vs Vita3K (r135 build) | collision 12.5% of busy samples on the console, 13.4% in Vita3K | round 141 |
| Vita3K host profile, 222 MHz | collision family ~11% of the game thread -> 9.35% (round 22); 12.8% on the walk out (round 28); collision probes 2.0 / 5.6 / 5.4 ms a frame (shrine / walk / plain, SOTC_SUBSYS_PROF) | rounds 22, 28 |
| Vita3K census | title world ~5 ms a frame (~800 queries, ~1 400 node visits, ~100 leaves, ~1 100 surfaces, ~620 ClipCollisionDB, 86% ending at the plane test) | round 63 |
| Vita3K, altar | Wander's ExecCharaGeometry 7.4 ms of which GetCharaRootDynamics 4.35 (GetCollisionDBCallBack ~1.8, keep / slope ~1.5, slide / landing ~1.5); segment tests ~4 ms (~14 a frame) | round 80 |
| Vita3K sampler, route 6 | decomp_collision 18.96% of the game thread with memos off, 10.95% with them on | round 101 |
| Vita3K census, riding | 5.4-6.7 ms a frame, segment test ~4.8 ms (~19 a frame, 150-350 us each), ground fill 0.6 ms | round 113 |
| Vita3K sampler, route 6 plain standing | collision ~15% of the game thread, flat, no function above 3% | round 119 |
| Vita3K sampler, route 6 plain (celosia-r123) | collision ~14%: object / part loop 2.52, ClipCollisionDB 1.50, coll115 walk / leaves 1.6, skinned finder 0.67 + corners 0.30, ground fill 0.70, clipKeep 0.45, attribute lookups 0.56, flushes 0.53 | round 125 |
| Vita3K sampler, riding | collision family 12.50 -> 9.93% with fin125 + obj125 | round 125 |
| Prof129 vs host sampler (Vita3K attract) | collision 6.0% (prof129) / 8.4% (host sampler) of the busy game thread | round 129 |

Unknown or not stated in the sources read: a console per-call reading of fin138 (the round 138 build asks for it), the
console verdict on the round 119 switches, the console gain of the NEON fast paths, and any measurement of colossus fights
(no save near a colossus until D8's SOTC_TELEPORT).

## Where the area stands after D9

On D8's list of 319 functions (decomp_status's area before D9's files moved two of them by source): **169 native after
D9** (158 after D8, + 11 shipped in D9's batch), 150 recompiled. Three of the 169 natives still run their original code
on some inputs, because their fallback copies were not trace-checked (CollisionPlaneJudgeCallbackCameraVisibility,
CollisionPlaneJudgeCallbackCameraLeave, gamecore_sub_013CB8E8: the camera's and an AI's id filters, when the attribute
lookup fails), so **166 of 319 (52.0%) are native in every path** (149, 46.7%, after D8 on the same count). decomp_status's
own figure for the area is 167 of 317 (50.1% by code size): the lifted fallback copies of ClipCollisionDirectCallBackDoubleSide
and moveCtrlWallJudgeFunc live in decomp_liftd9_90.cpp, so the tool now files those two by name (display, math).

D9 shipped in the area: the fall / landing transitions (motrootdyn_startxzfit, motrootdyn_startxz_from_xzfit,
motrootdyn_parent_from_falldown, motrootdyn_startxzfit_from_falldown), two of the landing's ground fits
(charrootdyn_getCompleteFittingYQFromStabGPWithSlopeLimit_X, charrootdyn_getCyclicCollisionDetectZYOneWayComplexCB),
GetCollisionEdgeLinkDBVWallFinder (the edge links' wall finder, every frame for Wander), SCRGetCollision, pathDetectCollision
and the AI move work's two volume setters; and fallback copies for six collision natives (GetCollisionDBAttributeAdr,
GetCollisionDBSurfBoundary, GetCollisionDBPlane, EntryCollisionFinder, moveCtrlWallJudgeFunc,
ClipCollisionDirectCallBackDoubleSide), so their slow paths no longer run generated code.

**What the 150 recompiled ones are, and what would reach them** (the table's last column has each one):

| group | functions | what would reach them |
|---|---:|---|
| reached in D9, never at top level in a trace pass | 6 | the landing group's callees (checkIYplane4D, the VWall direction status, the fall-down landing offset, the lower-collision reset, the slope-limit fit, xzfit from xz): a pass whose route lands often and lists only them |
| reached only in dv1's lake run | 23 | pendulum, the parent point modes (parentpointxzfit, charrootdyn_parentpointxzfitstab, updateParentPointGPInfo and the motrootdyn_startparentpointxzfit_* transitions), the edge point (charrootdyn_edgepointstab, startxz_from_edgepoint, startxzfit_from_edgepoint), pendulum_test: Wander hanging from a ledge or on a moving body. A recorded input that climbs (SOTC_INPUT_RECORD, then SOTC_INPUT_REPLAY on the Mac with the trace check) would reach them every time |
| Wander's other root dynamics modes | 32 | the chain hang, the VCOL pendulum, the hit-collision changes, the fitting variants, swim checks: climbing a colossus' fur, hanging from a chain, diving |
| the fights | 22 | SCRConditionSendDamageToCollisionObject / 2 / 3, WeaponHitAttr, SwordFlareHitColAttr, MyselfKeepHit, NICO_is_quake_attack_hit, the named colossi's scripts (Tortoise, Yamori, Snake, Narga, Worm), the falling rocks, the white fade of a colossus' death: a colossus fight, recorded once |
| judge volumes | 18 | the volume tests (cube, cylinder, elliptical sphere, their in-out rates) and the scripts' volume commands: arenas whose scripts use trigger volumes |
| collision library calls no route's caller makes | 25 | AddCollisionDB / GetCollisionDB, the ray volume's free point, the multi-dependency boundary, the DB's surface links, the geometry pointer's edge exchange, CheckCollisionColClass, the judge volume class getters: the gameplay that calls them (fights, scripts) |
| debug and test | 15 | the dynamics test object (nico_pendulumtest, dynamicstest and its getters), debug draw modes, the wide-area search (a script command no route's scripts use), getCollisionDisableAtDirectPlayFlag (no caller at all) |
| the wind emitters' judge-volume kind | 4 | an area whose wind uses judge volumes |
| others | 5 | the AI's physical op and six-direction probe, GAMEUTIL helpers, the weapon offset getter |

**Getting the area to 100% needs recorded gameplay, not more blind input.** One recorded session from the console card with
the teleport (Avion's lake: swim to a pillar, climb it, jump on Avion, hang, stab; then a death) would reach most of the
hanging, parent-point and fight groups; a second at Valus (new game) adds the first ledge climb. The Mac replay of such a
recording (tick-keyed, its CD reads and timers in step) is what makes a trace pass see them. The debug / test group and
the never-called functions (about 15) cannot be reached by play at all.

## Cost after D9

The Vita3K sampler (222 MHz, 3 cores) on D9's A/B runs, the game thread's samples between 120 and 1 440 s, every host
symbol of the collision natives (decomp_collision, coll115 / coll125, decomp_finder / skinfinder / finderwalk, the
round 43-48 natives that hold collision code) plus the area's guest functions (lifted or recompiled):

| run | collision's share of the game thread | the biggest parts |
|---|---:|---|
| mixA: the attract demo (the long gallop, the canyon) | **13.7%** | decomp_collision 5.15% (the segment tests, the leaf, ClipCollisionDB, the finder clips), coll115 2.14% (the host trees' walks), the ground fill and probe (decomp_hot47j) 1.74%, fin125's cache 0.94%, the finder tree node / edge info (hot48c) 0.83%, clipKeep / movedPoint (hot44a) 0.55%, the keep pass (hot44b) 0.44%, the skinned finder 0.39%; the guest functions together under 0.5% |
| mixB: Avion's lake (bow, swimming, back to the shrine) | **21.6%** | decomp_collision 9.55%, hot47j 2.73%, fin125 2.40%, coll115 1.80%, hot48c 0.88%, hot44a 0.75%, hot44b 0.60%, the slope probe (hot43c) 0.55%; the guest functions under 0.6% (the volume checks appear here: CheckCubeVolume, GetRateInOutEllipticalSphereVolumeWithInverseMatrix) |

The console's own figure (round 138's whole runs) is 5.7 ms a frame after fin125 / pf135, 12.5% of its busy samples
(round 141). **Almost all of collision's time is host code already**: what S2 can win is in the algorithms (the segment
tests' object / part / tree walk, the finder's per-frame fill), not in the remaining recompiled functions, which are rare
paths (hanging, fights, volumes).

## Every function of the area

decomp_status's "Collision and physics" area as decomp_status files it after D9 (317 functions: ClipCollisionDirectCallBackDoubleSide and moveCtrlWallJudgeFunc moved out by source, see the natives table). "now" is the status after D9: the source file that
replaces it, or recompiled. Callers are direct `jal`s only (the root dynamics' mode functions have none: the motion
data calls them through a table). "touches" lists the argument registers' offsets (R / W and the access size) and the
globals (& an address taken, R / W a load or a store), read in linear order. The last column says, for every function
that was recompiled after D8, what reached it in D9 or what would reach it.

| address | function | size | now | direct callers | callees | touches | reached by / would reach it |
|---|---|---:|---|---|---|---|---|
| 107f150 | GAMEUTIL_get_parent_collision_attribute | 24 | decomp_liftd3_00.cpp | GAMEUTIL_is_parent_collision_attribute_boss, GAMEUTIL_set_col_plane_info, NICO_is_quake_attack_hit, NICO_is_quake_attack_hit_except_boss, NICO_is_quake_attack_hit_only_boss, doit_camera_etc_condition | GetParentCharColAttr | - |  |
| 10c2ec0 | getCollisionDisableAtDirectPlayFlag | 16 | recompiled | - | - | globals: 0118c4fc:& | no caller in the recompiled code (only a neighbour falls through to it): a debug flag getter |
| 10c2f88 | fittingwithrot | 784 | recompiled | - | ClipCollisionFinder, GetDifferencialQuaternionWithNoRegularize, GetMotionNodeBasePos, GetMotionNodeParentBaseQuaternion | $a1: +0x0:R16; $a2: +0x0:R16 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10c3328 | getoptnodephysicalnodeid | 56 | recompiled | - | GetOptionNodePhysicalNodeId | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10c6710 | GetOptionNodePhysicalNodeId | 136 | decomp_motion.cpp | GetCharacterClassMotionPhysicalNodeID, GetCharacterClassOptionNodeGeo, getoptnodephysicalnodeid, manager_sub_010BC4F0 | GetMotionNbOptionNodes, GetMotionOptionNodePhysicalId, GetMotionVersion | $a0: +0x0:R4 |  |
| 10d43e8 | charrootdyn_getCollisionDB | 760 | decomp_liftd9_90.cpp | edgepoint, edgexz, gamecore_sub_0133A048, gamecore_sub_01463D10, manager_sub_010D8340, manager_sub_010E9A50 +7 | GetCharacterClass, GetCollisionDBCallBack, memset | globals: 001e1000:&R 010d41e0:& 010df590:& 010df5d0:& |  |
| 10d46e0 | charrootdyn_getParentedEdgeInfo | 472 | decomp_liftd7_02.cpp | edgepoint, pendulum | CompletePlaneD, GetCollisionDBPlane, GetCollisionEdgePointerVertexPoint3, GetPlaneSlopeDegree, GetPointDistanceFromPlane | $a0: +0xf0:R4 +0x124:R4 +0x610:W4 +0x624:R4/W4 +0x68c:W4; globals: 001e0fb0:&R |  |
| 10d5750 | charrootdyn_updateParentPointGPInfo | 752 | recompiled | manager_sub_010E35B8, parentpointxzfit | ClipCollisionFinderCallBack, GetGeometryPointerPosition, GetGeometryPointerSurfaceAttribute, GetGeometryPointerSurfacePlane, GetPlaneSlopeDegree +4 | $a0: +0x134:R4 +0x138:R4 +0x13c:R4 +0x690:W8 +0x698:W8 +0x6a0:W16 +0x6b4:W4 +0x6bc:W4 +0xe34:R4; globals: 001e0fd0:&R 010df8f0:& 0118ca30:&RW 0118ca38:W | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10d5a40 | charrootdyn_getChainStatus | 432 | decomp_liftd8_00.cpp | gamecore_sub_0133A048, manager_sub_010D8340, swim, xz | CheckClothChainDetect, ClipCollisionFinderCallBack, CopyGeometryPointer, GetCharacterClass, GetGeometryPointerPosition +5 | globals: 010dc808:& |  |
| 10d5bf0 | charrootdyn_getNipPlaneStatus | 384 | decomp_liftd3_01.cpp | charrootdyn_setLandingStatus, manager_sub_010E9A50, manager_sub_010EA718, pendulum, xz | ClipCollisionFinderWithKeepSlopeLimit, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, GetPlaneSlopeDegree, iosGetStabXZVector | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0x10:W16 |  |
| 10d62f8 | charrootdyn_HslidecollisionCB | 2272 | decomp_liftd6_00.cpp | manager_sub_010D8340, norootdynamics, swim | ClipCollisionFinderWithKeep4DSlopeLimit, ClipCollisionFinderWithKeep4DSlopeLimitCB, ClipCollisionFinderWithKeepSlopeLimit, ClipCollisionFinderWithKeepSlopeLimitCB, GetLayoutClass +6 | $a0: +0x0:R16/W16 +0x30:W16 +0x34:R4 +0x80:R4/W4 +0x84:R4/W4; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 01193948:R |  |
| 10d6bd8 | charrootdyn_setkeepcolhitstatus | 600 | decomp_liftd6_00.cpp | charnodeparent, edgepoint, edgexz, manager_sub_010D8340, manager_sub_010E9A50, manager_sub_010EA718 +4 | AddTwoWayLinkNext, DeleteTwoWayLink, GetLayoutClass | $a0: +0xff0:R4/W4 +0xff4:W4; $a1: +0x0:R4 +0x4:R4; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 10d6e30 | charrootdyn_slidecollisionCB | 912 | decomp_liftd6_00.cpp | charnodeparent, charrootdyn_slidecollision, manager_sub_010D7FA8, norootdynamics | ClipCollisionFinderWithKeep4DSlopeLimit, ClipCollisionFinderWithKeep4DSlopeLimitCB, GetLayoutClass, GetPointDistanceFromPlane, SlideCollisionFinder4D +2 | $a0: +0x0:W16 +0x10:W16 +0x30:W16 +0x80:R4/W4 +0x84:R4/W4; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 10d7a70 | charrootdyn_getCompleteFittingYQFromStabGPWithSlopeLimit_X | 1336 | decomp_liftd9_00.cpp | manager_sub_010E22F8 | GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, GetPlaneFittingPoint, manager_sub_010D71C0 | $a0: +0x0:R16/W16; $a1: +0x40:R16; $a2: +0x0:W4; globals: 0118ca70:&RW 0118ca80:&R 0118ca90:&R 0118ca98:W | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10d8158 | charrootdyn_setLandingStatus | 488 | decomp_liftd7_02.cpp | manager_sub_010D8340 | GetCharacterClass, GetCollisionDBCurrentPointOnLastPlane, GetGeometryPointerPosition, GetGeometryPointerSurfaceAttribute, GetGeometryPointerSurfacePlane +9 | globals: 001e0f90:&R |  |
| 10d9368 | charrootdyn_parentHColGp | 760 | recompiled | - | GetCharacterClass, GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, ParentLayoutObj +2 | - | standing / climbing on a moving body (a colossus): the parent point modes |
| 10d9aa0 | norootdynamics | 1312 | decomp_liftd6_00.cpp | GetCharaRootDynamics, norootdynamics_vel, norootdynamics_vel_xzstab | CheckUnderVariableXZSurface, ClipCollisionFinderCallBack, ForceUnparentLayoutObj, GetCharaVCollisionStatus, GetCharacterClass +7 | globals: 001e0f90:&R 010dc808:& |  |
| 10db820 | charrootdyn_getCyclicCollisionDetectZYOneWay | 840 | recompiled | - | ClipCollisionFinderCallBack, iosGetTableCos, iosGetTableSin | $a1: +0x0:R16; $a2: +0x0:R16; globals: 010df8f0:& | standing / climbing on a moving body (a colossus): the parent point modes |
| 10dbb68 | charrootdyn_getCyclicCollisionDetectZY | 912 | recompiled | charrootdyn_parentpointxzfitstab, manager_sub_010E4590, manager_sub_010E9578 | ClipCollisionFinderCallBack, iosGetTableCos, iosGetTableSin | $a1: +0x0:R16; $a2: +0x0:R16; globals: 010df8f0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10dc020 | charrootdyn_slidecollision | 96 | decomp_liftd4_01.cpp | edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, parentpointxzfit, xz | charrootdyn_slidecollisionCB | globals: 010dc808:& |  |
| 10dc080 | charrootdyn_getCollisionDBwithFunc | 344 | recompiled | - | GetCollisionDBCallBack | globals: 010d41e0:& | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dc1d8 | charrootdyn_getInterpVector | 72 | decomp_liftd4_01.cpp | edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, parentpointxzfit, pendulum +1 | - | $a0: +0x0:W16 +0xc:W4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 10dc220 | charrootdyn_getInterpPoint | 80 | decomp_liftd2_00.cpp | GetCharaRootDynamics, charrootdyn_memoryposition, parentpointxzfit | - | $a0: +0x0:W16 +0xc:W4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 10dc270 | charrootdyn_getGPPlaneInfo | 200 | decomp_liftd2_00.cpp | manager_sub_010E9A50, manager_sub_010EA718, pendulum, xz | GetGeometryPointerSurfaceAttribute, GetGeometryPointerSurfacePlane, GetPlaneSlopeDegree, GetPlaneZTopYPlaneCoordQuaternion, GetTurnYAngleFromGlobalDir | $a0: +0x13c:R4 +0x690:W8 +0x698:W8 +0x6a0:W16 +0x6b4:W4 +0x6bc:W4 |  |
| 10dc338 | charrootdyn_getGPPlaneInfoWithVPlane | 184 | decomp_liftd7_02.cpp | edgepoint | GetGeometryPointerSurfaceAttribute, GetPlaneSlopeDegree, GetPlaneZTopYPlaneCoordQuaternion, GetTurnYAngleFromGlobalDir | $a0: +0x13c:R4 +0x690:W8 +0x698:W8 +0x6a0:W16 +0x6b4:W4 +0x6bc:W4 |  |
| 10dc3f8 | charrootdyn_checkHangableEdgeWall | 88 | recompiled | pendulum | GetCollisionDBAttributeAdr | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dc608 | charrootdyn_checkZplane | 344 | decomp_liftd7_02.cpp | edgepoint, parentpointxzfit | ClipCollisionFinder | $a1: +0x134:R4 +0x138:R4 +0x13c:R4 +0xe34:R4; globals: 001e0f90:&R 001e0fd0:&R |  |
| 10dc760 | charrootdyn_setHColFindStatus | 96 | decomp_liftd8_00.cpp | edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, parentpointxzfit, pendulum +4 | LinkSecureParentInfoByGP, UnlinkSecureParentInfo | $a0: +0xfc0:W4 |  |
| 10dc7c0 | charrootdyn_getLandingSurface | 72 | decomp_liftd8_00.cpp | edgepoint, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxz_from_edgepoint, motrootdyn_startxz_from_parentpoint, motrootdyn_startxz_from_pendulum +5 | ClipCollisionFinderCallBack | $a3: +0x0:R16; globals: 010dc808:& |  |
| 10dc808 | charrootdyn_slideExceptionJudgeCB | 144 | sotc_worker.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 10dc898 | charrootdyn_getSlideCollisionJudgeCBArg | 48 | recompiled | pendulum | - | $a0: +0x0:W4 +0x4:W4; $a1: +0x68:R4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10dcb10 | charrootdyn_addSlideElement | 520 | decomp_liftd8_00.cpp | manager_sub_010E9A50, manager_sub_010EA718, xz | GetGeometryPointerSurfacePlane, GetGeometryPointerSurfaceSlopeDegree, GetPlaneFittingVector, iosFastPowf | $a0: +0x0:R16/W16; $a1: +0x0:W16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 001e0f90:&R |  |
| 10dcdc0 | charrootdyn_getvelocity | 56 | decomp_liftd2_00.cpp | edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, nico_balltest, parentpointxzfit +3 | - | $a0: +0x0:W16 +0xc:W4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 10dcdf8 | charrootdyn_addWindDynamicsToRoot | 280 | decomp_liftd2_00.cpp | manager_sub_010E9A50, manager_sub_010EA718, swim, xz | GetGeometryPointerSurfacePlane, GetPlaneFittingVector | $a0: +0x124:R4 +0x134:R4 +0x19c:R4 +0x1c0:R4 +0xc95:R1/W1 |  |
| 10dcf10 | charrootdyn_getPlaneYFittingVector | 192 | decomp_liftd3_01.cpp | xz | GetDifferencialQuaternionWithNoRegularize | $a0: +0x0:W16; $a2: +0x0:R16; globals: 001e0fb0:& |  |
| 10dcfd0 | charrootdyn_getLocalOffsetFromParentGP | 368 | decomp_liftd4_01.cpp | charrootdyn_parentpointxzfitstab, edgepoint, edgexz, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_setupedgepoint +9 | GetGeometryPointerPosition | $a0: +0x0:W16; $a1: +0x0:R16 |  |
| 10dd140 | charrootdyn_getCompleteFittingYQFromYDir | 280 | decomp_liftd6_00.cpp | charrootdyn_parentpointxzfitstab, manager_sub_010D1100, manager_sub_010E4590, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startedgepoint_from_pendulum_with_edge +6 | GetDifferencialQuaternionWithNoRegularize | $a0: +0x0:R16/W16; $a2: +0x0:R16 |  |
| 10dd258 | charrootdyn_findLocalYGeometryPoint | 192 | recompiled | manager_sub_010E8560 | ClipCollisionFinder | $a1: +0x0:R16; $a2: +0x0:R16 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10dd350 | charrootdyn_checkIYplane4D | 352 | recompiled | edgexz, manager_sub_010E9A50, manager_sub_010EA718 | ClipCollisionFinderCallBack | $a1: +0x134:R4 +0x138:R4 +0x13c:R4 +0xe34:R4; globals: 001e0fd0:&R 010df8f0:& 0118ca30:&RW 0118ca38:W | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 10dd4b0 | charrootdyn_checkIYplane3D | 352 | recompiled | - | ClipCollisionFinderCallBack | $a1: +0x134:R4 +0x13c:R4 +0xe34:R4; globals: 001e0fd0:&R 010df8f0:& 0118ca30:&RW 0118ca38:W | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dd610 | charrootdyn_checkIYPlane3DWithPos | 344 | recompiled | - | ClipCollisionFinderCallBack | $a1: +0x13c:R4 +0xe34:R4; $a2: +0x0:R16; globals: 001e0fd0:&R 010df8f0:& 0118ca30:&RW 0118ca38:W | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dd768 | charrootdyn_getCompleteFittingYQFromStabGPWithSlopeLimit | 400 | recompiled | charrootdyn_parentpointxzfitstab, manager_sub_010E8560, motrootdyn_startxzfit_from_xz | GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, getAverageNormal | $a0: +0x0:R16/W16 | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 10dd8f8 | charrootdyn_getCompleteFittingPlaneFromStabGPWithSlopeLimit | 432 | decomp_liftd4_01.cpp | manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxzfit | CompletePlaneD, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, GetPlaneFittingDiffVector, GetPointDistanceFromPlane +1 | $a1: +0x0:W16; globals: 001e0f90:&R |  |
| 10ddaa8 | charrootdyn_getGPRelativePositionWithQ | 184 | decomp_liftd4_01.cpp | charrootdyn_parentpointxzfitstab, manager_sub_010E9A50, manager_sub_010EA718 | GetGeometryPointerPosition | $a0: +0x0:R16/W16; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 10dde08 | charrootdyn_setEdgeUpperPlaneInfoFlag | 16 | recompiled | - | - | $a0: +0x20:W4 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dde18 | charrootdyn_resetEdgeUpperPlaneInfoFlag | 8 | recompiled | - | - | $a0: +0x20:W4 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dde20 | charrootdyn_setVColFindFlag | 184 | decomp_liftd7_08.cpp | manager_sub_010E9A50, manager_sub_010EA718, player_setDangerEdgeOtherSidePlanePoint, player_setDangerEdgePoint, player_setEdgeHangPointGeometry, player_setGripHangPointGeometry | AddTwoWayLinkNext, CopyGeometryPointer, DeleteTwoWayLink, GetGeometryPointerObject, GetGeometryPointerSurfaceAttribute +2 | $a0: +0xf3c:R4/W4 +0xf40:W8 +0xf48:W4 +0xf4c:W4; globals: 001e0f90:& |  |
| 10dded8 | charrootdyn_resetHColFindFlag | 24 | decomp_liftd8_00.cpp | FlushCharacterObj | UnlinkSecureParentInfo | - |  |
| 10ddef0 | charrootdyn_resetVColFindFlag | 56 | decomp_liftd7_02.cpp | FlushCharacterObj | DeleteTwoWayLink | $a0: +0xf3c:R4/W4 |  |
| 10ddf28 | charrootdyn_resetVColEdgeFindFlag | 56 | decomp_liftd4_01.cpp | FlushCharacterObj, player_setDangerEdgeOtherSidePlanePoint, player_setDangerEdgePoint | DeleteTwoWayLink | $a0: +0xe70:R4/W4 |  |
| 10ddf60 | charrootdyn_resetChainFindStatus | 24 | decomp_liftd4_01.cpp | FlushCharacterObj | UnlinkSecureParentInfo | - |  |
| 10ddf78 | charrootdyn_resetkeepcolhitstatus | 56 | decomp_liftd4_01.cpp | FlushCharacterObj | DeleteTwoWayLink | $a0: +0xff0:R4/W4 +0xff4:W4 |  |
| 10ddfb0 | charrootdyn_restoreposition | 384 | decomp_liftd8_00.cpp | manager_sub_010E9A50, manager_sub_010EA718, parentpointxzfit, swim, xz | GetCharacterClass, GetGeometryPointerPosition, UnlinkSecureParentInfo | $a0: +0x0:W16; $a3: +0x0:R16 |  |
| 10de1a0 | charrootdyn_getRegularColFinderPosition | 184 | decomp_liftd2_00.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, GetCharaRootDynamics, InitCharacterObj, SetObjectGeo, SetObjectGeoAndUnparent +17 | - | $a0: +0x0:W16; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 10de258 | charrootdyn_getCollisionAttribute | 40 | decomp_liftd2_00.cpp | gamecore_sub_0137B400, gamecore_sub_013B5FB0, gamecore_sub_013FCCE0 | GetGeometryPointerSurfaceAttribute | - |  |
| 10de340 | charrootdyn_getForceLimitMode | 40 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0xc30:R4 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10de368 | charrootdyn_getNoLimitMode | 24 | decomp_liftd4_01.cpp | manager_sub_010E22F8, manager_sub_010E8560, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxzfit, motrootdyn_startxzfit_from_xz | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0xc30:R4 |  |
| 10de380 | charrootdyn_getForceNoLimitMode | 24 | recompiled | charrootdyn_parentpointxzfitstab, manager_sub_010E8560, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxzfit_from_xz | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10de398 | charrootdyn_getLimitMode | 64 | decomp_liftd4_01.cpp | charrootdyn_parentpointxzfitstab, manager_sub_010E4590, manager_sub_010E8560, manager_sub_010E9A50, manager_sub_010EA718, motrootdyn_startxzfit_from_xz | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0x400:R4 +0x52d:R1 +0xc30:R4 |  |
| 10de3d8 | charrootdyn_checkSlopeLimit | 392 | decomp_liftd4_01.cpp | manager_sub_010E9A50, manager_sub_010EA718 | GetGeometryPointerSurfacePlane, GetPointDistanceFromPlane, iosGetTableCos | $a1: +0x0:R16; $a3: +0x0:R4 +0x8:R4 |  |
| 10de670 | charrootdyn_getCyclicCollisionDetectZYOneWayCB | 1232 | recompiled | gamecore_sub_013CC788 | ClipCollisionFinderCallBack, iosGetTableCos, iosGetTableSin | $a1: +0x0:R16; $a2: +0x0:R16 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10deb40 | charrootdyn_getCyclicCollisionDetectZYOneWayComplexCB | 920 | decomp_liftd9_00.cpp | manager_sub_010E22F8 | ClipCollisionFinderCallBack, iosGetTableCos, iosGetTableSin | $a1: +0x0:R16; $a2: +0x0:R16; globals: 010df8f0:& | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10defd0 | charrootdyn_detectCharEdge | 352 | decomp_liftd3_01.cpp | xz | GetCollisionDBEdgeInfoNext3, manager_sub_010D5130, manager_sub_010D5308 | $a0: +0x20:W4 +0x38:W4 +0x3c:W4 +0x80:W4 +0x8c:W4 +0x98:W4 |  |
| 10df130 | charrootdyn_isParentToEdge | 32 | recompiled | motrootdyn_startedgepoint_from_pendulum_with_edge, pendulum | - | $a0: +0xf0:R4 +0x124:R4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10df150 | charrootdyn_getGPPlaneInfoByCollisionDBData | 200 | recompiled | pendulum | GetCollisionDBAttributeAdr, GetCollisionDBPlane, GetPlaneSlopeDegree, GetPlaneZTopYPlaneCoordQuaternion, GetTurnYAngleFromGlobalDir | $a0: +0x13c:R4 +0x690:W8 +0x698:W8 +0x6a0:W16 +0x6b4:W4 +0x6bc:W4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10df310 | charrootdyn_getLocalOffsetFromParentGPwithGPCoordinate | 240 | decomp_liftd7_90.cpp | charrootdyn_setFallDownLandingOffset, charrootdyn_setLandingStatus | GetGeometryPointerQuatPos, GetMatrixFromQuatPos | $a0: +0x0:W16 |  |
| 10df400 | charrootdyn_setFallDownLandingOffset | 72 | recompiled | player_setEdgeHangPointGeometry, player_setGripHangPointGeometry | charrootdyn_getLocalOffsetFromParentGPwithGPCoordinate | $a0: +0x134:R4 | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 10df6d0 | charrootdyn_exchangeEdgePointOtherSideSurface | 112 | recompiled | - | ExchangeGeometryPointerCollisionEdgeOtherSideInfo, GetCharacterClass, motrootdyn_setupedgepoint | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10df740 | charrootdyn_checkParentGPPlaneDir | 432 | recompiled | - | ClipCollisionFinder, GetGeometryPointerSurfacePlane, GetPlaneFittingVector | $a1: +0x124:R4 +0x134:R4 +0x13c:R4 +0xe34:R4; globals: 001e0fd0:&R | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10df8f0 | manager_sub_010DF8F0 | 56 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 10df928 | charrootdyn_memoryposition | 472 | recompiled | - | GetCharacterClass, LinkSecureParentInfoByDBPos, SetGeometryPointerGlobalState, UnlinkSecureParentInfo, charrootdyn_getInterpPoint | $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dfb00 | charrootdyn_setChainFindStatus | 64 | recompiled | - | LinkSecureParentInfoByGP, UnlinkSecureParentInfo | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10dfb40 | motrootdyn_startfreefall | 72 | decomp_liftd7_03.cpp | - | charrootdyn_updateCharacterClassParentGeometry, manager_sub_010D7FA8 | globals: 001e0fd0:& |  |
| 10dfb88 | motrootdyn_startfreefall_from_pendulum | 208 | recompiled | - | charrootdyn_updateCharacterClassParentGeometry, manager_sub_010D7FA8 | - | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10dff10 | charrootdyn_resetRootDynamicsWithLowerCollisionForXZ | 128 | recompiled | - | ClipCollisionFinderCallBack, GetCharacterClass, ParentLayoutObj, manager_sub_010D90C0 | globals: 010dc808:& | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 10dff90 | charrootdyn_resetRootDynamicsWithLowerCollisionForXZFit | 128 | recompiled | - | ClipCollisionFinderCallBack, GetCharacterClass, ParentLayoutObj, manager_sub_010D8DA8 | globals: 010dc808:& | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e0038 | motrootdyn_startcharnodeparent | 352 | recompiled | - | GetGeometryPointerQuatPos | $a1: +0x134:R4 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e0198 | norootdynamics_vel_xzstab | 240 | recompiled | - | GetLayoutClass, StabilizeToYRotQuaternion, norootdynamics | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e0288 | norootdynamics_vel | 224 | recompiled | - | GetLayoutClass, norootdynamics | - | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e1ec8 | getRootFittingEnableFlag | 16 | recompiled | - | - | globals: 0118caa0:& | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e1ed8 | getRootFittingMode | 16 | recompiled | - | - | globals: 0118caa4:& | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e21c0 | CalcRootFitting | 272 | decomp_charageo.cpp | ExecCharaGeometry | CopyMultiQuaternion, GetCharacterClass, manager_sub_010E1808, regularizeMatAry | - |  |
| 10e31f0 | charrootdyn_edgepointstab | 472 | recompiled | manager_sub_010E35B8 | GetCollisionDBPlane, GetCollisionEdgePointerVertexPoint, GetQuaternionByAxisDirZY, SetQuaternionByAxisRotateVWithNoRegularize, manager_sub_010E22F8 | $a0: +0x0:R16/W16; $a1: +0x0:W16; globals: 001e0fb0:&R | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e33c8 | motrootdyn_startedgepoint_from_pendulum_with_edge | 224 | recompiled | - | ExchangeGeometryPointerCollisionEdgeOtherSideInfo, ForceUnparentLayoutObj, GetGeometryPointerSurfacePlane, SetLayoutClassRootQuaternion, charrootdyn_getCompleteFittingYQFromYDir +4 | $a1: +0x134:R4 +0x13c:R4 +0xc2a:R1; globals: 001e0fd0:& | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10e34a8 | motrootdyn_setupedgepoint | 56 | recompiled | charrootdyn_exchangeEdgePointOtherSideSurface | charrootdyn_getLocalOffsetFromParentGP, manager_sub_010E2A48 | - | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10e34e0 | motrootdyn_startedgepoint | 216 | decomp_liftd7_03.cpp | - | ParentLayoutObj, charrootdyn_getLocalOffsetFromParentGP, manager_sub_010E2A48 | $a1: +0x134:R4 +0x13c:R4 +0x19c:R4 +0x610:R4 +0x670:W4 +0xe70:R4 +0xed0:R4 +0xf3c:R4 |  |
| 10e3f60 | GetPleneFittingPointWithNormal | 192 | recompiled | - | GetPointDistanceFromPlane | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16 | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10e4020 | charrootdyn_parentpointxzfitstab | 1392 | recompiled | manager_sub_010E4590, motrootdyn_startparentpointxzfit_from_pendulum, motrootdyn_startxz_from_parentpoint, motrootdyn_startxzfit_from_parentpointxzfit, parentpointxzfit | DivQuaternion, GetGeometryPointerPosition, GetGeometryPointerQuaternion, GetGeometryPointerSurfacePlane, SetLayoutClassRootQuaternion +7 | $a0: +0x134:R4 +0x13c:R4 +0x40c:R4 +0x6d0:R4/W16 +0x6d4:R4/W4 +0x6d8:R4 +0xc30:R4; $a2: +0x0:R16; globals: 001e0ed4:R 0118cab0:&RW 0118cac0:& 0118cac4:W | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e48b8 | parentpointxzfit | 880 | recompiled | GetCharaRootDynamics | ForceUnparentLayoutObj, GetCharacterClass, GetGeometryPointerCollisionDB, GetLayoutClass, charrootdyn_checkZplane +14 | globals: 001e0f90:& 001e0fd0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e4c28 | motrootdyn_startparentpointxzfit_from_pendulum | 128 | recompiled | - | GetGeometryPointerSurfacePlane, charrootdyn_getCompleteFittingYQFromYDir, charrootdyn_parentpointxzfitstab, charrootdyn_updateCharacterClassParentGeometry | $a1: +0x13c:R4 +0x1c0:R4; globals: 001e0fd0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e4ca8 | motrootdyn_startparentpointxzfit | 32 | recompiled | motrootdyn_startpendulum, motrootdyn_startpendulum_with_VCOL | manager_sub_010E4590 | - | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e4cc8 | motrootdyn_startparentpointxzfit_from_falldown | 176 | recompiled | - | ParentLayoutObj, charrootdyn_getLocalOffsetFromParentGP, manager_sub_010E4590, motrootdyn_parent_from_falldown | $a1: +0x124:R4 +0x134:R4 +0x13c:R4 +0xf3c:R4 +0xfbc:R4 | standing / climbing on a moving body (a colossus): the parent point modes |
| 10e4d78 | motrootdyn_startparentpointxzfit_from_xzfit | 48 | recompiled | - | manager_sub_010E4590 | $a1: +0xb0c:R4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e4da8 | motrootdyn_startparentpointxzfit_from_xz | 88 | recompiled | - | charrootdyn_updateCharacterClassParentGeometry, manager_sub_010E4590, motrootdyn_changehcol_from_xz | globals: 001e0fd0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e4fe8 | pendulum | 6560 | recompiled | GetCharaRootDynamics | CheckLegalCollisionGP, ClipCollisionFinder, ClipCollisionFinderWithKeep, CompletePlaneD, CopyGeometryPointer +36 | $a1: +0x0:R16 +0x4:R4; globals: 001e0ed4:R 001e0f90:& 001e1010:& 010dc808:& 0118cad0:&R | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e6988 | motrootdyn_startpendulum_nochange_parent | 40 | recompiled | - | manager_sub_010E4E00 | - | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e69b0 | motrootdyn_startpendulum | 72 | recompiled | - | manager_sub_010E4E00, motrootdyn_startparentpointxzfit | - | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e69f8 | motrootdyn_startchainhang | 96 | recompiled | - | ParentLayoutObj, manager_sub_010E4E00 | $a1: +0xdec:R4 | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10e6a58 | motrootdyn_startpendulum_with_VCOL | 144 | recompiled | - | ParentLayoutObj, manager_sub_010E4E00, motrootdyn_startparentpointxzfit | $a1: +0x6d0:W4 +0xf3c:R4 | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10e6ae8 | swim | 1440 | decomp_liftd8_00.cpp | GetCharaRootDynamics | GetCharaVCollisionStatus, GetCharacterClass, StabilizeToYRotQuaternion, charrootdyn_HslidecollisionCB, charrootdyn_addWindDynamicsToRoot +10 | $a2: +0x0:R16; globals: 001e0f90:& 001e0fd0:& 010e70c0:& |  |
| 10e7088 | motrootdyn_startswim | 56 | decomp_liftd8_00.cpp | - | ForceUnparentLayoutObj | - |  |
| 10e8108 | motrootdyn_startxz_from_pendulum | 224 | recompiled | - | ForceUnparentLayoutObj, ParentLayoutObj, SetLayoutClassRootQuaternion, charrootdyn_getCompleteFittingYQFromYDir, charrootdyn_getLandingSurface +2 | $a1: +0x134:R4 +0x13c:R4 +0x3d8:R4 +0xab0:W16 +0xb54:W4; $a2: +0x4:R4; globals: 001e0f90:&R 001e0fb0:& | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10e81e8 | motrootdyn_startxz_from_edgepoint | 232 | recompiled | - | ForceUnparentLayoutObj, ParentLayoutObj, SetLayoutClassRootQuaternion, charrootdyn_getCompleteFittingYQFromYDir, charrootdyn_getLandingSurface +2 | $a1: +0x134:R4 +0x13c:R4 +0x3d8:R4 +0xab0:W16 +0xb54:W4; $a2: +0x4:R4; globals: 001e0f90:&R 001e0fb0:& 001e0fd0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10e82d0 | motrootdyn_startxz_from_parentpoint | 224 | recompiled | - | ForceUnparentLayoutObj, ParentLayoutObj, SetLayoutClassRootQuaternion, charrootdyn_getCompleteFittingYQFromYDir, charrootdyn_getLandingSurface +2 | $a1: +0x134:R4 +0x13c:R4 +0x1c4:R4 +0x3d8:R4 +0xab0:W16 +0xb54:W4; $a2: +0x4:R4; globals: 001e0f90:&R | standing / climbing on a moving body (a colossus): the parent point modes |
| 10e83b0 | motrootdyn_startxz_from_xzfit | 232 | decomp_liftd9_00.cpp | - | ForceUnparentLayoutObj, ParentLayoutObj, SetLayoutClassRootQuaternion, charrootdyn_getCompleteFittingYQFromYDir, charrootdyn_getLandingSurface +2 | $a1: +0x134:R4 +0x13c:R4 +0x3d8:R4 +0xab0:W16 +0xb54:W4; $a2: +0x4:R4; globals: 001e0f90:&R 001e0fb0:& 001e0fd0:& | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10e8498 | motrootdyn_startxz | 200 | decomp_liftd7_03.cpp | - | ForceUnparentLayoutObj, GetGeometryPointerPosition, ParentLayoutObj | $a1: +0x124:R4 +0x134:R4 +0x6d0:W4 +0x6d8:W4 +0xab0:W16 +0xf3c:R4 +0xfbc:R4; globals: 001e0f90:&R |  |
| 10e8738 | motrootdyn_startxzfit | 1136 | decomp_liftd9_00.cpp | motrootdyn_startxzfit_from_falldown | CopyGeometryPointer, GetGeometryPointerPosition, GetGeometryPointerSurfacePlane, GetPointDistanceFromPlane, ParentLayoutObj +10 | $a1: +0x124:R4 +0x134:R4 +0x13c:R4 +0x1c0:R4 +0x370:R2 +0x40c:R4 +0x6d4:W4 +0x6d8:W4 +0xab0:W16 +0xb40:W16 +0xb50:W4 +0xe34:R4 +0xe38:R4 +0xf3c:R4 +0xfbc:R4; gl | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10e8ba8 | motrootdyn_parent_from_falldown | 744 | decomp_liftd9_00.cpp | motrootdyn_startparentpointxzfit_from_falldown, motrootdyn_startxzfit_from_falldown | GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerQuatPos, GetGeometryPointerSurfacePlane, GetMatrixFromQuatPos, SetLayoutClassRootQuaternion +2 | $a1: +0x124:R4 +0x134:R4 +0x13c:R4 +0xf3c:R4 +0xfbc:R4; globals: 001e0fa0:& | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10e8e90 | motrootdyn_startxzfit_from_xz | 976 | recompiled | - | GetCharacterClass, GetGeometryPointerSurfacePlane, ParentLayoutObj, SetLayoutClassRootQuaternion, UnlinkSecureParentInfo +6 | $a1: +0x124:R4 +0x134:R4 +0x13c:R4 +0x1c0:R4 +0x40c:R4 +0x6d0:R16/W4 +0x6d8:W4 +0xab0:W16 +0xb34:R4/W4 +0xb40:W16 +0xb50:W4 +0xc30:R4; globals: 001e0ed4:R 001e0 | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 10eb438 | motrootdyn_changehcol_from_xz | 304 | recompiled | motrootdyn_startparentpointxzfit_from_xz | GetCharacterClass, GetGeometryPointerSurfacePlane, ParentLayoutObj | - | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10eb568 | motrootdyn_startxzfit_common | 328 | recompiled | - | UnlinkSecureParentInfo | $a1: +0x13c:R4 +0x1c0:R4 +0xab0:W16 +0xb40:W16 +0xb50:W4; globals: 001e0ed4:R 001e0f90:&R | a root dynamics mode change of Wander's (falls, slides, ledges, climbing); motion data calls these through the mode table |
| 10eb6b0 | motrootdyn_startxzfit_from_falldown | 72 | decomp_liftd9_00.cpp | - | motrootdyn_parent_from_falldown, motrootdyn_startxzfit | - | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 10eb6f8 | motrootdyn_startxzfit_from_edgepoint | 112 | recompiled | - | CopyGeometryPointer, charrootdyn_updateCharacterClassParentGeometry, manager_sub_010E9260 | - | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10eb768 | motrootdyn_startxzfit_from_parentpointxzfit | 128 | recompiled | - | CopyGeometryPointer, charrootdyn_parentpointxzfitstab, charrootdyn_updateCharacterClassParentGeometry, manager_sub_010E9260 | $a1: +0x1c4:R4 | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 10eb7e8 | motrootdyn_startxzfit_from_pendulum | 112 | recompiled | - | CopyGeometryPointer, charrootdyn_updateCharacterClassParentGeometry, manager_sub_010E9260 | - | Wander hanging: a ledge, a colossus' fur or a chain (pendulum / edge point modes) |
| 10eb858 | xzfit | 192 | decomp_liftd7_03.cpp | GetCharaRootDynamics | GetAnimationClass, manager_sub_010EA718 | - |  |
| 10ed050 | AddCollMan | 328 | decomp_liftd6_00.cpp | manager_sub_01105938 | isysGroupForExit, isysGroupForFirst, isysGroupForNext, manager_sub_010ECD80 | globals: 01215e80:& 01215f10:RW 01215f18:& 01215f1c:& |  |
| 10ed198 | DeleteCollMan | 232 | decomp_liftd6_00.cpp | SubCallDeleteFuncNonISysObjTypeRes, manager_sub_0110C328 | _isysDeleteGroup, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst, isysGroupForNext | globals: 01215e80:& 01215f10:RW 01215f20:& |  |
| 10ed5e0 | CheckCollisionColClass | 168 | recompiled | - | GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix, GetRateInOutEllipticalSphereVolumeWithInverseMatrix | $a1: +0x40:R4 +0x70:W4 | a collision library call no route's caller makes (callers: see the table) |
| 10ed7b0 | CheckPositionInsideOfJudgeVolumeInfo | 824 | decomp_liftd6_00.cpp | ClipJudgeVolumeInfo, ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject, manager_sub_01165980 | CheckCubeVolume, CheckEllipticalCylinderVolumeShape, CheckEllipticalSphereVolumeShape | $a0: +0x0:R16 +0x50:R4 +0x54:R4 +0x58:R4; $a1: +0x0:R16 |  |
| 10ee418 | GetJudgeVolumeClass | 104 | recompiled | DebugDispVolumeAll, DebugDispVolumeAllColor, updateAttackWork | GetCharacterClass, GetScriptAnimationClass | $a0: +0x8:R1 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 10ee480 | InitJudgeVolumeClass | 304 | decomp_liftd6_00.cpp | InitCharacterObj, manager_sub_01105938 | iosMallocAlign, isysInitGroup | $a0: +0x0:W4 +0x4:R4/W4 +0x8:W4; $a1: +0x0:R4; globals: 001e0ed4:R |  |
| 10ee5b0 | FlushJudgeVolumeClassImmediate | 120 | decomp_liftd6_00.cpp | manager_sub_011031F8 | iosFree, isysDeleteGroupAllObj | $a0: +0x0:R4/W4 +0x4:R4/W4 |  |
| 10ee628 | FlushJudgeVolumeObjImmediate | 208 | decomp_liftd6_00.cpp | FlushCharacterObjImmediate | GetCharacterClass, GetScriptAnimationClass, iosFree, isysDeleteGroupAllObj | $a0: +0x8:R1 |  |
| 10ee6f8 | FlushJudgeVolumeClass | 8 | decomp_liftd6_00.cpp | manager_sub_0110C2C8 | - | - |  |
| 10ee700 | FlushJudgeVolumeObj | 104 | decomp_liftd4_01.cpp | FlushCharacterObj | GetCharacterClass, GetScriptAnimationClass | $a0: +0x8:R1 |  |
| 10ee768 | GetJudgeVolumeClassWorkSize | 96 | decomp_liftd6_01.cpp | CreateScriptAnimationObj, GetCharacterObjWorkSize | - | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10ee7c8 | EntryJudgeVolumeClassObjectGroup | 64 | decomp_liftd6_01.cpp | EntryScriptAnimationObjJudgeVolumeGroup | isysAddGroupWithLinkParam | $a0: +0x4:R4 |  |
| 10ee808 | GetJudgeVolumeClassObjectGroup | 40 | recompiled | DebugDispVolumeAll, DebugDispVolumeAllColor, updateAttackWork | - | $a0: +0x4:R4; $a1: +0x0:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 10ee830 | GetJudgeVolumeClassObjectGroupById | 32 | recompiled | GetVolumeClippedTargetPos, gamecore_sub_01373520 | - | $a0: +0x4:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 10ee850 | CheckPositionInsideOfJudgeVolumeObject | 296 | decomp_liftd7_03.cpp | GAMEUTIL_is_boss_battle_area, SCRConditionVolumeCheckForBoneByGameObj, SCRConditionVolumeCheckForPlayerPos, SCRConditionVolumeCheckForPointPos, SCREventConditionVolumeCheckByGameObj, SCRFuncMinoBCanossaWallBreak +11 | GetCharacterClass, GetScriptAnimationClass, ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject, isysGroupForExit, isysGroupForFirst +1 | $a1: +0x8:R1; $a2: +0x0:R4 |  |
| 10ee978 | CheckPositionInsideOfJudgeVolumeObjectByLabel | 344 | recompiled | gamecore_sub_01345410 | GetCharacterClass, GetScriptAnimationClass, ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject, isysGroupForExit, isysGroupForFirst +1 | $a1: +0x8:R1 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 10eead0 | GetCenterPositionOfJudgeVolumeInfo | 304 | recompiled | - | - | $a0: +0x0:R16/W16 +0x8:W4; $a1: +0x0:R16 +0x48:R4 +0x50:R4 +0x54:R4 +0x58:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 10eec00 | SetJudgeVolumeInfo | 64 | decomp_liftd2_00.cpp | ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject, ScriptAnimationObjOfJudgeVolumeObject | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16 +0x40:W16 +0x50:W4 +0x54:W4 +0x58:W4 +0x5c:W4; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 10eec40 | SearchJudgeVolumeByName | 280 | decomp_liftd2_00.cpp | manager_sub_010BAAF0 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10f1d60 | FlushCollisionWork | 200 | decomp_liftd7_08.cpp | GetCharaRootDynamics, SCRSetupWeaponPlayer, gamecore_sub_0133A048, nico_optBoneParentScaleUpdateScript, nico_sheathScript, nico_swordstart +4 | FlushCollisionWorkByWorkStruct, GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1 |  |
| 11016a8 | AddObjectManagerCollisionWorkSizeStage | 24 | recompiled | - | - | globals: 01216d00:& 01216d10:RW | a collision library call no route's caller makes (callers: see the table) |
| 11021a0 | EntryObjectManagerCollisionGroup | 344 | decomp_liftd6_00.cpp | manager_sub_010EFB50, manager_sub_010EFD08 | GetLayoutClass, isysAddGroupRelativeWithLinkParam, isysAddGroupWithLinkParam, isysGroupForExit, isysGroupForFirst +1 | $a1: +0xc:R2; globals: 0118cb20:R |  |
| 110c790 | manager_sub_0110C790 | 64 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 11186a8 | AddCollisionDB | 632 | recompiled | manager_sub_01141518 | DeleteCollisionFinder, GetScriptCharacterClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext +3 | $a0: +0x0:R4/W4 +0x4:R4 +0x8:R4; globals: 01129400:& | a collision library call no route's caller makes (callers: see the table) |
| 1118920 | GetCollisionDB | 616 | recompiled | manager_sub_01141518, nico_balltest, nico_pendulumtest, updateVuColiDispWork | DeleteCollisionFinder, FlushCollisionFinder, GetScriptCharacterClass, isysGroupForExit, isysGroupForFirst +4 | $a0: +0x0:R4/W4 +0x4:R4 +0x8:R4; globals: 01129400:& | a collision library call no route's caller makes (callers: see the table) |
| 11193c8 | GetCollisionDBSurfBoundary | 416 | decomp_hot44a.cpp | ClipCollisionDB, edgepoint, gamecore_sub_013FBBB0, gamecore_sub_013FC148, gamecore_sub_013FC450, manager_sub_010D48B8 +4 | EntryModifiedManager, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary, manager_sub_01116108 | $a0: +0x0:W16 +0xc:R4/W4; $a1: +0x0:R4 +0x4:R4 |  |
| 111a2d8 | GetCollisionEdgePointerHVPlane | 1120 | decomp_liftd3_01.cpp | manager_sub_010D4C48, manager_sub_010E22F8 | EntryModifiedManager, GetCollisionEdgePointerGlobalVerticalPlane, GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:R16/W16; $a1: +0x0:W16; $a2: +0x0:R16/W16; $a3: +0x0:R16/W16; globals: 001e0fb0:&R |  |
| 111a738 | GetCollisionEdgePointerOtherSideDBData | 512 | decomp_liftd3_01.cpp | DebugDispCollisionFinder, GetCollisionEdgePointerOtherSideCollisionEdge, manager_sub_010D48B8, manager_sub_010D4C48, manager_sub_010E22F8, manager_sub_01120CB8 +1 | GetLayoutClass | $a0: +0x0:W4 +0x4:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1 +0x8:R4 |  |
| 111a938 | GetCollisionDBEdgeInfoNext3 | 1112 | decomp_hot48c.cpp | charrootdyn_detectCharEdge, manager_sub_010D4C48 | GetLayoutClass | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1 |  |
| 111ad90 | GetCollisionDBNext3 | 592 | decomp_liftd3_01.cpp | manager_sub_010D4C48 | GetLayoutClass | $a0: +0x0:W4 +0x4:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1 |  |
| 111afe0 | GetCollisionDBNext3Direct | 584 | decomp_liftd4_01.cpp | gamecore_sub_01395EF0, gamecore_sub_013B6658, gamecore_sub_013B7AE8 | GetLayoutClass | $a0: +0x0:W4 +0x4:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1 |  |
| 111b228 | GetCollisionDBEdgeInfo | 488 | decomp_liftd4_01.cpp | DebugDispCollisionFinder, DebugDispManualEdgeCollisionDB, gamecore_sub_013B8438 | GetLayoutClass | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x0:R4 +0x4:R4 +0x7:R1 |  |
| 11203e0 | RelocateCollisionWork | 568 | decomp_liftd6_01.cpp | InitCollisionWork, manager_sub_01129AC8 | GetSizeOfModifiedManagerWork, RelocateModifiedManager, RelocateTwoWayLinkHeader, SetLayoutClassCollisionWorkAdr | $a0: +0x24:R4/W4 +0x3c:W4 +0x54:W4 +0x6c:W4 +0x84:W4 +0x88:R4; $a1: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 |  |
| 1120618 | InitCollisionWork | 664 | decomp_liftd6_00.cpp | manager_sub_010EFB50, manager_sub_010EFD08 | FlushModifiedManager, InitModifiedManager, InitTwoWayLink, RelocateCollisionWork, iosMallocAlign +4 | $a1: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4; globals: 001e0f90:&R 01129ac8:& |  |
| 11208b0 | GetCollisionWorkSize | 296 | decomp_liftd6_00.cpp | manager_sub_010EFB50, manager_sub_010EFD08 | GetSizeOfModifiedManagerWork | $a0: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 |  |
| 1122a58 | GetRayVolumeUncollisionPoint | 2344 | recompiled | GetRayVolumeUncollisionPointForCamera | ClipCollisionDirectCallBack, CompletePlaneD, GetGeometryPointerCollisionDB, GetQuaternionByAxisDirZY, SetQuaternionByAxisRotateVWithNoRegularize +3 | $a1: +0x0:R16; $a2: +0x0:R16; globals: 001e0000:R 011954d0:&R 011954e0:& | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1123650 | CollisionPlaneJudgeCallbackArrow | 512 | decomp_liftd7_04.cpp | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 |  |
| 1123850 | CollisionPlaneJudgeCallbackCameraMove | 520 | decomp_collision.cpp | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 |  |
| 1123a58 | CollisionPlaneJudgeCallbackCameraVisibility | 520 | decomp_collision.cpp | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 |  |
| 1123c60 | CollisionPlaneJudgeCallbackCameraLeave | 304 | decomp_collision.cpp | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 |  |
| 1123e68 | ConstructorCollisionPoint | 144 | decomp_liftd6_02.cpp | player_collision_check_direct_water_surface_plus | memset | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 |  |
| 1123ef8 | ConstructorCollisionDynamics | 112 | decomp_liftd3_01.cpp | GAMEUTIL_reset_collision_info, get_camera_collision_data, reset_camera_collision_data_except_slide_move_ratio | memset | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 +0x20:W8 +0x28:W8 |  |
| 1123f68 | CopyCollisionPoint | 56 | decomp_liftd3_01.cpp | player_collision_check_direct_water_surface_plus | CopyGeometryPointer | $a1: +0x40:R16 |  |
| 1123fa0 | CopyCollisionDynamics | 48 | decomp_liftd4_01.cpp | CAM_get_warp_position, GAMEUTIL_set_data_by_dynamics_collision_info, check_camera_position_float_by_collision, gamecore_sub_01416FF8 | - | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R16 +0x10:R16 |  |
| 1123fd0 | FilterCopyCollisionDynamics | 120 | decomp_liftd4_01.cpp | GAMEUTIL_set_data_by_dynamics_collision_info, get_camera_collision_data | - | $a0: +0x0:W16; $a1: +0x0:R16 |  |
| 1124048 | ConvertFromCollisionPointToDynamics | 88 | decomp_liftd3_01.cpp | CAM_act_ray_check_for_camera, CAM_doit_objective_target_visibility, CAM_doit_post_update_new_camera_data_target_offset, CAM_get_safety_cam_and_look_at_posi, CAM_get_warp_position, GAMEUTIL_set_data_by_point_collision_info +4 | GetGeometryPointerSurfacePlane | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R4 +0x4:R4 +0x40:R16 |  |
| 11240a0 | ResetCollisionDebug | 16 | recompiled | - | - | globals: 0118ce00:W | debug only (a debug menu / draw mode); no game path |
| 1124368 | ps2__CalcCollisionMultiDepSurfBoundary | 168 | decomp_liftd9_90.cpp | ClipCollisionDirect, ClipCollisionDirectCallBack, ClipCollisionDirectCallBackDoubleSide, ClipCollisionDirectCallBackObjAry, ClipCollisionDirectCallBackObjAryDoubleSide, ClipCollisionDirectCallBackOneObj +1 | manager_sub_01116108 | $a0: +0x0:W16 |  |
| 1124410 | FlushCollisionWorkByWorkStruct | 192 | sotc_worker.cpp | FlushCollisionWork | FlushModifiedManager | $a0: +0x1c:W4 +0x24:R4 |  |
| 11244d0 | ReleaseCollisionWork | 48 | decomp_liftd6_00.cpp | FlushLayoutObj | iosFree | $a0: +0x0:R4/W4 |  |
| 1124500 | FlushCollisionWorkImmediate | 64 | decomp_liftd6_00.cpp | FlushLayoutObjImmediate | DeleteAllTwoWayLink, iosMallocSetExecGarbageCallback | - |  |
| 1124c88 | GetCollisionDBCallBack | 640 | decomp_hot47j.cpp | camera_collision_check_with_keep, charnodeparent, charrootdyn_getCollisionDB, charrootdyn_getCollisionDBwithFunc, check_camera_position_float_by_collision, create_camera_collisionDB +2 | DeleteCollisionFinder, FlushCollisionFinder, GetScriptCharacterClass, isysGroupForExit, isysGroupForFirst +3 | $a0: +0x0:R4/W4 +0x4:R4 +0x8:R4 |  |
| 1124f08 | AddCollisionDBCallBack | 624 | recompiled | - | DeleteCollisionFinder, GetScriptCharacterClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext +2 | $a0: +0x0:R4/W4 +0x4:R4 +0x8:R4 | a collision library call no route's caller makes (callers: see the table) |
| 1125178 | GetCollisionDBCurrentPointOnLastPlane | 24 | decomp_liftd4_01.cpp | GAMEUTIL_set_data_by_dynamics_collision_info, charrootdyn_setLandingStatus, manager_sub_010D3950 | manager_sub_01118B88 | - |  |
| 1125280 | GetCollisionDBRegion | 64 | decomp_liftd3_01.cpp | gamecore_sub_01420EE8, gamecore_sub_01421A68, gamecore_sub_01422E90 | manager_sub_01129258 | $a1: +0x0:W4 |  |
| 11252c0 | GetCollisionDBPositionWithST | 416 | decomp_liftd9_90.cpp | GetGeometryPointerPosition, GetGeometryPointerQuatPos | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:R16/W16; $a1: +0x0:R4 +0x4:R4 |  |
| 1125460 | GetCollisionDBPositionWithNodeLocalPos | 120 | decomp_liftd9_90.cpp | GetGeometryPointerPosition, GetGeometryPointerQuatPos | GetLayoutClass | $a0: +0x0:W16; $a1: +0x0:R4; $a2: +0x0:R16 |  |
| 1125680 | GetCollisionDBVectorPlane | 288 | recompiled | - | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W16; $a1: +0x0:W16; $a2: +0x0:R4 +0x4:R4 | a collision library call no route's caller makes (callers: see the table) |
| 11257a0 | GetCollisionDBPlane | 616 | decomp_liftd7_08.cpp | AI_SetQuaternionFromGP, GetGeometryPointerSurfacePlane, GetGeometryPointerSurfaceSlopeDegree, GetGeometryPointerSurfaceYTopZMinusPlaneCoordQuaternion, GetGeometryPointerSurfaceZTopYPlaneCoordQuaternion, GetGeometryPointerVirtualSurfacePlane +32 | EntryModifiedManager, GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:R4 +0x4:R4 |  |
| 1125a08 | GetCollisionDBAttribute | 296 | decomp_liftd9_90.cpp | GetGeometryPointerSurfaceAttribute | GetLayoutClass | $a0: +0x0:W16; $a1: +0x0:R4 +0x4:R4 |  |
| 1125b30 | GetCollisionDBAttributeAdr | 272 | decomp_liftd7_08.cpp | AI_JudgeColAttrFunc, CheckCollisionInsideOfSurfaceXZ, DebugGetParentCharColAttr, SCRConditionSendDamageToCollisionObject, SCRFuncDamageSendFunc, arrowFilter +60 | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 |  |
| 1126190 | NippingCheckCollisionFindetWithKeep4D | 320 | recompiled | - | manager_sub_011298A8 | $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0xc:R4; globals: 0118ce00:&RW | a collision library call no route's caller makes (callers: see the table) |
| 1126c10 | SlideCollisionFinder4D | 544 | recompiled | charrootdyn_slidecollisionCB, gamecore_sub_0133A048, nico_balltest, nico_pendulumtest | ClipCollisionFinderWithKeep4DSlopeLimit, GetLayoutClass, manager_sub_0111F688 | $a1: +0x0:R16; $a2: +0x0:R16 | a collision library call no route's caller makes (callers: see the table) |
| 1126e30 | SlideCollisionFinder4DCB | 600 | decomp_liftd4_01.cpp | charrootdyn_slidecollisionCB, gamecore_sub_01463D10, pendulum | ClipCollisionFinderWithKeep4DSlopeLimit, ClipCollisionFinderWithKeep4DSlopeLimitCB, GetLayoutClass, manager_sub_0111F688 | $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1127d80 | GetCollisionEdgePointerOtherSideCollisionEdge | 232 | decomp_liftd4_01.cpp | ExchangeGeometryPointerCollisionEdgeOtherSideInfo, gamecore_sub_013B8438, manager_sub_010D5308 | GetCollisionEdgePointerOtherSideDBData | $a0: +0x0:W4 +0x4:W4 +0x8:W4 |  |
| 11280c0 | CopyCollisionEdgePointer | 32 | recompiled | - | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 | a collision library call no route's caller makes (callers: see the table) |
| 11280e0 | CopyCollisionEdge | 40 | decomp_liftd7_02.cpp | manager_sub_010D48B8 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 |  |
| 1128108 | GetCollisionEdgePositionWithS | 360 | decomp_liftd9_90.cpp | GetGeometryPointerPosition, GetGeometryPointerQuatPos | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:W16; $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 1128270 | MakeCollisionEdgePointer | 24 | decomp_liftd8_00.cpp | gamecore_sub_01395EF0, gamecore_sub_013B6658 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4; $a1: +0x0:R4 +0x4:R4 |  |
| 1128288 | GetCollisionVariableXZSurfPosition | 104 | decomp_liftd9_90.cpp | GetGeometryPointerPosition | GetLayoutClass | $a0: +0x0:W16; $a1: +0x0:R4 |  |
| 11282f0 | IsClusterCollision | 16 | decomp_liftd6_00.cpp | loadCdColResFile, unloadCdColResFile | - | $a0: +0x4:R4 |  |
| 1128300 | MakeGeometryPointerCollisionInfo | 208 | recompiled | GetGeometryPointerWithCollisionAttributeJudgeFunc, SCRConditionSendDamageToCollisionObject | GetLayoutClass | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x30:W4; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x7:R1 | a collision library call no route's caller makes (callers: see the table) |
| 11283d0 | MakeGeometryPointerCollisionEdgeInfo | 48 | decomp_liftd7_04.cpp | ExchangeGeometryPointerCollisionEdgeOtherSideInfo, gamecore_sub_013B8438 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x30:W4; $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 1128400 | GetGeometryPointerCollisionSurfSTPosition | 104 | recompiled | - | CheckLegalCollisionGP | $a0: +0x0:W4; $a2: +0x8:R4 +0xc:R4 | a collision library call no route's caller makes (callers: see the table) |
| 11285b8 | GetCollisionDBGroupCallBack | 424 | decomp_liftd8_00.cpp | gamecore_sub_013FC148, gamecore_sub_013FC7E0 | GetLayoutClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext | - |  |
| 1128760 | GetCollisionDBLastPointOnCurrentPlane | 24 | decomp_liftd8_00.cpp | charrootdyn_debugDispParentGeometryStatus, gamecore_sub_013B8438, manager_sub_010D8340 | manager_sub_01118B88 | - |  |
| 1128778 | ChangeCollisionResourceAttribute | 88 | recompiled | ChangeLayoutObjCollisionResourceAttribute, SCRFuncChangedColDamageOff, SCRFuncChangedColDamageOn, gamecore_sub_013A5D70 | - | $a0: +0x30:R4 +0x34:R4 | a collision library call no route's caller makes (callers: see the table) |
| 11287d0 | GetCollisionResource | 16 | recompiled | SCRFuncChangedColDamageOff, SCRFuncChangedColDamageOn | - | $a0: +0x8:R4 | a collision library call no route's caller makes (callers: see the table) |
| 1128818 | CollisionObjectJudgeCallbackExceptOneObj | 16 | decomp_liftd7_04.cpp | - | - | - |  |
| 1128828 | CollisionObjectJudgeCallbackExceptDescendOneObj | 112 | recompiled | - | GetLayoutClass, GetLayoutParentObj | - | a collision library call no route's caller makes (callers: see the table) |
| 1128898 | ConstructorCollisionObjectJudgeCallbackData | 56 | decomp_liftd2_00.cpp | camera_collision_check_direct, camera_collision_check_with_keep, camera_invisible_collision_check_direct_by_exception_obj, check_camera_position_float_by_collision, create_camera_collisionDB, get_camera_position_float_by_hit_collision +2 | - | $a0: +0x0:W4 |  |
| 11288d0 | CollisionObjectJudgeCallbackExceptPluralObj | 72 | decomp_liftd7_09.cpp | gamecore_sub_0140FA58 | - | $a1: +0x0:R4 |  |
| 1128918 | CollisionObjectJudgeCallbackExceptDescendPluralObj | 216 | decomp_collision.cpp | - | GetLayoutClass, GetLayoutParentObj | $a1: +0x0:R4 |  |
| 1128a88 | CheckCollisionInsideOfObjectBoundary | 160 | decomp_liftd3_01.cpp | chkEstimatCollisionCore, gamecore_sub_01391E30, gamecore_sub_013CA508 | GetLayoutClass | - |  |
| 1128cb0 | getVolumeCollisionDebugMode | 16 | recompiled | - | - | globals: 0118ce04:& | debug only (a debug menu / draw mode); no game path |
| 1128cc0 | getCollisionDebugGraphMode | 16 | recompiled | - | - | globals: 0118ce08:& | debug only (a debug menu / draw mode); no game path |
| 1129108 | iasmCalcCollisionMultiDepSurfBoundarySub | 104 | recompiled | - | - | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16 | a collision library call no route's caller makes (callers: see the table) |
| 1129170 | CalcCollisionMultiDepSurfBoundary | 216 | recompiled | - | manager_sub_01116108 | $a0: +0x0:W16; $a1: +0x184:R4 | a collision library call no route's caller makes (callers: see the table) |
| 1129410 | GetCollisionDBNextSurfLinkComp | 120 | recompiled | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 | a collision library call no route's caller makes (callers: see the table) |
| 1129488 | GetCollisionDBNextSurfLink | 200 | recompiled | - | GetLayoutClass | $a0: +0x0:R4 +0x4:R4 | a collision library call no route's caller makes (callers: see the table) |
| 1129550 | MakeCollisionPointCollisionInfo | 216 | recompiled | - | GetLayoutClass | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x30:W4 +0x40:W16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x7:R1 | a collision library call no route's caller makes (callers: see the table) |
| 1129630 | GetSTPositionOfCollisionDB | 616 | recompiled | - | GetLayoutClass, manager_sub_01114C70, manager_sub_01116108 | $a0: +0x0:R4/W4; $a1: +0x0:W4; $a2: +0x0:R4 +0x4:R4; $a3: +0x0:R16 | a collision library call no route's caller makes (callers: see the table) |
| 1129a38 | GetCollisionDynamicsProjectionFloatPosition | 144 | recompiled | - | GetPlaneFittingPointForUnnormalizedPlane | $a0: +0x0:W16 | a collision library call no route's caller makes (callers: see the table) |
| 1129b00 | FlushCollisionEdgeLinkDBVWallFinder | 8 | decomp_liftd4_02.cpp | manager_sub_010E9A50, manager_sub_010EA718, xz | - | $a0: +0x20:W4 |  |
| 1129b08 | GetCollisionEdgeLinkDBWorkSize | 32 | decomp_liftd4_01.cpp | GetCharacterObjWorkSize | - | - |  |
| 1129b28 | InitCollisionEdgeLinkDB | 160 | decomp_liftd6_00.cpp | InitCharacterObj | iosMallocAlign | globals: 001e0fb0:&R |  |
| 1129bc8 | ReleaseCollisionEdgeLinkDB | 72 | decomp_liftd7_02.cpp | FlushCharacterObj | iosFree | $a0: +0x0:R4/W4 |  |
| 1129c10 | GetCollisionEdgeLinkDBVWallFinder | 320 | decomp_liftd7_03.cpp | manager_sub_010E9A50, manager_sub_010EA718, xz | GetCollisionEdgePointerVertexPoint, GetCollisionEdgePointerVertexPoint3 | $a0: +0x1c:R4 +0x20:W4 +0x24:R4; $a1: +0x0:R4 +0x4:R4 | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 1129d50 | ReconstructCollisionEdgeLinkDBVWallFinder | 344 | decomp_liftd4_02.cpp | gamecore_sub_01391E30, gamecore_sub_01431738 | GetCollisionEdgePointerVertexPoint, GetCollisionEdgePointerVertexPoint3 | $a0: +0x1c:R4 +0x20:R4/W4 +0x24:R4; $a1: +0x0:R4 +0x4:R4 |  |
| 1129ea8 | SetCollisionEdgeLinkDBFinderVWallDirectionStatus | 16 | recompiled | manager_sub_010E9A50, manager_sub_010EA718 | - | $a0: +0x0:W16; $a1: +0x0:R16 | reached, not trace-checked; falls and teleport drops (lake, Valus' canyon) |
| 1129eb8 | SetCollisionEdgeLinkDBFinderCollectStatus | 32 | decomp_liftd7_04.cpp | nico_blankHorseStart, nico_horseRStart, nico_playerstart | - | $a0: +0x0:W16 +0x12:W1 +0x14:W4 +0x18:W4 +0x1c:W4; $a3: +0x0:R16 |  |
| 1129ed8 | InitializeCollisionFinder | 208 | recompiled | gamecore_sub_0133AFA0 | DeleteTwoWayLink, iosJumpRecoverPoint | $a0: +0x0:W4 +0x4:R4/W4 +0x8:W4 +0xc:R4/W4 +0x10:R4/W4 | a collision library call no route's caller makes (callers: see the table) |
| 1129fa8 | FlushCollisionFinder | 144 | decomp_hot47j.cpp | FlushCharacterObjImmediate, GetCollisionDB, GetCollisionDBCallBack, SetCharacterObjCollisionProcFlag, gamecore_sub_0133AFA0 | DeleteTwoWayLink | $a0: +0x0:R4/W4 +0x4:R4 +0xc:R4 +0x10:R4 |  |
| 112a038 | InitCollisionFinder | 272 | decomp_liftd7_04.cpp | gamecore_sub_014269A8, initVuColiDispWork, manager_sub_01141690, nico_balltest, nico_pendulumtest | DeleteTwoWayLink, iosJumpRecoverPoint, iosMallocAlign | $a0: +0x0:W4 +0x4:R4/W4 +0x8:W4 +0xc:R4/W4 +0x10:R4/W4 |  |
| 112a148 | InitCollisionFinderWithRelyNumber | 264 | decomp_liftd6_00.cpp | InitCharacterObj | DeleteTwoWayLink, iosJumpRecoverPoint, iosMallocAlign | $a0: +0x0:W4 +0x4:R4/W4 +0x8:W4 +0xc:R4/W4 +0x10:R4/W4 |  |
| 112a250 | EntryCollisionFinder | 160 | decomp_hot47j.cpp | manager_sub_011154E8, manager_sub_01115798, manager_sub_01115A50, manager_sub_01116958 | AddTwoWayLinkNext, GetLayoutClass, InitTwoWayLink | $a0: +0x0:R4/W4 +0x4:R4 +0xc:R4 +0x10:R4 |  |
| 112a2f0 | DeleteCollisionFinder | 64 | decomp_hot47j.cpp | AddCollisionDB, AddCollisionDBCallBack, GetCollisionDB, GetCollisionDBCallBack | DeleteTwoWayLink | $a0: +0xc:R4 +0x10:R4 |  |
| 112a330 | GetCollisionFinderWorkSize | 32 | decomp_liftd4_01.cpp | GetCalcNumObjWorkSize, GetCharacterObjWorkSize | - | - |  |
| 112a350 | CopyCollisionFinder | 48 | decomp_liftd5_50.cpp | manager_sub_01140CE0 | - | $a0: +0x0:W4 +0x7:W4 +0x8:W4 +0xf:W4 +0x10:W4; $a1: +0x0:R4 +0x7:R4 +0x8:R4 +0xf:R4 +0x10:R4 |  |
| 112a380 | ReleaseCollisionFinder | 208 | decomp_liftd5_50.cpp | FlushCharacterObj, gamecore_sub_01425B00, manager_sub_0113A7B8, manager_sub_0113A918, manager_sub_0113AAE0 | DeleteTwoWayLink, iosFree | $a0: +0x0:R4/W4 +0x4:R4 +0xc:R4/W4 +0x10:R4/W4 |  |
| 112a478 | CheckCollisionFinderExistData | 104 | decomp_collision.cpp | manager_sub_011154E8, manager_sub_01115798, manager_sub_01115A50 | - | $a0: +0x0:R4 +0x4:R4 +0xc:R4 |  |
| 112a4e0 | DebugSetCollisionWideAreaSearchGroup | 16 | recompiled | - | - | globals: 0118ce90:W | debug only (a debug menu / draw mode); no game path |
| 112a4f0 | InitCollisionWideAreaSearchInfo | 88 | recompiled | - | - | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8; globals: 0118cea0:&R 0118cea8:R 0118ceb0:R 0118ceb8:R 0118cec0:&R 0118cec8:R 0118cf80:& | the collision wide-area search (requested by a script command; no route's scripts use it) |
| 112a548 | GetCollisionWideAreaSearch | 144 | recompiled | - | ClipCollisionDirect, GetGeometryPointerPosition | $a0: +0x0:R4/W4; globals: 0118ce90:R | the collision wide-area search (requested by a script command; no route's scripts use it) |
| 112a5d8 | RequestCollisionWideAreaSearch | 80 | recompiled | - | CopyGeometryPointer | $a0: +0x0:W4 +0x4:W4 | the collision wide-area search (requested by a script command; no route's scripts use it) |
| 112a628 | CancelRequestCollisionWideAreaSearch | 8 | recompiled | - | - | - | the collision wide-area search (requested by a script command; no route's scripts use it) |
| 112c780 | CheckCollisionInsideOfSurfaceXZ | 688 | recompiled | SCRFuncNargaWayVolumeMain, gamecore_sub_01405DB8, updateScriptDataSystem | GetCollisionDBAttributeAdr, GetCollisionDBVertexPoint, GetLayoutClass, getLayoutClassCollisionIndepSurfBoundary | $a2: +0x0:R16 | a collision library call no route's caller makes (callers: see the table) |
| 1132cc8 | GetGeometryPointerCollisionDB | 56 | decomp_hot47j.cpp | Check_InVariableXZSurfJudgeFunc_LowerSurface, GetCollisionCameraAttributeShareArray, GetCollisionCameraAttributeShareArrayNumber, GetRayVolumeUncollisionPoint, edgepoint, edgexz +21 | - | $a0: +0x30:R4 |  |
| 1132d00 | GetGeometryPointerWithCollisionAttributeJudgeFunc | 88 | recompiled | - | GetLayoutObjCollisionDBWithTargetAttributeJudgeFunc, MakeGeometryPointerCollisionInfo | globals: 001e0fd0:& | a collision library call no route's caller makes (callers: see the table) |
| 1132d58 | CheckLegalCollisionGP | 40 | decomp_liftd2_00.cpp | DebugGetParentCharColAttr, GetCharaRootDynamics, GetCollisionCameraAttributeShareArrayNumber, GetControlCheckStatusFromGP, GetGeometryPointerCollisionSurfSTPosition, kickLWS +3 | - | $a0: +0x30:R4 |  |
| 1132d80 | ExchangeGeometryPointerCollisionEdgeOtherSideInfo | 160 | recompiled | charrootdyn_exchangeEdgePointOtherSideSurface, motrootdyn_startedgepoint_from_pendulum_with_edge, pendulum, player_setDangerEdgeOtherSidePlanePoint | GetCollisionEdgePointerOtherSideCollisionEdge, MakeGeometryPointerCollisionEdgeInfo | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 +0x20:W8 +0x28:W8 +0x30:W8 +0x38:W8 | a collision library call no route's caller makes (callers: see the table) |
| 1134390 | GetPlaneFittingDiffVector | 72 | decomp_liftd2_01.cpp | charrootdyn_getCompleteFittingPlaneFromStabGPWithSlopeLimit | - | $a0: +0x0:W16 +0xc:W4; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 11343d8 | GetPlaneFittingPoint | 104 | decomp_liftd2_01.cpp | GetCollisionEdgePointerGlobalVerticalPlane, charrootdyn_HslidecollisionCB, charrootdyn_getCompleteFittingYQFromStabGPWithSlopeLimit_X, edgepoint, getAverageNormal, manager_sub_010D48B8 +5 | - | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1134440 | GetPlaneFittingPointForUnnormalizedPlane | 128 | decomp_liftd2_01.cpp | GetCollisionDynamicsProjectionFloatPosition, GetCollisionEdgePointerGlobalVerticalPlane, charrootdyn_HslidecollisionCB, manager_sub_010D5308, manager_sub_0111F688 | - | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1134628 | GetPlaneFittingVector | 88 | decomp_liftd2_01.cpp | GetCharaRootDynamics, GetCharaVCollisionStatus, GetCollisionDBPlaneNormalZQuaternion, SetCharacterClassWindDynamics, charrootdyn_addSlideElement, charrootdyn_addWindDynamicsToRoot +5 | - | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1136240 | CheckCubeVolumeWithInverseMatrix | 144 | recompiled | - | iosAbsVector | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 11362d0 | CheckCubeVolume | 176 | decomp_liftd3_01.cpp | CheckPositionInsideOfJudgeVolumeInfo, manager_sub_01165DD0 | iosAbsVector, iosInverseMatrix | $a1: +0x0:R16 |  |
| 1136380 | CheckEllipticalSphereVolumeWithInverseMatrix | 208 | recompiled | - | - | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136450 | CheckEllipticalSphereVolume | 256 | recompiled | - | iosInverseMatrix | $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136be0 | CheckCylinderVolumeWithInverseMatrix | 136 | recompiled | - | - | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R4 +0x4:R4 +0x8:R4 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136c68 | CheckCylinderVolume | 192 | recompiled | manager_sub_011654A8 | iosInverseMatrix | $a1: +0x0:R16 | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136d28 | GetRateInOutEllipticalSphereVolume | 96 | recompiled | GAMEUTIL_is_inside_of_region_ratio, manager_sub_01161130 | GetRateInOutEllipticalSphereVolumeWithInverseMatrix, iosInverseMatrix | - | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136d88 | GetRateInOutEllipticalSphereVolumeWithInverseMatrix | 328 | decomp_liftd2_01.cpp | CheckCollisionColClass, ExecFogColMan, ExecGlobalLightMan, GetRateInOutEllipticalSphereVolume | - | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16/R4 +0x4:R4 +0x8:R4 |  |
| 1136ed0 | GetRateInOutCubeVolume | 96 | recompiled | GAMEUTIL_is_inside_of_region_ratio, manager_sub_01161130 | GetRateInOutCubeVolumeWithInverseMatrix, iosInverseMatrix | - | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1136f30 | GetRateInOutCubeVolumeWithInverseMatrix | 440 | decomp_liftd8_00.cpp | CheckCollisionColClass, ExecFogColMan, ExecGlobalLightMan, GetRateInOutCubeVolume, manager_sub_01146FB8 | iosAbsVector | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16; $a1: +0x0:R16; $a2: +0x0:R16/R4 +0x4:R4 +0x8:R4; $a3: +0x0:R16/R4 +0x4:R4 +0x8:R4 |  |
| 11461c0 | dynamic_cast_to_wind_emitter_sphere | 32 | recompiled | - | - | $a0: +0x0:R4 | the wind emitters' judge-volume kind (no area on these routes uses it) |
| 11461e0 | dynamic_cast_to_wind_emitter_box | 40 | recompiled | - | - | $a0: +0x0:R4 | the wind emitters' judge-volume kind (no area on these routes uses it) |
| 1146208 | dynamic_cast_to_wind_emitter_judge_volume | 40 | recompiled | - | - | $a0: +0x0:R4 | the wind emitters' judge-volume kind (no area on these routes uses it) |
| 1146818 | boot_wind_emitter_by_judge_volume | 360 | recompiled | SequenceKickWindVolumeShape | CopyScriptAnimationWindGeometryPointer, CopyScriptAnimationWindParam, malloc_from_fast_get_partition, manager_sub_011472A8 | globals: 001e0fc0:&R 001e0fd0:&R 0118ccf0:& 0118cd40:& 01195950:& 01216ef0:&R 01216ef4:&R | the wind emitters' judge-volume kind (no area on these routes uses it) |
| 1343290 | GAMEUTIL_is_swimming_now | 112 | recompiled | gamecore_sub_013B0600, player_IsSwiming | GAMEUTIL_is_player_obj, GetCharacterClass | globals: 001b7b60:R | a game utility no route's caller asks |
| 1344270 | GAMEUTIL_reset_collision_info | 80 | decomp_liftd7_09.cpp | CAM_constructor_target_visibility, CAM_doit_objective_target_visibility, CAM_doit_subjective_target_visibility_and_update_cam_data | ConstructorCollisionDynamics | $a0: +0x0:W4 +0x40:W16 +0x50:W16; globals: 001e0f90:&R 001e0fd0:&R |  |
| 13442c0 | GAMEUTIL_set_data_by_point_collision_info | 56 | recompiled | - | ConvertFromCollisionPointToDynamics, GAMEUTIL_set_data_by_dynamics_collision_info | - | a game utility no route's caller asks |
| 13442f8 | GAMEUTIL_set_data_by_dynamics_collision_info | 184 | decomp_liftd7_09.cpp | CAM_doit_objective_target_visibility, CAM_doit_subjective_target_visibility_and_update_cam_data, GAMEUTIL_set_data_by_point_collision_info, doit_camera_target_lost_angle | CopyCollisionDynamics, FilterCopyCollisionDynamics, GAMEUTIL_is_nearly_equal_zero3D, GetCollisionDBCurrentPointOnLastPlane | $a0: +0x0:W4 |  |
| 134de48 | fadeWhiteOut | 264 | recompiled | gamecore_sub_01389A88 | PauseGame, scrRun, scriptIntrSignal, setFadeColor, setFadeValue | globals: 001b7b60:R 01465b5c:W 01465b60:&RW 01465b64:W | a colossus' death (the white fade-out / fade-in scripts) |
| 134dff0 | scrFadeWhiteIn | 112 | recompiled | - | scriptIntrSignal, setFadeColor, setFadeValue | globals: 001b7b60:R 01465b5c:W 01465b64:W | a colossus' death (the white fade-out / fade-in scripts) |
| 134e0d8 | scrFadeWhiteOut | 120 | recompiled | - | scriptIntrSignal, setFadeColor, setFadeValue | globals: 001b7b60:R 01465b5c:W 01465b64:W | a colossus' death (the white fade-out / fade-in scripts) |
| 1356588 | st_collisionalias | 120 | decomp_liftd7_06.cpp | - | FlushCollisionWork, GetLayoutClass, UpdateAliasLayoutObjClothGeometry, getClothGeometry, isysGetThisSubThread +1 | globals: 01465650:& 014657d0:R 014657d4:R |  |
| 1374f08 | character_ikcolattrfilter | 248 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 137a140 | SCRConditionSendDamageToCollisionObject2 | 2072 | recompiled | - | ClipCollisionDirectCallBack, GetGeometryPointerObject, GetLayoutClass, SendCharaAttackMsg, getSCRDataCtrlWork +2 | globals: 001e0fd0:&R 0137b738:& 01465888:& | a fight: a weapon or a colossus' attack hitting a collision object |
| 137acf8 | SCRConditionSendDamageToCollisionObject3 | 1224 | recompiled | - | ClipCollisionDirectCallBack, GetLayoutClass, gamecore_sub_0137A958, getSCRDataCtrlWork | globals: 001e0fd0:&R 0137b738:& 01465888:& | a fight: a weapon or a colossus' attack hitting a collision object |
| 137b578 | SCRConditionSendDamageToCollisionObject | 448 | recompiled | - | GetCharacterClass, GetCollisionDBAttributeAdr, GetLayoutClass, MakeGeometryPointerCollisionInfo, SendCharaAttackMsg +1 | - | a fight: a weapon or a colossus' attack hitting a collision object |
| 137d110 | SCRGetCollision | 144 | decomp_liftd7_08.cpp | SCRFuncKameRockLWSLoad, gamecore_sub_013B0168, tryToCheckCollision6Dir | ClipCollisionDirectCallBack, CopyGeometryPointer | $a1: +0x0:R16; $a2: +0x0:R16 | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 137d1a0 | SCRGetCollisionCallBack | 152 | decomp_liftd7_08.cpp | gamecore_sub_0136B710, gamecore_sub_013A6158, gamecore_sub_013CAD58, gamecore_sub_013CAEE0, perceptionExec, perceptionExec2 | ClipCollisionDirectCallBack, CopyGeometryPointer | $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 137e868 | NICO_is_quake_attack_hit | 48 | recompiled | - | GAMEUTIL_get_parent_collision_attribute | - | a fight: a weapon or a colossus' attack hitting a collision object |
| 1385ba8 | nico_pendulumtest | 1816 | recompiled | - | GetCollisionDB, GetGeometryPointerSurfacePlane, GetLayoutClass, GetPointDistanceFromPlane, GetQuaternionByAxisDirZY +20 | globals: 001e0fb0:& 01465650:& 014657d0:R 014657d4:R 01465880:& 014664f8:&R 014664fc:R 01466500:R | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 13862c0 | getDynamicsTestType | 16 | recompiled | - | - | globals: 014664f4:& | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 13862d0 | getPendulumGravityRatio | 16 | recompiled | - | - | globals: 014664f8:& | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 13862e0 | getPendulumAirResistRatio | 16 | recompiled | - | - | globals: 014664fc:& | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 13862f0 | getPendulumSideFriction | 16 | recompiled | - | - | globals: 01466500:& | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 1386300 | dynamicstest | 8 | recompiled | - | - | - | the dynamics test object (a debug test of the pendulum physics: the getters are its tuning table; pendulum_test is also called from pendulum) |
| 1395830 | gamecore_sub_01395830 | 136 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 13958b8 | gamecore_sub_013958B8 | 168 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 139cc10 | SCRFuncSetFallRockTimerAndSetGroundDestFlag | 112 | recompiled | - | GetSCRCharaAIWorkPtr, SCRGetFirstObj | - | a colossus arena's falling-rock script command (that fight under way) |
| 13a86c0 | SCRFuncNargaWayVolumeMain | 200 | recompiled | - | CheckCollisionInsideOfSurfaceXZ, GetLayoutClass, GetSCRCharaAIWorkPtr | globals: 001e0ed4:R | that colossus' own fight script (its arena, the fight under way) |
| 13aa498 | get_predict_hit_position_nico_sight_on | 16 | decomp_liftd7_08.cpp | AI_CheckSightOn, gamecore_sub_013B2E90 | - | $a0: +0x0:W16; $a1: +0x1d0:R16 +0x1e0:R4 |  |
| 13af4a0 | physicalOp | 1328 | recompiled | - | GetLayoutClass, GetScriptLayoutClass, SCRGetFirstObj, SCRGetXZAngle, SCRPlayerGetWeapon +13 | globals: 001b7b60:R 001e0fb0:& 001e0fc0:&R 01465650:& 014657c8:R 014657d0:R 01465900:& | the AI's physical-operation step (a creature's AI mode no route's AI entered) |
| 13c9b30 | tryToCheckCollision6Dir | 1800 | recompiled | - | GetGeometryPointerPosition, SCRGetCollision, charrootdyn_detectCharClimbableWall, nico_GetGeometryPointerAttr | $a1: +0x0:R16; $a2: +0x0:W16 +0x4:R4/W4; $a3: +0xe34:R4; globals: 001b7b60:R 0146aa90:R | the AI's six-direction collision probe (a creature's AI mode no route's AI entered) |
| 13cb8e8 | gamecore_sub_013CB8E8 | 192 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 13cd218 | pendulum_test | 296 | recompiled | - | GetCharacterClass, GetDifferencialQuaternionWithNoRegularize | $a0: +0x0:R4 +0x4:R4 +0x14:R4; globals: 001b7b60:R 001e0fb0:& | reached, not trace-checked; only dv1's lake run (Wander climbed a pillar from the water by an edge hang at frame 13 092; later parent point and pendulum modes): not reproduced by dv2-dv4 |
| 13d4ab8 | AI_GetHitPosPerpendicular | 400 | decomp_liftd2_01.cpp | gamecore_sub_013DDF00, gamecore_sub_013E1E48, gamecore_sub_013EA180 | - | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 001e0f90:& |  |
| 13d54f0 | AI_JudgeNotCharaObjFunc | 32 | decomp_collision.cpp | - | GetCharacterClass | - |  |
| 13d5510 | AI_JudgeColAttrFunc | 240 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 13e4798 | KiteGroundInitFunc | 384 | decomp_liftd7_08.cpp | - | AI_GetGeometryPointerOfLayout, SetQuaternionByAxisRotateVWithNoRegularize, iosGetRandom, memset | $a0: +0x0:W16; globals: 001b7b70:R 001e0fb0:& 001e0fd0:&R |  |
| 13f5d98 | SCRFuncSnakeCUpGroundInit | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | that colossus' own fight script (its arena, the fight under way) |
| 13f5dc0 | SCRFuncSnakeCUpGroundMain | 208 | recompiled | - | CharaCtrlSetScriptCtrlStatus, GetSCRCharaAIWorkPtr | globals: 001e0ed4:R | that colossus' own fight script (its arena, the fight under way) |
| 13ff148 | GetSwordFlareHitPlaneAttribute | 16 | recompiled | SCRConditionSwordFlareHitColAttr | - | globals: 01466c78:R | a fight: a weapon or a colossus' attack hitting a collision object |
| 13ff388 | getCollisionPosOffset | 16 | recompiled | - | - | globals: 01466d10:& | the weapon table getter for a collision offset (a weapon use no route made) |
| 1404a10 | SCRConditionTortoiseGeyserHitColAttr | 72 | recompiled | - | GetSCRCharaAIWorkPtr | - | that colossus' own fight script (its arena, the fight under way) |
| 140d0b0 | SCRFuncYamoriBSetBreathVolume | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | that colossus' own fight script (its arena, the fight under way) |
| 140d640 | SCRFuncYamoriBSetAttainPosForGroundAttack | 176 | recompiled | - | getSCRDataCtrlWork, setAttainmentPosAndDirAndLock, setPosGazeCtrlWork | globals: 0146c210:&R 0146c218:R | that colossus' own fight script (its arena, the fight under way) |
| 142fc88 | widePathCollisionDirect | 976 | decomp_liftd6_02.cpp | gamecore_sub_01405DB8, gamecore_sub_014307E8, gamecore_sub_01434A40, gamecore_sub_01434B10, gamecore_sub_0144B048 | ClipCollisionDirectCallBackDoubleSide, gamecore_sub_01434CF8, getColiGroupByUserPathWork, getPathColiGroupByType | $a0: +0x3c50:R4 +0x3c54:R4; $a1: +0x0:R16; $a2: +0x0:R16; globals: 001e0fb0:&R 014336a0:& |  |
| 1433128 | pathDetectCollision | 272 | decomp_liftd7_10.cpp | updateUserPathWork | ClipCollisionDirectCallBackDoubleSide, gamecore_sub_01434CF8 | $a1: +0x0:W16; $a2: +0x0:R16; globals: 014336a0:& | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 1433238 | pathDetectCollision2 | 264 | decomp_liftd7_10.cpp | gamecore_sub_0142E8F0, updateUserPathWork, updateUserPathWorkEx | ClipCollisionDirectCallBack, gamecore_sub_01434CF8 | $a1: +0x0:W16; $a2: +0x0:R16; globals: 014336a0:& |  |
| 1437530 | chkEstimatCollisionCore | 1328 | decomp_liftd6_02.cpp | gamecore_sub_01441300, gamecore_sub_01444738 | CheckCollisionInsideOfObjectBoundary, ClipCollisionDirectCallBack, ClipCollisionDirectCallBackObjAry, GetGeometryPointerSurfacePlane, GetPlaneSlopeDegree +8 | $a0: +0x4:R4 +0x14:R4 +0x18:R4; $a2: +0x0:R16; globals: 001e0fb0:&R |  |
| 1437a78 | _setBackVolumeBaseMoveWork | 8 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x24:W4 | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 1437a80 | _setSlideVolumeBaseMoveWork | 8 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x28:W4 | SHIPPED (trace-checked); falls and teleport drops (lake, Valus' canyon) |
| 1437f90 | getHitPosWayCollision | 504 | recompiled | SCRFuncSetAttainHitPosByWayCollision | ClipCollisionDirectCallBackDoubleSide, getPathColiGroup, iosRotMatrixY | $a1: +0x0:W16; $a2: +0x0:R16; globals: 001e1020:&R 014336a0:& 0146c7e8:&R 0146c7ec:& 0146c80c:R | a colossus' attack aim by way collision (SCRFuncSetAttainHitPosByWayCollision: that fight under way) |
| 1444800 | gamecore_sub_01444800 | 32 | decomp_collision.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 144cd38 | setChkKeepHitWormCtrlWork | 8 | recompiled | updateScriptDataSystem | - | $a0: +0xd0:W4 | that colossus' own fight script (its arena, the fight under way) |
| 144cd78 | setGroundFrictionCoffWormCtrlWork | 8 | recompiled | updateScriptDataSystem | - | $a0: +0x90:W4 | that colossus' own fight script (its arena, the fight under way) |
| 14506e8 | SCRFuncResetVolumeTime0 | 32 | recompiled | - | getSCRDataCtrlWork | - | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1450708 | SCRFuncVolumeTime0Main | 232 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, getSCRDataCtrlWork | globals: 001e0ed4:R 014657e0:R | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1452138 | SCRFuncSetAttainHitPosByWayCollision | 96 | recompiled | - | getHitPosWayCollision, getSCRDataCtrlWork, setAttainmentPos | - | a colossus fight script's attack aim (that fight under way) |
| 1457868 | SCRConditionMyselfKeepHit | 32 | recompiled | - | GetCharacterClass | - | a fight: a weapon or a colossus' attack hitting a collision object |
| 14580a0 | SCRConditionVolumeTime0 | 56 | recompiled | - | getSCRDataCtrlWork | - | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 1458138 | SCRConditionVolumeCheckForPointPos | 112 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, getPointDataFromEvent, getSCRDataCtrlWork | - | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 14581a8 | SCRFuncVolumeCheckByGameObj | 304 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, equipmentInvisibilityCloak, getSCRDataCtrlWork | globals: 001e0ed4:R | a script's judge volume (an arena's trigger volumes; the volume time / check commands of the fight scripts) |
| 14582d8 | SCRFuncVolumeCheckByGameObjForSelfPos | 280 | decomp_scriptvm2.cpp | - | CheckPositionInsideOfJudgeVolumeObject, GetLayoutClass | globals: 001e0ed4:R |  |
| 1459360 | SCRConditionSwordFlareHitColAttr | 96 | recompiled | - | GetSwordFlareHitPlaneAttribute | - | a fight: a weapon or a colossus' attack hitting a collision object |
| 1459b60 | SCRConditionWeaponHitAttr | 112 | recompiled | - | getPlayerWeaponHitAttr, getSCRDataCtrlWork | - | a fight: a weapon or a colossus' attack hitting a collision object |
| 14651f0 | SCREventConditionVolumeCheckByGameObj | 280 | decomp_liftd8_02.cpp | - | CheckPositionInsideOfJudgeVolumeObject, getSScriptWork | globals: 001e0ed4:R |  |
