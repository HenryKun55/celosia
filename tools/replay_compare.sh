#!/bin/bash
# Replays an input recording (SOTC_INPUT_RECORD) on the Mac's GS path, the
# Mac's native renderer and in Vita3K, and makes the compare page: one row
# per MARK with the three pictures of the same frames side by side, and the
# determinism check (where Wander, Agro and the camera are in each replay
# against the recording).
#
# Usage: tools/replay_compare.sh <recording> [name]
#   <recording>  input_<date>.txt (its .mcd and .settings.cfg beside it), e.g.
#                ../vita3k-celosia-play/fs/ux0/data/celosia/rec/input_20261103_200000.txt
#   name         the output folder's name (default: the recording's)
# Output: build/agent-scratch/replay/<name>/index.html (gs/, native/, vita/
# hold each run's log and pictures).
# Env: RUNNER     the Mac runner (default build/cmake/ps2xRuntime/ps2EntryRunner)
#      GS_CPU=1   the GS picture rasterized on the CPU (PS2X_GS_GPU=0: exact GS
#                 fog, slower)
#      V3         the Vita3K tree (default ../vita3k-celosia-rec; its memcard,
#                 settings.cfg and env.txt are put back afterwards)
#      VITA3K     the Vita3K binary (default build/agent-scratch/vita3k-rec, whose
#                 portable folder is ../vita3k-celosia-rec; a copy with a
#                 portable folder must point at V3, checked)
#      VPK        install this VPK in the tree first (else the installed app)
#      MHZ        Vita3K's CPU clock (default: Vita3K's own)
#      SKIP       any of "gs native vita"
#      SHOTS      tick offsets pictured around each mark (default -6,-3,0,3,6)
#      SECS       the longest any one replay may take (default 3600)
cd "$(dirname "$0")/.."
ROOT=$PWD
REC=$(cd "$(dirname "${1:?usage: tools/replay_compare.sh <recording> [name]}")" && pwd)/$(basename "$1")
[ -f "$REC" ] || { echo "no recording $REC" >&2; exit 1; }
NAME=${2:-$(basename "$REC" .txt)}
OUT=$ROOT/build/agent-scratch/replay/$NAME
RUNNER=${RUNNER:-$ROOT/build/cmake/ps2xRuntime/ps2EntryRunner}
SHOTS=${SHOTS:--6,-3,0,3,6}
SECS=${SECS:-3600}
mkdir -p "$OUT/input"
cp "$REC" "$OUT/input/"
for x in mcd settings.cfg; do [ -f "$REC.$x" ] && cp "$REC.$x" "$OUT/input/"; done
IN=$OUT/input/$(basename "$REC")
skip() { case " $SKIP " in *" $1 "*) return 0;; esac; return 1; }

# A Mac replay: own folder as the working directory (its settings file), the
# recording's memory card copy.
mac() {
    local label=$1; shift
    local d=$OUT/$label
    rm -rf "$d" && mkdir -p "$d/mc" "$d/shots"
    [ -f "$IN.mcd" ] && cp "$IN.mcd" "$d/mc/Mcd001.ps2"
    [ -f "$IN.settings.cfg" ] && cp "$IN.settings.cfg" "$d/celosia_settings.cfg"
    (cd "$d" && env SOTC_INPUT_REPLAY="$IN" SOTC_REPLAY_EXIT=1 SOTC_REPLAY_SHOTS="$SHOTS" PS2X_MEMCARD_DIR="$d/mc" PS2X_NO_SPU2_AUDIO=1 \
        PS2X_SNAPSHOT_DIR="$d/shots" PS2X_SNAPSHOT_SECONDS=1000000 "$@" \
        perl -e "alarm shift; exec @ARGV" "$SECS" "$RUNNER" "$ROOT/disc/SCUS_974.72" > "$d/run.log" 2>&1)
    echo "$label: $(grep -ac '^\[replay\] shot' "$d/run.log") pictures, $(grep -a '^\[replay\] end' "$d/run.log" || echo 'did not reach the end')"
}

