#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"

./run-script.sh

echo
# compare.sh intentionally exits 1 when differences exist. Preserve that
# exit code so this script can also be used in automated testing.
./compare.sh
