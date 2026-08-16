#!/bin/sh
set -eu

if [ "$#" -eq 0 ]; then
  echo "[container] Building project (Release)..."
  cmake --build build -- -j"$(nproc)"
  echo "[container] Running tests..."
  ctest --test-dir build --output-on-failure
else
  exec "$@"
fi
