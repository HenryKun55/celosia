#!/usr/bin/env python3
"""Reads a scene recording (Debug "Record scene" / "Gravar cena", round 25).

A recording is a folder (ux0:data/celosia/rec/<date_time> on the Vita,
logs/rec/<date_time> on the Mac) written by
PS2Recomp/ps2xRuntime/src/lib/native/native_recorder.cpp: info.txt,
assets_NNN.bin, frames_NNN.bin, full_<frame>.bin, mc_slot1.ps2.

  tools/rec.py list   <rec>                       summary, full captures, layer 8 frames
  tools/rec.py frames <rec> [--from N] [--to N]   one line a frame: timing split, draws, layer 8, glow
  tools/rec.py draws  <rec> <frame> [--layer L]   per-draw table (the full capture's when there is one)
  tools/rec.py textures <rec> <frame> [--layer L] [--out DIR]   the frame's textures as PNG
  tools/rec.py render <rec> <frame|all> [--modes full,noglow,layers,upto] [--metal] [--warm 2] [--out DIR]
                                                  draws full captures again with this build's renderer
                                                  (OpenGL, the Vita's backend, by default) to PNGs;
                                                  --fix-bossmask TEX,U,V / --fix-meshdisp: round 27's
                                                  fixes previewed on a recording made before them
  tools/rec.py profile <rec> [--from N] [--to N] [--top 30]   the sampled guest pc / ra, named
  tools/rec.py pad    <rec> [--from N] [--to N]   the pad as the game read it (changes only)

Frames are the recording's own count (1 = the first frame recorded); the
game frame and VBlank are in `frames`.
"""
import argparse
import os
import struct
import subprocess
import sys
import zlib
from collections import Counter, defaultdict

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
MAGIC = 0x31425243

# record types (native_recorder.cpp)
R_MESH, R_SKIN, R_TEX, R_TEXSRC = 1, 2, 3, 4
R_BEGIN, R_DRAW, R_END = 10, 11, 12
R_FOG, R_BLOOM, R_BLUR, R_GLOW, R_DOF = 20, 21, 22, 23, 24
R_INFO, R_PAD, R_SAMPLES, R_NOTE = 30, 31, 32, 33
R_FMESH, R_PAL, R_FDRAW = 40, 41, 42

BLENDS = ['normal', 'add', 'opaque', 'sub']
FLAG_NAMES = ['bg', 'fogged', 'writeZ', 'noZ', 'overlay', 'screen', 'screenZ', 'shadowVol', 'mirrorSrc', 'mirrorSample',
              'refract', 'fresnel', 'screenCopy', 'prevFrame', 'lit', 'skinned']
PSM = {0x00: 'CT32', 0x01: 'CT24', 0x02: 'CT16', 0x13: 'T8', 0x14: 'T4', 0x1b: 'T8H', 0x24: 'T4HL', 0x2c: 'T4HH'}
VERTEX = struct.Struct('<8f4B')  # 36 bytes
SKINVERTEX_SIZE = 60


def blocks(path):
    with open(path, 'rb') as f:
        data = f.read()
    at = 0
    while at + 24 <= len(data):
        magic, kind, zipped, _, raw, stored, frame = struct.unpack_from('<IBBHIIQ', data, at)
        if magic != MAGIC:
            break
        body = data[at + 24:at + 24 + stored]
        at += 24 + stored
        if len(body) < stored:
            break
        yield kind, frame, (zlib.decompress(body) if zipped else body)


def records(data):
    at = 0
    while at + 8 <= len(data):
        t, n = struct.unpack_from('<II', data, at)
        yield t, data[at + 8:at + 8 + n]
        at += 8 + n


def numbered(rec, prefix):
    i, out = 0, []
    while os.path.exists(os.path.join(rec, '%s_%03d.bin' % (prefix, i))):
        out.append(os.path.join(rec, '%s_%03d.bin' % (prefix, i)))
        i += 1
    return out


def full_frames(rec):
    out = []
    for name in os.listdir(rec):
        if name.startswith('full_') and name.endswith('.bin'):
            out.append(int(name[5:-4]))
    return sorted(out)


# ---- decoding ----

