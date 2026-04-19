#!/bin/bash
set -euo pipefail

# Build script for EdgeComputing project
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

if command -v make >/dev/null 2>&1; then
    make
else
    echo "make not found, compiling directly with gcc..."
    mkdir -p output
    gcc -Wall -Wextra -Iinclude src/main.c src/logger.c -o output/edge_app
fi

echo "Build complete: $(pwd)/output/edge_app"
