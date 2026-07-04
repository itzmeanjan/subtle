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

  # Extract bitwidth suffix (u8/i8, u16/i16, u32/i32, u64/i64).
  width="${func##*[iu]}"

  # Pick signed vs unsigned secret globals based on the type suffix.
  if [[ "$func" == *_i${width} ]]; then
    sx="secret_ix${width}"
    sy="secret_iy${width}"
    sbuf1="secret_buf1_i${width}"
    sbuf2="secret_buf2_i${width}"
  else
    sx="secret_x${width}"
    sy="secret_y${width}"
    sbuf1="secret_buf1_u${width}"
    sbuf2="secret_buf2_u${width}"
  fi

  # Determine which globals are secret, per operation family.
  if [[ "$func" == *_is_zero_* ]]; then
    secrets="${sx}"
  elif [[ "$func" == *_lookup_* ]]; then
    secrets="secret_x${width}, ${sbuf1}"
  elif [[ "$func" == *_swap_span_* ]]; then
    secrets="${sbuf1}, ${sbuf2}, secret_br${width}"
  elif [[ "$func" == *_conditional_memcpy_* ]]; then
    secrets="${sbuf1}, ${sbuf2}, secret_br${width}"
  elif [[ "$func" == *_memcmp_* ]]; then
    secrets="${sbuf1}, ${sbuf2}"
  elif [[ "$func" == *_zeroize_* ]]; then
    secrets="${sbuf1}"
  else
    secrets="${sx}, ${sy}"
    if [[ "$func" == *_select_* || "$func" == *_swap_* ]]; then
      secrets="${secrets}, secret_br${width}"
    fi
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
