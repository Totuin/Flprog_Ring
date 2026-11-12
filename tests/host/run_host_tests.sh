#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEST="$ROOT/tests/host"
BIN="${TMPDIR:-/tmp}/flprog_ring_host_test_$$"
trap 'rm -f "$BIN"' EXIT
c++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -O1 -g \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    -I "$TEST/stubs" -I "$ROOT/src" -I "$ROOT/src/utilites" \
    "$ROOT/src/flprogRing.cpp" "$ROOT/src/utilites/flprogAbstractRingVariable.cpp" \
    "$TEST/test_ring.cpp" -o "$BIN"
ASAN_OPTIONS=detect_leaks=1 "$BIN"
