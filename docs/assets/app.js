(() => {
  const RUNTIME = 'Runtime (DMA walk, memory, scheduler)';
  const KIND = { native: 'Native code', recompiled: 'Recompiled MIPS', runtime: 'Runtime' };
  const kindVar = (k) => k === 'native' ? 'var(--native)' : k === 'runtime' ? 'var(--runtime)' : 'var(--mips)';
  const inkVar = (k) => k === 'native' ? 'var(--native-ink)' : k === 'runtime' ? 'var(--runtime-ink)' : 'var(--mips-ink)';
  const pct = (v, d = 1) => (v === null || v === undefined || isNaN(v)) ? '—' : Number(v).toFixed(d) + '%';
  const $ = (id) => document.getElementById(id);
  const areaName = (a) => a === 'Other game code' ? 'Game code, names not identified yet' : a === RUNTIME ? 'Runtime' : a;
  const load = (f) => fetch(f).then((r) => { if (!r.ok) throw new Error(f + ': ' + r.status); return r.json(); });

  const motion = !window.matchMedia || !window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  const touch = !!(window.matchMedia && window.matchMedia('(hover: none)').matches);
  const verb = touch ? 'Tap' : 'Click';
  // Treemap height for a width: taller on phones so the tiles are big enough to tap.
  const phone = () => !!(window.matchMedia && window.matchMedia("(max-width: 40rem)").matches);
  const mapHeight = (w, ratio) => Math.round(Math.max(260, Math.min(w * (phone() ? Math.max(ratio || 0.7, 1.05) : (ratio || 0.7)), 520)));
  // Long identifiers (decomp_collision::clipDBT, manager_sub_0113ACB8) may break after :: and _.
  function breakable(text) {
    const f = document.createDocumentFragment();
    if (!phone()) { f.append(text); return f; }
    text.split(/(?<=::|_)/).forEach((part, i) => { if (i) f.appendChild(document.createElement('wbr')); f.append(part); });
    return f;
  }
  // Card layout on phones labels each cell with its column's header.
  function labelCells(table) {
    const heads = [...table.querySelectorAll('thead th')].map((th) => th.textContent);
    table.querySelectorAll('tbody tr').forEach((tr) => [...tr.children].forEach((td, i) => { if (heads[i]) td.dataset.label = heads[i]; }));
  }
  // Numbers count up to their value the first time they scroll into view.
  const seen = new WeakSet();
  function countTo(el, value, fmt) {
    if (!motion || seen.has(el)) { el.textContent = fmt(value); return; }
    seen.add(el);
    const run = () => {
      const t0 = performance.now(), dur = 1100;
      const step = (t) => { const k = Math.min(1, (t - t0) / dur), e = 1 - Math.pow(1 - k, 3); el.textContent = fmt(value * e); if (k < 1) requestAnimationFrame(step); };
      requestAnimationFrame(step);
    };
    el.textContent = fmt(0);
    if (!('IntersectionObserver' in window)) { run(); return; }
    const io = new IntersectionObserver((es) => { if (es.some((e) => e.isIntersecting)) { io.disconnect(); run(); } }, { threshold: 0.3 });
    io.observe(el);
  }
  // Charts and blocks get a class when they first come into view (CSS animates it).
  function onView(el, fn) {
    if (!motion || !('IntersectionObserver' in window)) { fn(); return; }
    const io = new IntersectionObserver((es) => { if (es.some((e) => e.isIntersecting)) { io.disconnect(); fn(); } }, { threshold: 0.15 });
    io.observe(el);
  }

  function tipFor(el) {
    let t = el.querySelector('.tip');
    if (!t) { t = document.createElement('div'); t.className = 'tip'; t.hidden = true; el.appendChild(t); }
    return t;
  }
  function placeTip(el, tip, ev) {
    const box = el.getBoundingClientRect();
    const x = ev.clientX - box.left, y = ev.clientY - box.top;
    tip.hidden = false;
    const tw = tip.offsetWidth, th = tip.offsetHeight;
    tip.style.left = Math.max(0, Math.min(x + 14, box.width - tw)) + 'px';
    tip.style.top = Math.max(0, Math.min(y + 14, box.height - th)) + 'px';
  }
  // Hover tips; on touch, a tap shows the tip and a tap elsewhere hides it.
  function hoverTip(sel, el, tip, linesFor) {
    let kind = 'mouse';
    sel.on('pointerdown', (ev) => { kind = ev.pointerType || 'mouse'; })
      .on('pointermove', (ev, d) => { if (ev.pointerType && ev.pointerType !== 'mouse') return; setTip(tip, linesFor(d, ev)); placeTip(el, tip, ev); })
      .on('pointerleave', (ev) => { if (ev.pointerType && ev.pointerType !== 'mouse') return; tip.hidden = true; })
      .on('click.tip', (ev, d) => { if (kind === 'mouse' && !touch) return; setTip(tip, linesFor(d, ev)); placeTip(el, tip, ev); });
    if (!el._tipOff) { el._tipOff = true; document.addEventListener('pointerdown', (ev) => { if (!el.contains(ev.target)) el.querySelectorAll('.tip').forEach((t) => { t.hidden = true; }); }, { passive: true }); }
  }
  function setTip(tip, lines) {
    tip.textContent = '';
    lines.forEach(([cls, text], i) => { const d = document.createElement(i === 0 ? 'b' : 'div'); if (cls) d.className = cls; d.textContent = text; tip.appendChild(d); if (i === 0) tip.appendChild(document.createElement('br')); });
  }
  function fitText(node, text, room) {
    node.textContent = text;
    if (room < 18) { node.textContent = ''; return; }
    let t = text;
    while (t.length > 1 && node.getComputedTextLength() > room) { t = t.slice(0, -1); node.textContent = t + '…'; }
  }

  // Treemap: groups (areas) of leaves sized by value. Clicking a group (or a
  // leaf of it) opens opts.drill(group) as its own map; the crumb goes back.
  function treemap(el, groups, opts) {
    const crumb = $(el.id + '-crumb');
    const state = el._zoom || null;
    el.textContent = ''; if (crumb) crumb.textContent = '';
    const w = Math.max(280, el.clientWidth), h = mapHeight(w, opts.ratio);
    el.style.minHeight = '';
    const svg = d3.select(el).append('svg').attr('viewBox', `0 0 ${w} ${h}`).attr('role', 'img').attr('aria-label', opts.label);
    const tip = tipFor(el);
    const drilled = state && opts.drill ? opts.drill(state) : null;
    const layer = svg.append('g');
    // Zoom: opening an area grows its tile to the whole map; going back
    // shrinks the map into the area's tile.
    const zoomFrom = el._from; el._from = null;
    const zoomed = (inverse) => {
      if (!motion || !zoomFrom) { if (motion && !el._entered) { el._entered = true; svg.classed('enter', true); } return; }
      const r = zoomFrom, sx = (r.x1 - r.x0) / w, sy = (r.y1 - r.y0) / h;
      const t = inverse ? `translate(${-r.x0 / sx},${-r.y0 / sy}) scale(${1 / sx},${1 / sy})` : `translate(${r.x0},${r.y0}) scale(${sx},${sy})`;
      layer.attr('transform', t).attr('opacity', inverse ? 0 : 0.4).transition().duration(560).ease(d3.easeCubicOut).attr('transform', 'translate(0,0) scale(1,1)').attr('opacity', 1);
    };
    if (crumb) {
      if (drilled) {
        const back = document.createElement('button'); back.type = 'button'; back.textContent = '‹ All areas';
        back.addEventListener('click', () => { el._from = el._area; el._zoom = null; treemap(el, groups, opts); });
        crumb.appendChild(back);
        const t = document.createElement('b'); t.textContent = areaName(state); crumb.appendChild(t);
        const sum = document.createElement('span'); sum.textContent = opts.summary ? opts.summary(state) : ''; crumb.appendChild(sum);
      } else {
        const sp = document.createElement('span'); sp.textContent = (opts.hint || 'Click an area to open it.').replace(/^Click/, verb); crumb.appendChild(sp);
      }
    }
    let chips = $(el.id + '-areas');
    if (!chips && opts.drill) { chips = document.createElement('div'); chips.className = 'areas'; chips.id = el.id + '-areas'; chips.setAttribute('role', 'group'); chips.setAttribute('aria-label', 'Open an area'); el.after(chips); }
    if (chips) { chips.textContent = ''; chips.hidden = !!drilled; }
    if (drilled) {
      const items = drilled.filter((i) => i.value > 0);
      const total = d3.sum(items, (i) => i.value) || 1;
      const root = d3.hierarchy({ children: items }).sum((d) => d.value || 0).sort((a, b) => b.value - a.value);
      d3.treemap().size([w, h]).padding(2).round(true)(root);
      const leaf = layer.selectAll('g').data(root.leaves()).join('g').attr('class', 'l');
      leaf.append('rect').attr('x', (d) => d.x0).attr('y', (d) => d.y0).attr('width', (d) => Math.max(0, d.x1 - d.x0)).attr('height', (d) => Math.max(0, d.y1 - d.y0))
        .attr('rx', 2).attr('fill', (d) => opts.color(d.data)).attr('opacity', (d) => d.data.more ? 0.7 : 1);
      leaf.append('text').attr('x', (d) => d.x0 + 5).attr('y', (d) => d.y0 + 15).attr('font-size', 11).attr('font-weight', 600).attr('font-family', 'var(--body)').attr('fill', (d) => opts.ink(d.data))
        .each(function (d) { if (d.y1 - d.y0 < 18) { this.textContent = ''; return; } fitText(this, opts.itemTop ? opts.itemTop(d.data) : (100 * d.value / total).toFixed(1) + '%', d.x1 - d.x0 - 10); });
      leaf.append('text').attr('x', (d) => d.x0 + 5).attr('y', (d) => d.y0 + 29).attr('font-size', 10).attr('font-family', 'var(--mono)').attr('fill', (d) => opts.ink(d.data))
        .each(function (d) { if (d.y1 - d.y0 < 34) { this.textContent = ''; return; } fitText(this, opts.itemName(d.data), d.x1 - d.x0 - 10); });
      hoverTip(leaf, el, tip, (d) => opts.itemTip(d.data, d.value, total));
      zoomed(false);
      return;
    }
    const root = d3.hierarchy({ children: groups.map((g) => ({ name: g.name, children: g.items.filter((i) => i.value > 0) })).filter((g) => g.children.length) })
      .sum((d) => d.value || 0).sort((a, b) => b.value - a.value);
    d3.treemap().size([w, h]).paddingOuter(2).paddingTop(19).paddingInner(2).round(true)(root);
    const total = root.value || 1;
    const open = (name, fromChip) => {
      if (!opts.drill || !opts.drill(name)) return;
      tip.hidden = true;
      const a = (root.children || []).find((c) => c.data.name === name);
      el._area = a ? { x0: a.x0, y0: a.y0, x1: a.x1, y1: a.y1 } : null; el._from = el._area;
      el._zoom = name; treemap(el, groups, opts);
      if (fromChip) { const c = $(el.id + '-crumb'); const b = c && c.querySelector('button'); if (b) b.focus({ preventScroll: true }); if (c && c.getBoundingClientRect().top < 60) c.scrollIntoView({ block: 'start', behavior: motion ? 'smooth' : 'auto' }); }
    };
    if (chips) for (const a of (root.children || [])) {
      if (!opts.drill(a.data.name)) continue;
      const b = document.createElement('button'); b.type = 'button';
      const nat = d3.sum(a.children, (c) => c.data.kind === 'native' || c.data.status === 'native' ? c.value : 0);
      const sw = document.createElement('i'); sw.style.background = nat >= a.value / 2 ? 'var(--native)' : 'var(--mips)'; b.appendChild(sw);
      b.append(areaName(a.data.name) + ' '); const v = document.createElement('span'); v.textContent = opts.fmt(a.value, total); b.appendChild(v);
      b.addEventListener('click', () => open(a.data.name, true));
      chips.appendChild(b);
    }
    const g = layer.selectAll('g.a').data(root.children || []).join('g').attr('class', opts.drill ? 'a click' : 'a')
      .attr('tabindex', opts.drill ? 0 : null).attr('role', opts.drill ? 'button' : null).attr('aria-label', (d) => 'Open ' + areaName(d.data.name))
      .on('click', (ev, d) => open(d.data.name)).on('keydown', (ev, d) => { if (ev.key === 'Enter' || ev.key === ' ') { ev.preventDefault(); open(d.data.name); } });
    g.append('rect').attr('x', (d) => d.x0).attr('y', (d) => d.y0).attr('width', (d) => d.x1 - d.x0).attr('height', (d) => d.y1 - d.y0)
      .attr('rx', 3).attr('fill', 'var(--bg)').attr('stroke', 'var(--line)');
    g.append('text').attr('x', (d) => d.x0 + 5).attr('y', (d) => d.y0 + 14).attr('fill', 'var(--ink)')
      .attr('font-family', 'var(--body)').attr('font-weight', 600).attr('font-size', 12)
      .each(function (d) { fitText(this, areaName(d.data.name) + '  ' + opts.fmt(d.value, total), d.x1 - d.x0 - 10); });
    const leaf = g.selectAll('g.l').data((d) => d.children).join('g').attr('class', 'l').style('animation-delay', (d, i) => (i * 40) + 'ms');
    leaf.append('rect').attr('x', (d) => d.x0).attr('y', (d) => d.y0).attr('width', (d) => Math.max(0, d.x1 - d.x0)).attr('height', (d) => Math.max(0, d.y1 - d.y0))
      .attr('rx', 2).attr('fill', (d) => opts.color(d.data));
    leaf.append('text').attr('x', (d) => d.x0 + 5).attr('y', (d) => d.y0 + 15).attr('font-size', 11).attr('font-weight', 600).attr('font-family', 'var(--body)')
      .attr('fill', (d) => opts.ink(d.data))
      .each(function (d) { if (d.y1 - d.y0 < 18) { this.textContent = ''; return; } fitText(this, opts.leafLabel(d.data, d.value, d.parent.value, total), d.x1 - d.x0 - 10); });
    leaf.append('text').attr('x', (d) => d.x0 + 5).attr('y', (d) => d.y0 + 29).attr('font-size', 10).attr('font-family', 'var(--body)')
      .attr('fill', (d) => opts.ink(d.data))
      .each(function (d) { if (!opts.leafSub || d.y1 - d.y0 < 34) { this.textContent = ''; return; } fitText(this, opts.leafSub(d.data, d.value, d.parent.value, total), d.x1 - d.x0 - 10); });
    if (!touch) leaf.on('pointermove', (ev, d) => { if (ev.pointerType && ev.pointerType !== 'mouse') return; setTip(tip, opts.tip(d, total)); placeTip(el, tip, ev); }).on('pointerleave', () => { tip.hidden = true; });
    g.style('animation-delay', (d, i) => (i * 60) + 'ms');
    zoomed(true);
  }

  function drawLede(tree) {
    const sum = (k) => d3.sum(tree.rows.filter((r) => r.kind === k), (r) => r.time);
    const parts = [['native', sum('native'), 'k-native', 'native'], ['recompiled', sum('recompiled'), 'k-mips', 'MIPS'], ['runtime', sum('runtime'), 'k-runtime', 'runtime']];
    const bar = $('lede-bar'); bar.textContent = '';
    const cells = [];
    for (const [, v, cls, name] of parts) { const d = document.createElement('div'); d.className = cls; d.style.flex = motion ? '0 0 0%' : `0 0 calc(${v}% - 2px)`; d.textContent = `${v.toFixed(1)}% ${name}`; d.title = d.textContent; bar.appendChild(d); cells.push([d, v]); }
    if (motion) requestAnimationFrame(() => requestAnimationFrame(() => cells.forEach(([d, v]) => { d.style.flex = `0 0 calc(${v}% - 2px)`; })));
    $('lede-note').textContent = `Share of the Vita game thread's instructions in a profile of round ${String(tree.label).replace(/^r/, '')}'s build on the usual route (Vita3K at 222 MHz, 180-300 s). The game thread is what limits the frame rate: on the console it needs 50-62 ms a frame, and a steady 20 fps needs 50.`;
  }

  function drawRecomp(status, port) {
    countTo($('rc-fn'), status.summary.by_functions.total, (v) => d3.format(',')(Math.round(v)));
    $('rc-mb').innerHTML = ''; $('rc-mb').append((status.summary.by_code_size.total / 1048576).toFixed(1)); const s = document.createElement('small'); s.textContent = 'MB'; $('rc-mb').append(s);
    for (const map of ['renderer', 'hardware']) {
      const rows = port.filter((r) => r.map === map);
      const done = rows.filter((r) => r.status === 'native').length, part = rows.filter((r) => r.status === 'partial').length;
      const el = $(map === 'renderer' ? 'rc-r' : 'rc-h'); el.textContent = ''; el.append(String(done)); const sm = document.createElement('small'); sm.textContent = '/ ' + rows.length; el.append(sm);
      const groups = [...d3.group(rows, (r) => r.area)].map(([name, items]) => ({ name, items: items.map((r) => ({ ...r, value: 1 })) }));
      treemap($('map-' + map), groups, {
        label: map + ' features', ratio: 0.62,
        fmt: (v) => v + (v === 1 ? ' feature' : ' features'),
        color: (r) => r.status === 'native' ? 'var(--native)' : r.status === 'partial' ? 'var(--runtime)' : 'var(--mips)',
        ink: (r) => r.status === 'native' ? 'var(--native-ink)' : 'var(--mips-ink)',
        leafLabel: (r) => r.feature,
        hint: 'Click an area to list its features.',
        drill: (area) => rows.filter((r) => r.area === area).map((r) => ({ ...r, value: 1 })),
        summary: (area) => { const a = rows.filter((r) => r.area === area); return a.filter((r) => r.status === 'native').length + ' of ' + a.length + ' native (' + pct(100 * a.filter((r) => r.status === 'native').length / a.length, 0) + ')'; },
        itemTop: (r) => r.feature,
        itemName: (r) => (r.status === 'native' ? 'native' : r.status) + (r.since ? ' since ' + r.since.slice(5) : ''),
        itemTip: (r) => [[null, r.feature], ['t', r.area + ' · ' + (r.status === 'native' ? 'Native' : r.status)], ['', r.note || ''], ['t', 'Working since ' + (r.since || '?') + (r.commit ? ' · ' + r.commit : '')]],
        tip: (d) => [[null, d.data.feature], ['t', d.data.area + ' · ' + (d.data.status === 'native' ? 'Native' : d.data.status)], ['', d.data.note || ''], ['t', 'Working since ' + (d.data.since || '?') + (d.data.commit ? ' · ' + d.data.commit : '')]],
      });
      if (done + part < rows.length) $('rc-left').querySelector('b').textContent = (rows.length - done) + ' features still to port.';
    }
  }

  function drawDecomp(tree, status, hist) {
    const sum = (k, f) => d3.sum(tree.rows.filter((r) => r.kind === k), (r) => r[f]);
    countTo($('dc-time'), sum('native', 'time'), (v) => pct(v));
    countTo($('dc-mips'), sum('recompiled', 'time'), (v) => pct(v));
    countTo($('dc-calls'), status.summary.by_calls.percent, (v) => pct(v));
    countTo($('dc-code'), status.summary.by_code_size.percent, (v) => pct(v));
    $('dc-fns').textContent = d3.format(',')(status.summary.by_functions.native) + ' of ' + d3.format(',')(status.summary.by_functions.total);
    const byArea = d3.group(tree.rows, (r) => r.area);
    const groups = (field) => [...byArea].map(([name, items]) => ({ name, items: items.map((r) => ({ ...r, value: r[field] })) }));
    const common = {
      color: (r) => kindVar(r.kind), ink: (r) => inkVar(r.kind),
      fmt: (v, t) => (100 * v / t).toFixed(1) + '%',
    };
    const fnList = (area, key) => ((tree.functions || {})[area] || {})[key];
    const shortFn = (n) => n.split(' @ ')[0].replace(/^(decomp_\w+|sotc_\w+)::/, '');
    const areaSummary = (area) => { const a = (status.areas || {})[area]; const tn = d3.sum(tree.rows.filter((r) => r.area === area && r.kind === 'native'), (r) => r.time), tr = d3.sum(tree.rows.filter((r) => r.area === area && r.kind === 'recompiled'), (r) => r.time);
      return (a ? pct(a.percent) + ' of the code native (' + a.native_functions + ' of ' + d3.format(',')(a.functions) + ' functions) · ' : '') + 'game thread time ' + pct(tn) + ' native, ' + pct(tr) + ' MIPS'; };
    const kindShort = { native: 'Native', recompiled: 'MIPS', runtime: 'Runtime' };
    treemap($('tree-code'), groups('bytes'), { ...common, label: 'Code size by area',
      hint: 'Click an area to open its functions, sized by their MIPS code.',
      leafLabel: (r, v, pv) => kindShort[r.kind] + ' ' + (100 * v / pv).toFixed(1) + '%',
      leafSub: (r) => d3.format(',')(r.functions) + ' functions',
      drill: (area) => { const l = fnList(area, 'code'); return l && l.length ? l.map((f) => ({ ...f, value: f.bytes })) : null; },
      summary: areaSummary,
      itemName: (f) => shortFn(f.name),
      itemTip: (f, v, t) => [[null, f.name.split(' @ ')[0]], ['t', f.more ? 'Smaller functions, together' : (f.name.split(' @ ')[1] ? 'PS2 address ' + f.name.split(' @ ')[1] : '')], ['', KIND[f.kind] + ' · ' + (f.bytes / 1024).toFixed(1) + ' KB of MIPS'], ['t', (100 * v / t).toFixed(1) + '% of this area']],
      tip: (d, t) => [[null, areaName(d.data.area)], ['', KIND[d.data.kind] + ': ' + (d.data.bytes / 1024).toFixed(0) + ' KB, ' + (100 * d.value / t).toFixed(1) + '% of all code'], ['t', d3.format(',')(d.data.functions) + ' functions'], ['t', 'Game thread time: ' + pct(d.data.time)]] });
    treemap($('tree-time'), groups('time'), { ...common, label: 'Game thread time by area',
      hint: 'Click an area to see which functions take its time.',
      leafLabel: (r, v, pv) => kindShort[r.kind] + ' ' + (100 * v / pv).toFixed(1) + '%',
      leafSub: (r, v) => v.toFixed(1) + '% of the frame',
      drill: (area) => { const l = fnList(area, 'time'); return l && l.length ? l.map((f) => ({ ...f, value: f.time })) : null; },
      summary: areaSummary,
      itemName: (f) => shortFn(f.name),
      itemTip: (f, v, t) => [[null, f.name.split(' @ ')[0]], ['t', f.more ? 'Smaller functions, together' : (f.name.split(' @ ')[1] ? 'PS2 address ' + f.name.split(' @ ')[1] : 'Host code')], ['', KIND[f.kind] + ' · ' + pct(f.time, 2) + ' of the game thread'], ['t', (100 * v / t).toFixed(1) + '% of this area']],
      tip: (d) => [[null, areaName(d.data.area)], ['', KIND[d.data.kind] + ': ' + pct(d.data.time) + ' of the game thread'], ['t', d.data.functions ? d3.format(',')(d.data.functions) + ' functions, ' + (d.data.bytes / 1024).toFixed(0) + ' KB of MIPS' : 'Host code around the game']] });

    // area table
    const tb = $('area-table').querySelector('tbody'); tb.textContent = '';
    const areas = [...byArea].map(([name, items]) => {
      const g = (k, f) => d3.sum(items.filter((r) => r.kind === k), (r) => r[f]);
      return { name, nb: g('native', 'bytes'), rb: g('recompiled', 'bytes'), nf: g('native', 'functions'), rf: g('recompiled', 'functions'), nt: g('native', 'time'), rt: g('recompiled', 'time'), ut: g('runtime', 'time') };
    }).sort((a, b) => (a.name === RUNTIME) - (b.name === RUNTIME) || (b.nt + b.rt) - (a.nt + a.rt));
    for (const a of areas) {
      const tr = document.createElement('tr');
      const td = (text, cls) => { const c = document.createElement('td'); if (cls) c.className = cls; if (text instanceof Node) c.appendChild(text); else c.textContent = text; tr.appendChild(c); return c; };
      td(areaName(a.name));
      if (a.name === RUNTIME) { td(''); td(''); td(''); td(pct(a.ut) + ' runtime', 'r'); tb.appendChild(tr); continue; }
      const size = a.nb + a.rb, share = size ? 100 * a.nb / size : 0;
      const m = document.createElement('span'); const meter = document.createElement('span'); meter.className = 'meter'; const fill = document.createElement('i'); fill.style.width = share + '%'; fill.style.background = 'var(--native)'; meter.appendChild(fill); m.appendChild(meter); m.append(pct(share));
      td(m, 'r'); td(a.nf + ' / ' + d3.format(',')(a.nf + a.rf), 'r'); td(pct(a.nt), 'r'); td(pct(a.rt), 'r');
      tb.appendChild(tr);
    }
    labelCells($('area-table'));

    // top functions
    const fill = (id, kind, withArea) => {
      const body = $(id).querySelector('tbody'); body.textContent = '';
      const rows = tree.top.filter((t) => t.kind === kind).slice(0, kind === 'recompiled' ? 20 : 10);
      for (const t of rows) {
        const tr = document.createElement('tr');
        const c1 = document.createElement('td'); const parts = t.name.split(' @ ');
        const nm = document.createElement('span'); nm.appendChild(breakable(parts[0])); c1.appendChild(nm);
        if (parts[1]) { const ad = document.createElement('span'); ad.className = 'mono small'; ad.textContent = '  ' + parts[1]; c1.appendChild(ad); }
        tr.appendChild(c1);
        if (withArea) { const c2 = document.createElement('td'); c2.className = 'small'; c2.textContent = areaName(t.area); tr.appendChild(c2); }
        const c3 = document.createElement('td'); c3.className = 'r'; c3.textContent = pct(t.time, 2); tr.appendChild(c3);
        body.appendChild(tr);
      }
      labelCells($(id));
    };
    fill('top-mips', 'recompiled', true); fill('top-native', 'native', true); fill('top-runtime', 'runtime', false);

    drawHistory(hist.entries);
  }

  function drawHistory(entries) {
    const el = $('hist-chart'); el.textContent = '';
    const w = Math.max(280, el.clientWidth), h = 240, m = { t: 14, r: 14, b: 30, l: 40 };
    const x = d3.scalePoint(entries.map((_, i) => i), [m.l, w - m.r]).padding(0.3);
    const y = d3.scaleLinear([0, 100], [h - m.b, m.t]);
    const svg = d3.select(el).append('svg').attr('viewBox', `0 0 ${w} ${h}`).attr('role', 'img').attr('aria-label', 'Decomp progress over time');
    svg.append('g').selectAll('line').data([0, 25, 50, 75, 100]).join('line').attr('x1', m.l).attr('x2', w - m.r).attr('y1', y).attr('y2', y).attr('stroke', 'var(--line)');
    svg.append('g').selectAll('text').data([0, 25, 50, 75, 100]).join('text').attr('x', m.l - 6).attr('y', (v) => y(v) + 4).attr('text-anchor', 'end').text((v) => v + '%');
    const step = Math.max(1, Math.ceil(entries.length / Math.max(2, Math.floor(w / 80))));
    svg.append('g').selectAll('text').data(entries.map((e, i) => ({ e, i })).filter(({ i }) => i === entries.length - 1 || (i % step === 0 && entries.length - 1 - i >= step / 2)))
      .join('text').attr('x', ({ i }) => x(i)).attr('y', h - 10).attr('text-anchor', 'middle').text(({ e }) => e.date.slice(5));
    const series = [['time', 'var(--native)', 'Game thread time native'], ['calls', 'var(--accent)', 'Calls native'], ['code', 'var(--runtime)', 'Code native (by size)']];
    for (const [k, color] of series) {
      const pts = entries.map((e, i) => ({ i, v: e[k] })).filter((p) => p.v !== null && p.v !== undefined);
      const path = svg.append('path').attr('class', 'line').attr('d', d3.line().x((p) => x(p.i)).y((p) => y(p.v))(pts)).attr('fill', 'none').attr('stroke', color).attr('stroke-width', 2).attr('stroke-linejoin', 'round');
      if (motion && !el._drawn) { const len = path.node().getTotalLength(); path.attr('stroke-dasharray', len + ' ' + len).attr('stroke-dashoffset', len); onView(el, () => path.transition().duration(1400).ease(d3.easeCubicInOut).attr('stroke-dashoffset', 0)); }
      svg.append('g').selectAll('circle').data(pts).join('circle').attr('cx', (p) => x(p.i)).attr('cy', (p) => y(p.v)).attr('r', 3.5).attr('fill', color).attr('stroke', 'var(--panel)').attr('stroke-width', 1.5);
    }
    el._drawn = true;
    const cross = svg.append('line').attr('y1', m.t).attr('y2', h - m.b).attr('stroke', 'var(--ink-3)').attr('stroke-dasharray', '3 3').attr('visibility', 'hidden');
    const tip = tipFor(el);
    const scrub = (ev) => {
        const [mx] = d3.pointer(ev);
        let best = 0; entries.forEach((_, i) => { if (Math.abs(x(i) - mx) < Math.abs(x(best) - mx)) best = i; });
        const e = entries[best];
        cross.attr('x1', x(best)).attr('x2', x(best)).attr('visibility', 'visible');
        const lines = [[null, (e.label && !/^[0-9a-f]{7}$/.test(e.label) ? 'Round ' + e.label.replace(/^r/, '') + ' · ' : '') + e.date], ['t', 'commit ' + e.commit]];
        for (const [k, , name] of series) if (e[k] !== null && e[k] !== undefined) lines.push(['', name + ': ' + pct(e[k])]);
        setTip(tip, lines); placeTip(el, tip, ev);
    };
    // Mouse: follow the pointer. Touch: tap, or drag sideways (the page still scrolls vertically).
    svg.append('rect').attr('x', m.l).attr('y', m.t).attr('width', w - m.l - m.r).attr('height', h - m.t - m.b).attr('fill', 'transparent')
      .on('pointerdown', scrub).on('pointermove', scrub)
      .on('pointerleave pointercancel', (ev) => { if (ev.type === 'pointerleave' && ev.pointerType && ev.pointerType !== 'mouse') return; cross.attr('visibility', 'hidden'); tip.hidden = true; });
    if (!el._tipOff) { el._tipOff = true; document.addEventListener('pointerdown', (ev) => { if (!el.contains(ev.target)) { el.querySelectorAll('.tip').forEach((t) => { t.hidden = true; }); el.querySelectorAll('svg > line[stroke-dasharray]').forEach((l) => l.setAttribute('visibility', 'hidden')); } }, { passive: true }); }
  }

  const ROUNDS_FIRST = 12; let roundsAll = false;
  function drawRounds(rounds) {
    const list = $('round-list'); list.textContent = '';
    const all = [...rounds.rows].reverse(), more = $('rounds-more');
    const shown = roundsAll ? all : all.slice(0, ROUNDS_FIRST);
    if (more) { more.hidden = roundsAll || all.length <= ROUNDS_FIRST; more.textContent = 'Show all ' + all.length + ' rounds'; }
    for (const r of shown) {
      const d = document.createElement('div'); d.className = 'round' + (r.commit ? '' : ' now');
      const h = document.createElement('div'); h.className = 'h';
      const b = document.createElement('b'); b.textContent = 'Round ' + r.round.replace(/^r/, ''); h.appendChild(b);
      const chip = document.createElement('span'); chip.className = 'chip'; chip.style.background = 'var(--bg)'; chip.style.border = '1px solid var(--line)'; chip.textContent = r.focus; h.appendChild(chip);
      const meta = document.createElement('span'); meta.className = 'small'; meta.textContent = r.date + (r.commit ? ' · ' + r.commit : ' · running'); h.appendChild(meta);
      d.appendChild(h);
      const p = document.createElement('p'); p.textContent = r.summary; d.appendChild(p);
      if (r.result) { const res = document.createElement('span'); res.className = 'res'; res.textContent = r.result; d.appendChild(res); }
      list.appendChild(d);
    }
  }

  // Lightbox: any banner or gallery image opens full size in a dialog;
  // arrows, swipes and Esc work, the page stays where it was.
  (function lightbox() {
    const dlg = $('lightbox'); if (!dlg || !dlg.showModal) return;
    const imgs = [...document.querySelectorAll('.banner img, .gallery img')];
    const big = $('lb-img'), cap = $('lb-cap'), count = $('lb-count');
    let at = 0, lastFocus = null;
    const largest = (im) => { const set = (im.getAttribute('srcset') || '').split(',').map((x) => x.trim().split(/\s+/)); return set.length && set[set.length - 1][0] ? set[set.length - 1][0] : im.src; };
    function show(i) {
      at = (i + imgs.length) % imgs.length;
      const im = imgs[at];
      big.src = largest(im); big.alt = im.alt;
      const fc = im.closest('figure') && im.closest('figure').querySelector('figcaption');
      cap.textContent = fc ? fc.textContent : im.alt;
      count.textContent = (at + 1) + ' / ' + imgs.length;
    }
    function open(i) { lastFocus = document.activeElement; show(i); dlg.showModal(); document.documentElement.classList.add('lb-open'); }
    function close() { dlg.close(); }
    dlg.addEventListener('close', () => { document.documentElement.classList.remove('lb-open'); if (lastFocus && lastFocus.focus) lastFocus.focus(); });
    imgs.forEach((im, i) => {
      im.tabIndex = 0; im.setAttribute('role', 'button'); im.setAttribute('aria-label', 'Open image: ' + im.alt); im.classList.add('zoomable');
      im.addEventListener('click', () => open(i));
      im.addEventListener('keydown', (e) => { if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); open(i); } });
    });
    $('lb-prev').addEventListener('click', () => show(at - 1));
    $('lb-next').addEventListener('click', () => show(at + 1));
    $('lb-close').addEventListener('click', close);
    dlg.addEventListener('click', (e) => { if (e.target === dlg) close(); });
    dlg.addEventListener('keydown', (e) => { if (e.key === 'ArrowLeft') show(at - 1); else if (e.key === 'ArrowRight') show(at + 1); });
    // Swipe sideways for the next or previous image, down to close.
    let x0 = null, y0 = 0;
    dlg.addEventListener('touchstart', (e) => { if (e.touches.length !== 1) { x0 = null; return; } x0 = e.touches[0].clientX; y0 = e.touches[0].clientY; }, { passive: true });
    dlg.addEventListener('touchend', (e) => {
      if (x0 === null) return; const dx = e.changedTouches[0].clientX - x0, dy = e.changedTouches[0].clientY - y0; x0 = null;
      if (Math.abs(dx) > 50 && Math.abs(dx) > Math.abs(dy)) show(at + (dx < 0 ? 1 : -1));
      else if (dy > 80 && dy > Math.abs(dx) * 1.5) close();
    }, { passive: true });
  })();

  let data = null;
  // Browsers without scroll anchoring (iOS Safari) would jump when the charts
  // and tables above the screen fill in: note a landmark's place, draw, then
  // scroll back by what it moved. Chrome anchors by itself.
  const anchorsItself = !!(window.CSS && CSS.supports && CSS.supports('overflow-anchor', 'auto'));
  function steady(draw) {
    const vh = window.innerHeight;
    const marks = !anchorsItself && window.scrollY > 0 ? [...document.querySelectorAll('main h2, main h3, .banner, .scene, .ledge, .fig, .round, .gallery, footer')] : [];
    const mark = marks.find((m) => { const r = m.getBoundingClientRect(); return r.top >= 0 && r.top < vh; }) || marks.filter((m) => m.getBoundingClientRect().top < 0).pop();
    const top0 = mark ? mark.getBoundingClientRect().top : 0;
    draw();
    if (mark) { const dy = mark.getBoundingClientRect().top - top0; if (Math.abs(dy) > 0.5) window.scrollBy({ top: dy, behavior: 'instant' }); }
  }
  // Drawn in a few steps with a frame between them, so no single task blocks
  // a phone's main thread for long.
  const nextFrame = () => new Promise((r) => requestAnimationFrame(() => setTimeout(r, 0)));
  async function render() {
    if (!data) return;
    const [tree, status, hist, port, rounds] = data;
    steady(() => drawLede(tree)); await nextFrame();
    steady(() => drawRecomp(status, port)); await nextFrame();
    steady(() => drawDecomp(tree, status, hist)); await nextFrame();
    steady(() => { drawRounds(rounds); document.querySelectorAll('.table-box').forEach((b) => { b.style.minHeight = ''; }); });
    if (!document.body.classList.contains('ready')) { document.body.classList.add('ready'); reveal(document.querySelectorAll('.round'), true); }
    $('foot-updated').textContent = 'Last measurement: ' + tree.updated + ' (' + tree.label + ').';
  }
  // The charts' library and data (~0.6 MB before compression) load once the
  // reader starts down the page or a data section comes within a screen, so
  // the hero and the letter load alone. Until then the maps and tables hold
  // their space.
  function reserve() {
    for (const [id, ratio] of [['map-renderer', 0.62], ['map-hardware', 0.62], ['tree-code', 0.7], ['tree-time', 0.7]]) {
      const el = $(id); if (el && !el.firstChild) el.style.minHeight = mapHeight(Math.max(280, el.clientWidth), ratio) + 'px';
    }
    const narrow = window.matchMedia && window.matchMedia('(max-width: 40rem)').matches;
    document.querySelectorAll('table[data-rows]').forEach((t) => {
      const tb = t.querySelector('tbody'); if (tb.firstChild) return;
      const n = +t.dataset.rows, rowH = narrow ? ({ 'area-table': 135, 'top-mips': 118, 'top-native': 96 })[t.id] || 90 : 37;
      t.parentElement.style.minHeight = Math.round(n * rowH * 0.95) + 'px';
    });
  }
  reserve();
  let loading = null;
  function loadData() {
    if (loading) return loading;
    const lib = window.d3 ? Promise.resolve() : new Promise((ok, fail) => { const sc = document.createElement('script'); sc.src = 'vendor/d3-7.9.0.min.js'; sc.onload = ok; sc.onerror = () => fail(new Error('vendor/d3-7.9.0.min.js did not load')); document.head.appendChild(sc); });
    loading = Promise.all([lib, ...['decomp_tree.json', 'decomp_status.json', 'decomp_history.json', 'port_status.json', 'rounds.json'].map(load)])
      .then(([, ...d]) => { data = d; return render(); })
      .catch((err) => { $('lede-note').textContent = 'The data files did not load (' + err.message + '). They live next to this page in docs/.'; });
    return loading;
  }
  const dataSecs = ['.lede', '#recomp', '#decomp', '#targets', '#rounds', 'footer'].map((q) => document.querySelector(q)).filter(Boolean);
  if ('IntersectionObserver' in window) {
    const dio = new IntersectionObserver((es) => { if (es.some((e) => e.isIntersecting)) { dio.disconnect(); loadData(); } }, { rootMargin: '0px 0px 100% 0px' });
    dataSecs.forEach((s) => dio.observe(s));
    const started = () => { if (window.scrollY > 40) { window.removeEventListener('scroll', started); loadData(); } };
    window.addEventListener('scroll', started, { passive: true });
  } else loadData();
  let t = null, lastW = window.innerWidth;
  // Mobile browsers fire resize when the URL bar hides; only redraw on width.
  window.addEventListener('resize', () => { if (window.innerWidth === lastW) return; lastW = window.innerWidth; clearTimeout(t); t = setTimeout(() => { if (data) render(); else reserve(); }, 150); });
  const moreBtn = $('rounds-more');
  if (moreBtn) moreBtn.addEventListener('click', () => { roundsAll = true; if (data) drawRounds(data[4]); });

  // ---------------- Atmosphere ----------------
  // Scroll reveals: elements below the fold start hidden (.rv) and settle in
  // (.in) when they come into view; [data-stagger] children follow one another.
  function reveal(nodes, stagger) {
    if (!motion || !('IntersectionObserver' in window)) return;
    const io = new IntersectionObserver((es) => es.forEach((e) => { if (e.isIntersecting) { e.target.classList.add('in'); io.unobserve(e.target); } }), { threshold: 0.12, rootMargin: '0px 0px -6% 0px' });
    let i = 0;
    Array.from(nodes).forEach((n) => {
      if (n.getBoundingClientRect().top < window.innerHeight * 0.95) return;
      n.classList.add('rv'); if (stagger) { n.classList.add('st'); n.style.setProperty('--d', (Math.min(i++, 6) * 0.09) + 's'); }
      io.observe(n);
    });
  }
  reveal(document.querySelectorAll('[data-reveal]'), false);
  document.querySelectorAll('[data-stagger]').forEach((g) => reveal(g.children, true));

  // Scroll-linked: progress line, sticky bar, hero and banner parallax.
  const hero = document.querySelector('.hero'), heroImg = $('hero-img'), heroIn = $('hero-in'), bar = $('bar'), prog = document.querySelector('#progress i');
  const frames = [...document.querySelectorAll('.banner .frame')];
  const links = [...document.querySelectorAll('.bar-links a')], secs = links.map((a) => document.querySelector(a.getAttribute('href')));
  const barLinks = document.querySelector('.bar-links');
  let ticking = false, hh = hero ? hero.offsetHeight : 0, lastCur = -2;
  window.addEventListener('resize', () => { hh = hero ? hero.offsetHeight : 0; }, { passive: true });
  function onScroll() {
    ticking = false;
    // Reads first (layout), then writes (transform/opacity only).
    const y = window.scrollY, vh = window.innerHeight, H = document.documentElement.scrollHeight - vh;
    let cur = -1; secs.forEach((s, i) => { if (s && s.getBoundingClientRect().top < vh * 0.35) cur = i; });
    const fr = motion ? frames.map((f) => f.getBoundingClientRect()) : [];
    if (prog) prog.style.transform = 'scaleX(' + (H > 0 ? Math.min(1, y / H) : 0) + ')';
    if (bar) bar.classList.toggle('on', y > hh * 0.7);
    if (cur !== lastCur) {
      lastCur = cur;
      links.forEach((a, i) => a.classList.toggle('cur', i === cur));
      // On a phone the links scroll sideways: bring the current one into view.
      if (cur >= 0 && barLinks && barLinks.scrollWidth > barLinks.clientWidth + 1) {
        const left = links[cur].offsetLeft - barLinks.offsetLeft - 12;
        barLinks.scrollTo({ left: Math.max(0, left), behavior: motion ? 'smooth' : 'auto' });
      }
    }
    if (!motion) return;
    if (y < hh) {
      if (heroImg) heroImg.style.transform = 'translate3d(0,' + (y * 0.35).toFixed(1) + 'px,0) scale(' + (1.02 + y / hh * 0.06).toFixed(4) + ')';
      if (heroIn) { heroIn.style.transform = 'translate3d(0,' + (y * 0.18).toFixed(1) + 'px,0)'; heroIn.style.opacity = String(Math.max(0, 1 - y / (hh * 0.75)).toFixed(3)); }
    }
    frames.forEach((f, i) => {
      const r = fr[i];
      if (r.bottom < 0 || r.top > vh) return;
      const k = (r.top + r.height / 2 - vh / 2) / (vh / 2 + r.height / 2); // -1..1 across the screen
      const img = f.firstElementChild; if (img) img.style.transform = 'translate3d(0,' + (k * -9).toFixed(2) + '%,0)';
    });
  }
  // The hero's loops (rays, mist, colossus, rider) pause once it is off screen.
  if (hero && 'IntersectionObserver' in window) new IntersectionObserver((es) => { hero.classList.toggle('off', !es[0].isIntersecting); }).observe(hero);
  window.addEventListener('scroll', () => { if (!ticking) { ticking = true; requestAnimationFrame(onScroll); } }, { passive: true });
  onScroll();

  // Dust in the light: a few dozen motes drifting through the hero, drawn
  // only while the hero is on screen and the tab is visible.
  (function dust() {
    const cv = $('dust'); if (!cv || !motion || !cv.getContext) return;
    const ctx = cv.getContext('2d'); let w = 0, h = 0, dpr = 1, motes = [], on = true, raf = 0, last = 0;
    function size() {
      w = cv.clientWidth; h = cv.clientHeight; dpr = Math.min(w < 700 ? 1.5 : 2, window.devicePixelRatio || 1); cv.width = Math.round(w * dpr); cv.height = Math.round(h * dpr); ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      const n = Math.round(Math.min(70, Math.max(24, w * h / 22000)));
      motes = Array.from({ length: n }, () => ({ x: Math.random() * w, y: Math.random() * h, r: 0.4 + Math.random() * 1.8, vx: 4 + Math.random() * 10, vy: -2 - Math.random() * 6, p: Math.random() * 6.28, s: 0.4 + Math.random() * 1.2 }));
    }
    function frame(t) {
      raf = 0; if (!on) return;
      const dt = Math.min(0.05, last ? (t - last) / 1000 : 0.016); last = t;
      ctx.clearRect(0, 0, w, h);
      for (const m of motes) {
        m.p += dt * m.s; m.x += (m.vx + Math.sin(m.p) * 6) * dt; m.y += (m.vy + Math.cos(m.p * 0.7) * 3) * dt;
        if (m.x > w + 10) m.x = -10; if (m.y < -10) { m.y = h + 10; m.x = Math.random() * w; }
        const light = Math.max(0, Math.min(1, m.x / w * 1.4)); // brighter where the light falls (right)
        const a = (0.12 + 0.45 * light) * (0.6 + 0.4 * Math.sin(m.p * 2.3));
        ctx.beginPath(); ctx.arc(m.x, m.y, m.r, 0, 6.2832); ctx.fillStyle = 'rgba(255,238,200,' + a.toFixed(3) + ')'; ctx.fill();
      }
      raf = requestAnimationFrame(frame);
    }
    const go = () => { if (on && !raf && !document.hidden) { last = 0; raf = requestAnimationFrame(frame); } };
    size(); go();
    let rw = 0; window.addEventListener('resize', () => { if (cv.clientWidth === w && Math.abs(cv.clientHeight - h) < 120) return; clearTimeout(rw); rw = setTimeout(size, 150); }, { passive: true });
    document.addEventListener('visibilitychange', go);
    if ('IntersectionObserver' in window) new IntersectionObserver((es) => { on = es[0].isIntersecting; go(); }).observe(cv);
  })();

  // Scenes (site.css, SCENES): the illustrations' parts (.sa) carry 1s
  // keyframes. Where CSS scroll-driven animations exist a view() timeline
  // plays them; elsewhere this sets each paused animation's time from the
  // scene's progress through the screen (0 as its top enters at the bottom, 1
  // as its bottom leaves at the top: view()'s own range). Loops run only while
  // a scene is on screen (.live); legs only while the page moves (.go). Agro
  // follows the page's progress along the bottom edge and faces the way you
  // scroll.
  (function scenes() {
    if (!motion) return;
    const list = [...document.querySelectorAll('[data-scene]')];
    const sda = !!(window.CSS && CSS.supports && CSS.supports('animation-timeline', 'view()'));
    const vis = new Set(), cache = new Map();
    const anims = (s) => {
      let a = cache.get(s);
      if (!a || !a.length) { a = (s.getAnimations ? s.getAnimations({ subtree: true }) : []).filter((x) => x.animationName && x.animationName.indexOf('s-') === 0); cache.set(s, a); }
      return a;
    };
    const agro = $('agro'), dir = agro ? agro.firstElementChild : null;
    const gaits = [agro, ...document.querySelectorAll('.ledge')].filter(Boolean);
    let lastY = window.scrollY, face = 1, going = false, still = 0, raf = 0, agroW = agro ? agro.offsetWidth : 0;
    function frame() {
      raf = 0;
      const y = window.scrollY, vh = window.innerHeight;
      if (!sda && vis.size) {
        const ps = [...vis].map((s) => { const r = s.getBoundingClientRect(); return [s, Math.max(0, Math.min(1, (vh - r.top) / (vh + r.height)))]; });
        ps.forEach(([s, p]) => anims(s).forEach((a) => { a.currentTime = p * 1000; }));
      }
      if (agro) {
        const H = document.documentElement.scrollHeight - vh, hh = hero ? hero.offsetHeight : 0;
        agro.classList.toggle('on', y > hh * 0.75);
        const k = H > 0 ? Math.min(1, Math.max(0, y / H)) : 0;
        agro.style.transform = 'translate3d(' + (k * (window.innerWidth - agroW)).toFixed(1) + 'px,0,0)';
        if (y !== lastY) { const f = y > lastY ? 1 : -1; if (f !== face) { face = f; dir.style.transform = f < 0 ? 'scaleX(-1)' : ''; } }
      }
      if (y !== lastY) {
        if (!going) { going = true; gaits.forEach((g) => g.classList.add('go')); }
        clearTimeout(still);
        still = setTimeout(() => { going = false; gaits.forEach((g) => g.classList.remove('go')); }, 160);
      }
      lastY = y;
    }
    const kick = () => { if (!raf) raf = requestAnimationFrame(frame); };
    if ('IntersectionObserver' in window) {
      const io = new IntersectionObserver((es) => {
        es.forEach((e) => { e.target.classList.toggle('live', e.isIntersecting); if (e.isIntersecting) vis.add(e.target); else vis.delete(e.target); });
        kick();
      }, { rootMargin: '15% 0px' });
      list.forEach((s) => io.observe(s));
    } else list.forEach((s) => { s.classList.add('live'); vis.add(s); });
    window.addEventListener('scroll', kick, { passive: true });
    window.addEventListener('resize', () => { agroW = agro ? agro.offsetWidth : 0; cache.clear(); kick(); });
    kick();
  })();
})();
