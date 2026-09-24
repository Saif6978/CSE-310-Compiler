#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building All Phases of Intermediate Code Generator ==="
make all

echo ""
echo "=== Running Full Verification (Phase 1 & Phase 2) ==="
make test
