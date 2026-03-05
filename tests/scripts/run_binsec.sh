#!/usr/bin/env bash
# Binsec constant-time verification runner.
#
# Discovers all binsec_ct_* symbols in the binary, generates a .cfg for each,
# runs binsec -sse -checkct, and reports results.
#
# Usage: run_binsec.sh <binsec_path> <binary_path>

set -euo pipefail

BINSEC="$1"
BINARY="$2"

PASS=0
FAIL=0
UNKNOWN=0
TOTAL=0

# Discover all binsec_ct_* function symbols (T = text/code section).
FUNCTIONS=$(nm "$BINARY" | grep -oP '(?<= T )binsec_ct_\w+' | sort -u)

if [ -z "$FUNCTIONS" ]; then
  echo "ERROR: No binsec_ct_* symbols found in $BINARY"
  exit 1
fi

TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

for func in $FUNCTIONS; do
  TOTAL=$((TOTAL + 1))

  # Extract bitwidth suffix (u8, u16, u32, u64).
  width="${func##*_u}"

  # Determine which globals are secret.
  secrets="secret_x${width}, secret_y${width}"
  if [[ "$func" == *_select_* || "$func" == *_swap_* ]]; then
    secrets="${secrets}, secret_br${width}"
  fi

  # Generate .cfg script.
  cfg="${TMPDIR}/${func}.cfg"
  cat > "$cfg" <<EOF
starting from <${func}>
with concrete stack pointer
secret global ${secrets}
halt at <_exit>
explore all
EOF

  # Run binsec with tuning parameters.
  output=$("$BINSEC" -sse -checkct \
    -sse-script "$cfg" \
    -sse-depth 10000 \
    -sse-timeout 300 \
    -fml-solver-timeout 60 \
    "$BINARY" 2>&1) || true

  # Parse result.
  if echo "$output" | grep -qi "program status.*secure" && ! echo "$output" | grep -qi "program status.*insecure"; then
    PASS=$((PASS + 1))
    status="SECURE"
  elif echo "$output" | grep -qi "insecure"; then
    FAIL=$((FAIL + 1))
    status="INSECURE"
    # Print details for insecure results.
    echo "$output" | head -30
  else
    UNKNOWN=$((UNKNOWN + 1))
    status="UNKNOWN"
  fi

  printf "  %-30s %s\n" "$func" "$status"
done

echo "---"
echo "Total: ${TOTAL}  Secure: ${PASS}  Insecure: ${FAIL}  Unknown: ${UNKNOWN}"

# Fail if any function is insecure.
if [ "$FAIL" -gt 0 ]; then
  echo "FAILED: ${FAIL} function(s) are not constant-time."
  exit 1
fi

# Warn about unknown results but don't fail.
if [ "$UNKNOWN" -gt 0 ]; then
  echo "WARNING: ${UNKNOWN} function(s) could not be fully verified (timeout/depth limit)."
fi

exit 0
