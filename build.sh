#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building All CSE 310 Compiler Projects ==="
make all

echo ""
echo "=== Running Complete Verification Suite ==="
make test
