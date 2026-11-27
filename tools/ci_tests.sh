#!/bin/bash
# The ps2x_tests suites the CI runs (.github/workflows/tests.yml), also by hand:
#
#   tools/ci_tests.sh [build_dir]            configure if needed, build ps2x_tests, run the gate
#   tools/ci_tests.sh [build_dir] --report   only the known-failing suites, reported (never fails)
#   CI_CMAKE_ARGS="..."                       extra CMake configure arguments
#
# The gate: the suites that pass on the Mac (and on Linux) today; any failure
# fails the run. The rest already fail or crash on the Mac (CodeGenerator, PS2GS
# and others; PS2Memory ends the process), so they are only reported, until
# they are fixed and move to the gate. A suite this branch does not have
# (ps2x_tests --list) is skipped, so one list serves every branch.
set -uo pipefail
cd "$(dirname "$0")/.."

BUILD=${1:-build/tests}
REPORT=0
[ "${2:-}" = "--report" ] && REPORT=1

GATE="ElfAnalyzerHeuristics PadInput PS2IopSubsystem PS2RuntimeIO PS2VU0Math R5900Decoder TexturePack Widescreen"
KNOWN_FAILING="CodeGenerator PS2GS PS2Memory PS2Recompiler PS2RuntimeExpansion PS2RuntimeInterrupt PS2RuntimeKernel PS2SifDma PS2SifRpc PS2VU1"

if [ ! -f "$BUILD/build.ninja" ]; then
    # No FFmpeg: the tests decode no video, and it is one dependency less to install.
    # shellcheck disable=SC2086
    cmake -S PS2Recomp -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release \
        -DPS2X_BUILD_TEST=ON -DPS2X_BUILD_ANALYZER=ON -DPS2X_BUILD_STUDIO=OFF \
        -DPS2X_ENABLE_FFMPEG=OFF ${CI_CMAKE_ARGS:-} || exit 1
fi
cmake --build "$BUILD" --target ps2x_tests || exit 1
TESTS=$(cd "$BUILD/ps2xTest" && pwd)/ps2x_tests
SUITES=" $("$TESTS" --list | tr '\n' ' ') "

run() { # suite -> "passed failed" (or "crash")
    local out
    out=$(cd PS2Recomp/ps2xTest && "$TESTS" "$1" 2>&1 | sed 's/\x1b\[[0-9;]*m//g')
    local p f
    p=$(echo "$out" | awk '/^Passed:/ {print $2}')
    f=$(echo "$out" | awk '/^Failed:/ {print $2}')
    if [ -z "$p" ] || [ -z "$f" ]; then
        echo "crash"
        echo "$out" | tail -20 >&2
    else
        echo "$p $f"
        [ "$f" != 0 ] && echo "$out" | grep -E "Failed\]|^      - " >&2
    fi
}

status=0
if [ "$REPORT" = 0 ]; then
    echo "== gate"
    for s in $GATE; do
        case "$SUITES" in *" $s "*) ;; *) echo "  $s: not on this branch, skipped"; continue ;; esac
        r=$(run "$s")
        if [ "$r" = "crash" ] || [ "${r#* }" != 0 ]; then
            echo "  $s: FAILED ($r)"
            status=1
        else
            echo "  $s: ${r% *} passed"
        fi
    done
else
    echo "== known failing (report only)"
    for s in $KNOWN_FAILING; do
        case "$SUITES" in *" $s "*) ;; *) continue ;; esac
        r=$(run "$s" 2>/dev/null)
        if [ "$r" = "crash" ]; then echo "  $s: crashed"; else echo "  $s: ${r% *} passed, ${r#* } failed"; fi
    done
fi
exit $status