def read_state(b, at):
    """DrawState as putState writes it; returns (dict, next offset)."""
    s = {}
    s['texture'], s['clamp'], s['blend'], s['order'], s['opacity'] = struct.unpack_from('<QBBIf', b, at)
    at += 18
    s['uvOffset'] = struct.unpack_from('<2f', b, at); at += 8
    s['lodK'], s['lodScale'], s['maxLod'] = struct.unpack_from('<3f', b, at); at += 12
    flags = struct.unpack_from('<8B', b, at); at += 8
    for k, v in zip(['background', 'fogged', 'writeDepth', 'noDepthWrite', 'overlay', 'screen', 'screenDepthTest', 'shadowVolume'], flags):
        s[k] = v
    s['shadow'], = struct.unpack_from('<f', b, at); at += 4
    s['reflectionSource'], s['reflectionSample'] = struct.unpack_from('<2B', b, at); at += 2
    s['reflectionTint'] = struct.unpack_from('<4f', b, at); at += 16
    s['refractionSample'], s['fresnel'] = struct.unpack_from('<2B', b, at); at += 2
    s['mirrorRipple'] = struct.unpack_from('<2f', b, at); at += 8
    s['screenCopy'], s['previousFrame'] = struct.unpack_from('<2B', b, at); at += 2
    s['debugColour'], s['skinPalette'], s['skinPaletteCount'] = struct.unpack_from('<3I', b, at); at += 12
    s['lit'], = struct.unpack_from('<B', b, at); at += 1
    s['light'] = struct.unpack_from('<32f', b, at); at += 128
    return s, at


def effect(t, b):
    if t == R_FOG:
        en, start, width, steps, order, vf, base, scale, c0, c1, c2 = struct.unpack_from('<BffiIBfffff', b)
        return 'fog on %d layer %u start %.1f width %.1f steps %d | vertex fog %d base %.1f scale %.1f colour %.2f %.2f %.2f' % (
            en, order, start, width, steps, vf, base, scale, c0, c1, c2)
    if t == R_BLOOM:
        v = struct.unpack_from('<BIfff3f4ff', b)
        return 'bloom on %d layer %u threshold %.1f z %.4g %.4g mask %.2f %.2f %.2f colour %.2f %.2f %.2f %.2f temporal %.3f' % v
    if t == R_BLUR:
        v = struct.unpack_from('<Iffffi', b)
        return 'screenBlur layer %u angle %.4f scale %.4f d %.2f %.2f steps %d' % v
    if t == R_GLOW:
        v = struct.unpack_from('<BIi3fff', b)
        return 'glow on %d layer %u rounds %d tint %.3f %.3f %.3f amounts A %.3f B %.3f' % v
    if t == R_DOF:
        v = struct.unpack_from('<BIffIii', b)
        return 'dof on %d layer %u z %.4g %.4g focusZ %u slices %d rounds %d' % v
    return ''


INFO = struct.Struct('<4Q9Q8I7QfB24I')
INFO_KEYS = (['gameFrame', 'vblank', 'timeUs', 'periodUs', 'hookUs', 'decodeUs', 'texUs', 'meshUs', 'renderUs', 'presentUs', 'waitUs', 'jobUs', 'joinUs',
              'draws', 'targets', 'texUploads', 'meshUploads', 'heldDraws', 'asyncJobs', 'asyncDone', 'jobs',
              'texBytes', 'meshBytes', 'schedWaitUs', 'cdUs', 'cdBytes', 'cdReads', 'vblanks', 'gameDt', 'renderThread'])


def parse_info(b):
    v = INFO.unpack_from(b)
    d = dict(zip(INFO_KEYS, v[:len(INFO_KEYS)]))
    d['guest'] = v[len(INFO_KEYS):]
    return d


def parse_compact(b):
    mesh, tex, order, blend, fate, n, flags, verts = struct.unpack_from('<QQIBBHII', b)
    cmin = struct.unpack_from('<4B', b, 32)
    cmax = struct.unpack_from('<4B', b, 36)
    opacity, skin = struct.unpack_from('<fH', b, 40)
    return dict(mesh=mesh, texture=tex, order=order, blend=blend, fate=fate, n=n, flags=flags, vertices=verts, cmin=cmin, cmax=cmax,
                opacity=opacity, skinCount=skin)