# The Vita3K replay: the game holds each pictured frame for SOTC_REPLAY_HOLD_MS
# and says so in err.txt ("[replay] hold <tick>"); its window is captured then.
vita() {
    local V3=${V3:-$(cd .. && pwd)/vita3k-celosia-rec}
    local FS=$V3/fs D=$V3/fs/ux0/data/celosia d=$OUT/vita
    local BIN=${VITA3K:-$ROOT/build/agent-scratch/vita3k-rec/Vita3K.app/Contents/MacOS/Vita3K}
    [ -f "$V3/config.yml" ] || { echo "vita: no Vita3K tree at $V3" >&2; return; }
    # A Vita3K copy with a "portable" folder next to its app uses that folder's
    # fs whatever -c and VITA3K_PREF_PATH say: it must be this tree.
    local portable
    portable=$(cd "$(dirname "$BIN")/../../.." 2>/dev/null && [ -e portable ] && cd portable && pwd -P)
    if [ -n "$portable" ] && [ "$portable" != "$(cd "$V3" && pwd -P)" ]; then
        echo "vita: $BIN runs in $portable, not $V3: set VITA3K to that tree's Vita3K (or V3)" >&2; return
    fi
    rm -rf "$d" && mkdir -p "$d" "$D/replay" "$D/memcards"
    if [ -n "$VPK" ]; then rm -rf "$FS/ux0/app/CELO00001" && mkdir -p "$FS/ux0/app/CELO00001" && unzip -q -o "$VPK" -d "$FS/ux0/app/CELO00001"; fi
    local keep=$d/kept; mkdir -p "$keep"
    for f in memcards/Mcd001.ps2 settings.cfg env.txt; do [ -f "$D/$f" ] && cp "$D/$f" "$keep/$(basename "$f")"; done
    cp "$IN" "$D/replay/input.txt"
    [ -f "$IN.mcd" ] && cp "$IN.mcd" "$D/memcards/Mcd001.ps2"
    [ -f "$IN.settings.cfg" ] && cp "$IN.settings.cfg" "$D/settings.cfg"
    printf '%s\n' PS2X_NO_SPU2_AUDIO=1 SOTC_INPUT_REPLAY=ux0:data/celosia/replay/input.txt "SOTC_REPLAY_SHOTS=$SHOTS" SOTC_REPLAY_HOLD_MS=1500 > "$D/env.txt"
    swiftc -O tools/winid.swift -o "$d/.winid" 2>/dev/null
    local pat="$V3/config.yml -r CELO00001" ok=0
    stop() { pkill -f "$pat"; sleep 2; pkill -9 -f "$pat"; }
    for try in 1 2 3; do
        rm -f "$D/err.txt" "$D/log.txt"
        VITA3K_CPU_MHZ="${MHZ:-}" VITA3K_CORES="${CORES:-}" VITA3K_PREF_PATH="$FS/" "$BIN" -c "$V3/config.yml" -r CELO00001 > "$d/vita3k-stdout.txt" 2>&1 &
        local pid=$! seen=0 start=$SECONDS booted=0
        while kill -0 $pid 2>/dev/null && [ $((SECONDS - start)) -lt "$SECS" ]; do
            sleep 0.1
            [ $booted = 0 ] && grep -aq '^\[replay\]' "$D/err.txt" 2>/dev/null && booted=1
            if [ $booted = 0 ] && [ $((SECONDS - start)) -gt "${BOOT_S:-90}" ]; then break; fi
            local holds
            holds=$(grep -a '^\[replay\] hold ' "$D/err.txt" 2>/dev/null | wc -l | tr -d ' ')
            while [ "$seen" -lt "$holds" ]; do
                seen=$((seen + 1))
                local t
                t=$(grep -a '^\[replay\] hold ' "$D/err.txt" | sed -n "${seen}p" | awk '{print $3}')
                sleep 0.4
                local w
                w=$("$d/.winid" "$pid")
                [ -n "$w" ] && [ "$w" != 0 ] && screencapture -x -o -l"$w" "$d/t_$t.png"
            done
            grep -aq '^\[replay\] end' "$D/err.txt" 2>/dev/null && { ok=1; break; }
        done
        stop
        [ $booted = 1 ] && break
        echo "vita: try $try did not boot, again" >&2
    done
    cp "$D/err.txt" "$d/err.txt" 2>/dev/null
    for f in Mcd001.ps2 settings.cfg env.txt; do
        case $f in Mcd001.ps2) dst=$D/memcards/$f;; *) dst=$D/$f;; esac
        if [ -f "$keep/$f" ]; then cp "$keep/$f" "$dst"; else rm -f "$dst"; fi
    done
    echo "vita: $(ls "$d" | grep -c '^t_.*png') pictures, $([ $ok = 1 ] && echo 'reached the end' || echo 'did not reach the end')"
}

pids=""
skip gs || { mac gs SOTC_NATIVE=2 ${GS_CPU:+PS2X_GS_GPU=0} & pids="$pids $!"; }
skip native || { mac native SOTC_NATIVE=1 & pids="$pids $!"; }
skip vita || vita
[ -n "$pids" ] && wait $pids
args=()
skip gs || args+=("Mac GS=$OUT/gs")
skip native || args+=("Mac native=$OUT/native")
skip vita || args+=("Vita3K=$OUT/vita")
python3 tools/replay_compare.py "$IN" "$OUT" "${args[@]}"
