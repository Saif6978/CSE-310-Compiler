#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building Phase 2 (Full Intermediate Code Generator) ==="
make all

echo ""
echo "=== Running Phase 2 Test Suite ==="
make test