class Frames:
    """The compact frame blocks, by recording frame."""

    def __init__(self, rec):
        self.frames = {}
        for path in numbered(rec, 'frames'):
            for kind, frame, data in blocks(path):
                if kind != 2:
                    continue
                f = dict(frame=frame, draws=[], effects=[], pads=[], notes=[], samples=[], idle=0, full=False, info=None, end=None)
                for t, b in records(data):
                    if t == R_BEGIN:
                        f['full'] = bool(b[8])
                    elif t == R_DRAW:
                        f['draws'].append(parse_compact(b))
                    elif t == R_END:
                        f['end'] = struct.unpack_from('<IIB', b)
                    elif t in (R_FOG, R_BLOOM, R_BLUR, R_GLOW, R_DOF):
                        f['effects'].append((t, b))
                    elif t == R_INFO:
                        f['info'] = parse_info(b)
                    elif t == R_PAD:
                        vb, gf = struct.unpack_from('<QQ', b)
                        f['pads'].append((vb, gf, b[16:24]))
                    elif t == R_SAMPLES:
                        n, idle = struct.unpack_from('<II', b)
                        f['idle'] = idle
                        f['samples'] = [struct.unpack_from('<III', b, 8 + 12 * i) for i in range(n)]
                    elif t == R_NOTE:
                        bf, n = struct.unpack_from('<QI', b)
                        f['notes'].append((bf, b[12:12 + n].decode('utf-8', 'replace')))
                self.frames[frame] = f


def texture_sources(rec):
    src, sizes = {}, {}
    for path in numbered(rec, 'assets'):
        for kind, frame, data in blocks(path):
            for t, b in records(data):
                if t == R_TEXSRC:
                    tid, header, psm, w, h, fmt, clut = struct.unpack_from('<QIIIIIQ', b)
                    src[tid] = dict(header=header, psm=psm, w=w, h=h, format=fmt, clut=clut)
                elif t == R_TEX:
                    tid, w, h, levels, n = struct.unpack_from('<QIIII', b)
                    sizes[tid] = (w, h, levels)
    return src, sizes


def glow_of(f):
    for t, b in f['effects']:
        if t == R_GLOW:
            return struct.unpack_from('<BIi3fff', b)
    return None


# ---- commands ----

def cmd_list(a):
    print(open(os.path.join(a.rec, 'info.txt')).read().split('\ntuning')[0])
    for line in open(os.path.join(a.rec, 'info.txt')):
        if line.startswith('stopped'):
            print(line.strip())
    fr = Frames(a.rec)
    fulls = full_frames(a.rec)
    n = len(fr.frames)
    print('frames recorded: %d, full captures kept: %d' % (n, len(fulls)))
    if fulls:
        print('full captures: %s' % ' '.join(str(x) for x in fulls[:60]) + (' ...' if len(fulls) > 60 else ''))
    l8 = [k for k, f in sorted(fr.frames.items()) if any(d['order'] == 8 for d in f['draws'])]
    if l8:
        runs, start, prev = [], l8[0], l8[0]
        for k in l8[1:]:
            if k != prev + 1:
                runs.append((start, prev))
                start = k
            prev = k
        runs.append((start, prev))
        print('frames with layer 8 draws: %d, in runs %s' % (len(l8), ', '.join('%d-%d' % r for r in runs[:30])))
    sizes = defaultdict(int)
    for name in os.listdir(a.rec):
        sizes[name.split('_')[0]] += os.path.getsize(os.path.join(a.rec, name))
    print('bytes: ' + ', '.join('%s %.1f MB' % (k, v / 1048576.0) for k, v in sorted(sizes.items())))


def cmd_frames(a):
    fr = Frames(a.rec)
    print('%6s %7s %7s %6s | %5s %5s %5s %5s %5s %5s | %5s %4s %4s | %s' % ('rec', 'game', 'vblank', 'ms', 'game', 'hooks', 'rendr', 'prsnt', 'wait', 'jobs',
                                                                        'draws', 'held', 'l8', 'glow / notes'))
    for k in sorted(fr.frames):
        if (a.frm is not None and k < a.frm) or (a.to is not None and k > a.to):
            continue
        f = fr.frames[k]
        i = f['info'] or {}
        period = i.get('periodUs', 0) / 1000.0
        blocked = (i.get('waitUs', 0) if i.get('waitUs', 0) else i.get('renderUs', 0) + i.get('presentUs', 0)) / 1000.0
        game = period - i.get('hookUs', 0) / 1000.0 - blocked
        l8 = sum(1 for d in f['draws'] if d['order'] == 8)
        g = glow_of(f)
        gtxt = ('glow %d tint %.2f %.2f %.2f A %.2f B %.2f' % (g[0], g[3], g[4], g[5], g[6], g[7])) if g else ''
        notes = ' | '.join(n[1].split(': ', 1)[-1][:80] for n in f['notes'][:1])
        print('%6d %7d %7d %6.1f | %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f | %5d %4d %4d | %s%s %s' % (
            k, i.get('gameFrame', 0), i.get('vblank', 0), period, game, i.get('hookUs', 0) / 1000.0, i.get('renderUs', 0) / 1000.0,
            i.get('presentUs', 0) / 1000.0, i.get('waitUs', 0) / 1000.0, i.get('jobUs', 0) / 1000.0, len(f['draws']),
            sum(1 for d in f['draws'] if d['fate'] == 1), l8, 'F ' if f['full'] else '', gtxt, notes))


