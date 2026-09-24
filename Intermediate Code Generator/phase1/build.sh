#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building Phase 1 (Intermediate Code Generator) ==="
make all

echo ""
echo "=== Running Phase 1 Test Suite ==="
make test
