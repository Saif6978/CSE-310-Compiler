#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Building Syntax and Semantic Analyzer ==="
make all

echo ""
echo "=== Running Offline 3 Verification Suite ==="
make test