def load_full(rec, frame):
    path = os.path.join(rec, 'full_%06d.bin' % frame)
    if not os.path.exists(path):
        return None
    out = dict(draws=[], meshes={}, palettes={}, effects=[], end=None)
    for kind, fr, data in blocks(path):
        for t, b in records(data):
            if t == R_FDRAW:
                n, fate = struct.unpack_from('<IB', b)
                meshes = struct.unpack_from('<%dQ' % n, b, 5)
                clip = struct.unpack_from('<16f', b, 5 + 8 * n)
                s, _ = read_state(b, 5 + 8 * n + 64)
                out['draws'].append(dict(meshes=meshes, fate=fate, clip=clip, state=s))
            elif t == R_FMESH:
                mid, count, ni = struct.unpack_from('<QII', b)
                out['meshes'][mid] = (count, ni, b[16:16 + count * 36])
            elif t == R_PAL:
                h, count, rw = struct.unpack_from('<IIB', b)
                out['palettes'][h] = count
            elif t in (R_FOG, R_BLOOM, R_BLUR, R_GLOW, R_DOF):
                out['effects'].append((t, b))
            elif t == R_END:
                out['end'] = struct.unpack_from('<IIB', b)
    return out


def colour_range(raw):
    if not raw:
        return None
    lo, hi = [255] * 4, [0] * 4
    for i in range(0, len(raw), 36):
        c = raw[i + 32:i + 36]
        for k in range(4):
            lo[k] = min(lo[k], c[k])
            hi[k] = max(hi[k], c[k])
    return lo, hi


def cmd_draws(a):
    fr = Frames(a.rec)
    src, sizes = texture_sources(a.rec)
    f = fr.frames.get(a.frame)
    if not f:
        sys.exit('no frame %d (frames %d-%d)' % (a.frame, min(fr.frames), max(fr.frames)))
    full = load_full(a.rec, a.frame)
    i = f['info'] or {}
    print('frame %d: game frame %d vblank %d, %d draws%s' % (a.frame, i.get('gameFrame', 0), i.get('vblank', 0), len(f['draws']),
                                                         ', full capture' if full else ' (compact only: no full capture of this frame)'))
    for t, b in f['effects']:
        print('  ' + effect(t, b))
    for n in f['notes']:
        print('  note (backend frame %d): %s' % n)
    print('%4s %3s %4s %-7s %-16s %3s %6s %-26s %-17s %-17s %5s %s' % ('#', 'lay', 'fate', 'blend', 'mesh', 'n', 'verts', 'texture', 'colour min', 'colour max',
                                                                      'opac', 'flags'))
    for k, d in enumerate(f['draws']):
        if a.layer is not None and d['order'] != a.layer:
            continue
        t = d['texture']
        if t:
            s = src.get(t)
            w, h, lv = sizes.get(t, (0, 0, 0))
            ttxt = '%016x' % t
            if s:
                ttxt += ' %s@%06x' % (PSM.get(s['psm'], hex(s['psm'])), s['header'])
            elif w:
                ttxt += ' %dx%d' % (w, h)
        else:
            ttxt = '-'
        flags = [n for b, n in enumerate(FLAG_NAMES) if d['flags'] >> b & 1]
        if d['flags'] >> 16 & 3:
            flags.append('clamp%d' % (d['flags'] >> 16 & 3))
        print('%4d %3d %4s %-7s %016x %3d %6d %-26s %-17s %-17s %5.2f %s' % (
            k, d['order'], ['', 'held', 'hid'][d['fate']], BLENDS[d['blend']] if d['blend'] < 4 else d['blend'], d['mesh'], d['n'], d['vertices'], ttxt[:26],
            '%3d %3d %3d %3d' % tuple(d['cmin']), '%3d %3d %3d %3d' % tuple(d['cmax']), d['opacity'], ' '.join(flags)))
    if full and a.verbose:
        print('\nfull capture states (layer %s):' % ('all' if a.layer is None else a.layer))
        for k, d in enumerate(full['draws']):
            s = d['state']
            if a.layer is not None and s['order'] != a.layer:
                continue
            extra = ''
            for m in d['meshes']:
                if m in full['meshes']:
                    r = colour_range(full['meshes'][m][2])
                    if r:
                        extra = ' frame-mesh colours %s..%s' % (r[0], r[1])
                    break
            print('%4d layer %d meshes %s tint %s uvOff %s lod %.2f/%.2f/%.2f skin %d/%d lit %d%s' % (
                k, s['order'], ' '.join('%x' % m for m in d['meshes'][:4]), ['%.2f' % x for x in s['reflectionTint']], ['%.3f' % x for x in s['uvOffset']],
                s['lodK'], s['lodScale'], s['maxLod'], s['skinPalette'], s['skinPaletteCount'], s['lit'], extra))
            print('      clip %s' % ' '.join('%.3g' % x for x in d['clip']))


