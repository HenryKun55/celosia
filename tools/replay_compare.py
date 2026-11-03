#!/usr/bin/env python3
"""The input replay's report (tools/replay_compare.sh): one HTML page with
the determinism check and one row per MARK of the recording.

Usage: replay_compare.py <recording> <out dir> <label>=<run dir> ...
  <recording>  the SOTC_INPUT_RECORD file (its S and MARK lines)
  <run dir>    a replay's folder: run.log or err.txt with its [rstate] lines,
               and its pictures of tick N as snap_N.png / native_snap_N.png
               (the Mac, in shots/) or t_N.png (Vita3K window captures);
               "<label>:native=<run dir>" takes the native_snap_ pictures.
Writes <out dir>/index.html and <out dir>/img/, prints the determinism table.
"""
import html
import os
import re
import shutil
import struct
import sys
import zlib


def read_recording(path):
    states, marks, info, ticks = {}, [], [], 0
    with open(path, errors="replace") as f:
        for line in f:
            if line.startswith("S "):
                v = line.split()[1:]
                states[int(v[0])] = [float(x) for x in v[1:]]
            elif line.startswith("MARK "):
                n, t = line.split()[1:3]
                marks.append((int(n), int(t)))
            elif line.startswith("T "):
                ticks += 1
            elif line.split(" ", 1)[0] in ("platform", "date", "memcard", "settings"):
                info.append(line.strip())
    return states, marks, info, ticks


def read_run(folder):
    states, notes = {}, []
    for name in ("run.log", "err.txt"):
        p = os.path.join(folder, name)
        if not os.path.exists(p):
            continue
        with open(p, errors="replace") as f:
            for line in f:
                if line.startswith("[rstate] "):
                    v = line.split()[1:]
                    try:
                        states[int(v[0])] = [float(x) for x in v[1:]]
                    except ValueError:
                        pass
                elif line.startswith("[replay]") and (" hold " not in line and " shot " not in line):
                    notes.append(line.strip())
    return states, notes


def pictures(folder, native):
    """tick -> picture path."""
    out = {}
    for sub in ("shots", "."):
        d = os.path.join(folder, sub)
        if not os.path.isdir(d):
            continue
        for name in os.listdir(d):
            m = re.fullmatch(r"(native_snap|snap|t)_(\d+)\.(png|ppm)", name)
            if not m:
                continue
            kind = m.group(1)
            if (kind == "native_snap") != native:
                continue
            out.setdefault(int(m.group(2)), os.path.join(d, name))
    return out


def ppm_to_png(src, dst):
    with open(src, "rb") as f:
        data = f.read()
    parts = data.split(maxsplit=4)
    w, h = int(parts[1]), int(parts[2])
    pixels = parts[4][: w * h * 3]
    raw = b"".join(b"\x00" + pixels[y * w * 3:(y + 1) * w * 3] for y in range(h))
    def chunk(kind, body):
        c = struct.pack(">I", len(body)) + kind + body
        return c + struct.pack(">I", zlib.crc32(kind + body) & 0xFFFFFFFF)
    with open(dst, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) +
                chunk(b"IDAT", zlib.compress(raw, 6)) + chunk(b"IEND", b""))


GROUPS = (("Wander", 0), ("Agro", 3), ("camera", 6))