def cmd_textures(a):
    from PIL import Image
    fr = Frames(a.rec)
    f = fr.frames.get(a.frame)
    if not f:
        sys.exit('no frame %d' % a.frame)
    want = {d['texture'] for d in f['draws'] if d['texture'] and (a.layer is None or d['order'] == a.layer)}
    out = a.out or os.path.join(a.rec, 'textures_%06d' % a.frame)
    os.makedirs(out, exist_ok=True)
    src, _ = texture_sources(a.rec)
    found = 0
    for path in numbered(a.rec, 'assets'):
        for kind, frame, data in blocks(path):
            for t, b in records(data):
                if t != R_TEX:
                    continue
                tid, w, h, levels, n = struct.unpack_from('<QIIII', b)
                if tid not in want:
                    continue
                rgba = bytearray(b[24:24 + w * h * 4])
                for i in range(3, len(rgba), 4):  # PS2 alpha: 0x80 = opaque
                    rgba[i] = min(255, rgba[i] * 2)
                s = src.get(tid)
                name = '%016x' % tid + ('_%s_%06x' % (PSM.get(s['psm'], hex(s['psm'])), s['header']) if s else '') + '_%dx%d.png' % (w, h)
                Image.frombytes('RGBA', (w, h), bytes(rgba)).save(os.path.join(out, name))
                found += 1
    print('%d of %d textures written to %s' % (found, len(want), out))


def runner():
    return os.path.join(ROOT, 'build/cmake/ps2xRuntime/ps2EntryRunner')


def cmd_render(a):
    frames = 'all' if a.frame == 'all' else a.frame
    out = a.out or os.path.join(a.rec, 'png')
    os.makedirs(out, exist_ok=True)
    env = dict(os.environ)
    env.update(SOTC_REC_REPLAY=os.path.abspath(a.rec), SOTC_REC_FRAMES=frames, SOTC_REC_OUT=os.path.abspath(out), SOTC_REC_MODES=a.modes,
               SOTC_REC_WARM=str(a.warm), PS2X_NO_SPU2_AUDIO='1', SOTC_NATIVE='1')
    if not a.metal:
        env['SOTC_NATIVE_GL'] = '1'  # the Vita's backend (and GPU skinning, which the Vita's skinned draws need)
    if a.fix_bossmask:
        # round 27's preview for older recordings: that texture's draws get the
        # death mask, its radius each frame from the recorded 0x1186298
        fr = Frames(a.rec).frames
        radius = ','.join('%d:%g' % (k, struct.unpack('<f', struct.pack('<I', f['info']['guest'][2]))[0]) for k, f in sorted(fr.items()) if f['info'])
        env.update(SOTC_REC_FIX_BOSSMASK=a.fix_bossmask, SOTC_REC_FIX_RADIUS=radius)
    if a.fix_meshdisp:
        env['SOTC_REC_FIX_MESHDISP'] = '1'
    disc = os.environ.get('CELOSIA_DISC', os.path.join(ROOT, 'disc'))
    cmd = [runner(), os.path.join(disc, 'SCUS_974.72')]
    print('running', ' '.join(cmd), '(replay of %s, frames %s)' % (a.rec, frames))
    r = subprocess.run(cmd, env=env, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors='replace')
    for line in r.stdout.splitlines():
        if line.startswith('[replay]'):
            print(line)
    print('pictures in', out)
    return r.returncode


def functions():
    import bisect
    entries = []
    path = os.path.join(ROOT, 'recomp/game/functions.csv')
    if not os.path.exists(path):
        return lambda a: '%08x' % a
    for line in open(path):
        p = line.strip().split(',')
        try:
            entries.append((int(p[1], 16), int(p[2], 16), p[0]))
        except (IndexError, ValueError):
            continue
    entries.sort()
    starts = [e[0] for e in entries]

    def name(a):
        i = bisect.bisect_right(starts, a) - 1
        if i >= 0 and entries[i][0] <= a < entries[i][1]:
            return entries[i][2]
        return '%08x' % a
    return name


def cmd_profile(a):
    fr = Frames(a.rec)
    name = functions()
    by_pc, by_ra, total, idle, frames = Counter(), Counter(), 0, 0, 0
    for k, f in fr.frames.items():
        if (a.frm is not None and k < a.frm) or (a.to is not None and k > a.to):
            continue
        frames += 1
        idle += f['idle']
        for pc, ra, n in f['samples']:
            by_pc[name(pc)] += n
            by_ra[(name(pc), name(ra))] += n
            total += n
    if not total:
        sys.exit('no samples')
    print('%d frames, %d samples running, %d idle (%.0f%% of the samples the EE was waiting)' % (frames, total, idle, 100.0 * idle / max(1, total + idle)))
    for fn, n in by_pc.most_common(a.top):
        callers = sorted(((c, m) for (f2, c), m in by_ra.items() if f2 == fn), key=lambda x: -x[1])[:2]
        print('%6.2f%%  %-40s  %s' % (100.0 * n / total, fn, ', '.join('%s %.0f%%' % (c, 100.0 * m / n) for c, m in callers)))


def cmd_pad(a):
    fr = Frames(a.rec)
    last = None
    for k in sorted(fr.frames):
        if (a.frm is not None and k < a.frm) or (a.to is not None and k > a.to):
            continue
        for vb, gf, d in fr.frames[k]['pads']:
            if d != last:
                buttons = ~(d[2] | d[3] << 8) & 0xFFFF
                print('rec %6d game %7d vblank %8d buttons %04x sticks %3d %3d %3d %3d' % (k, gf, vb, buttons, d[4], d[5], d[6], d[7]))
                last = d


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest='cmd', required=True)
    s = sub.add_parser('list'); s.add_argument('rec')
    for nm in ('frames', 'profile', 'pad'):
        s = sub.add_parser(nm); s.add_argument('rec'); s.add_argument('--from', dest='frm', type=int); s.add_argument('--to', type=int)
        if nm == 'profile':
            s.add_argument('--top', type=int, default=30)
    s = sub.add_parser('draws'); s.add_argument('rec'); s.add_argument('frame', type=int); s.add_argument('--layer', type=int); s.add_argument('-v', '--verbose', action='store_true')
    s = sub.add_parser('textures'); s.add_argument('rec'); s.add_argument('frame', type=int); s.add_argument('--layer', type=int); s.add_argument('--out')
    s = sub.add_parser('render'); s.add_argument('rec'); s.add_argument('frame'); s.add_argument('--modes', default='full,noglow,layers')
    s.add_argument('--metal', action='store_true'); s.add_argument('--warm', type=int, default=2); s.add_argument('--out')
    s.add_argument('--fix-bossmask', help='TEX,U,V: preview of round 27 on an older recording (that texture -> the death mask at centre U,V)')
    s.add_argument('--fix-meshdisp', action='store_true', help="preview of round 27: layer 18's meshDisp grids by the frame's projection")
    a = p.parse_args()
    return {'list': cmd_list, 'frames': cmd_frames, 'draws': cmd_draws, 'textures': cmd_textures, 'render': cmd_render, 'profile': cmd_profile,
            'pad': cmd_pad}[a.cmd](a)


if __name__ == '__main__':
    sys.exit(main() or 0)