def compare(ref, run):
    """Per group: worst distance, first tick off by more than 0.001 and by more than 1 unit."""
    common = sorted(set(ref) & set(run))
    res = {"samples": len(common), "missing": len(set(ref) - set(run)), "step": [None, None]}
    for name, at in GROUPS:
        worst, first_small, first_big, worst_tick = 0.0, None, None, None
        for t in common:
            a, b = ref[t][at:at + 3], run[t][at:at + 3]
            d = sum((x - y) ** 2 for x, y in zip(a, b)) ** 0.5
            if d > worst:
                worst, worst_tick = d, t
            if d > 0.001 and first_small is None:
                first_small = t
            if d > 1.0 and first_big is None:
                first_big = t
        res[name] = (worst, worst_tick, first_small, first_big)
    for t in common:
        if abs(ref[t][9] - run[t][9]) > 1e-6:
            res["step"] = [t, (ref[t][9], run[t][9])]
            break
    return res


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    rec, out = sys.argv[1], sys.argv[2]
    runs = []
    for arg in sys.argv[3:]:
        label, folder = arg.split("=", 1)
        native = label.endswith(":native")
        runs.append((label[:-7] if native else label, folder, native))
    states, marks, info, ticks = read_recording(rec)
    os.makedirs(os.path.join(out, "img"), exist_ok=True)

    # Determinism.
    rows, table = [], []
    for label, folder, native in runs:
        if native:
            continue
        rs, notes = read_run(folder)
        c = compare(states, rs)
        line = f"{label:14s} {c['samples']:5d} samples"
        cells = [html.escape(label), str(c["samples"]) + (f" ({c['missing']} missing)" if c["missing"] else "")]
        for name, _ in GROUPS:
            worst, wt, fs, fb = c[name]
            line += f"  {name} max {worst:.4f}" + (f" @{wt}" if wt is not None else "")
            cells.append(f"{worst:.4f}" + (f" (tick {wt})" if worst else "") +
                         (f"<br><small>first &gt;0.001: {fs}; &gt;1: {fb}</small>" if fs is not None else ""))
        step = c["step"]
        cells.append("same" if step[0] is None else f"differs from tick {step[0]} ({step[1][0]:.6f} / {step[1][1]:.6f})")
        cells.append("<br>".join(html.escape(n) for n in notes[:8]))
        table.append(line)
        rows.append(cells)
    print("\n".join(table))

    # One row per mark: each run's picture at each tick around the mark.
    pics = {(label, native): pictures(folder, native) for label, folder, native in runs}
    rec_dir = os.path.dirname(os.path.abspath(rec))
    body = []
    for n, tick in marks:
        ticks_here = sorted({t for (label, native), p in pics.items() for t in p if abs(t - tick) <= 30})
        if tick not in ticks_here:
            ticks_here.append(tick)
            ticks_here.sort()
        body.append(f'<h2 id="mark{n}">MARK {n} &mdash; tick {tick}</h2>')
        console = os.path.join(rec_dir, os.path.basename(rec) + f".mark{n}.ppm")
        if os.path.exists(console):
            dst = os.path.join(out, "img", f"console_mark{n}.png")
            ppm_to_png(console, dst)
            body.append(f'<p>The screen when it was marked (console): <a href="img/console_mark{n}.png"><img class="small" src="img/console_mark{n}.png"></a></p>')
        body.append("<table><tr><th>tick</th>" + "".join(f"<th>{html.escape(l)}</th>" for l, _, _ in runs) + "</tr>")
        for t in ticks_here:
            cls = ' class="mark"' if t == tick else ""
            cells = []
            for label, folder, native in runs:
                src = pics[(label, native)].get(t)
                if not src:
                    cells.append("<td>&mdash;</td>")
                    continue
                safe = re.sub(r"[^A-Za-z0-9]", "_", label)
                dst = os.path.join(out, "img", f"{safe}_{t}.png")
                if src.endswith(".ppm"):
                    ppm_to_png(src, dst)
                else:
                    shutil.copyfile(src, dst)
                cells.append(f'<td><a href="img/{safe}_{t}.png"><img src="img/{safe}_{t}.png"></a></td>')
            off = t - tick
            body.append(f"<tr{cls}><td>{t}<br><small>{'mark' if off == 0 else f'{off:+d}'}</small></td>" + "".join(cells) + "</tr>")
        body.append("</table>")

    head = "".join(f"<li>{html.escape(i)}</li>" for i in info)
    det = "<table><tr><th>replay</th><th>checkpoints</th>" + "".join(f"<th>{g} (max distance)</th>" for g, _ in GROUPS) + \
          "<th>step</th><th>notes</th></tr>" + "".join("<tr>" + "".join(f"<td>{c}</td>" for c in r) + "</tr>" for r in rows) + "</table>"
    index = "".join(f'<a href="#mark{n}">MARK {n} (tick {t})</a> ' for n, t in marks)
    page = f"""<!DOCTYPE html><html><head><meta charset="utf-8"><title>Replay {html.escape(os.path.basename(out))}</title>
<style>
body {{ font: 14px -apple-system, sans-serif; background: #111; color: #ddd; margin: 16px; }}
a {{ color: #8cf; }} table {{ border-collapse: collapse; margin: 8px 0 24px; }}
td, th {{ border: 1px solid #333; padding: 4px; vertical-align: top; text-align: left; }}
img {{ width: 100%; max-width: 640px; display: block; image-rendering: pixelated; }}
img.small {{ max-width: 320px; display: inline; }}
tr.mark td {{ background: #322; }} h2 {{ margin-top: 32px; }}
</style></head><body>
<h1>Input replay: {html.escape(os.path.basename(rec))}</h1>
<ul>{head}<li>{ticks} ticks recorded, {len(marks)} marks</li></ul>
<h2>Determinism</h2>
<p>The game's own state (Wander's and Agro's positions, the camera's eye, the frame's step) every 30 ticks
of each replay against the recording's. 0 means the same to the printed 4 decimals.</p>
{det}
<h2>Marks</h2><p>{index}</p>
<p>Each picture is the frame of that tick (the logic frame), held on screen and captured at full resolution;
click for the file.</p>
{''.join(body)}
</body></html>"""
    with open(os.path.join(out, "index.html"), "w") as f:
        f.write(page)
    print(f"report: {os.path.join(out, 'index.html')}")


if __name__ == "__main__":
    main()
