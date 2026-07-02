#!/usr/bin/env bash
# Run Binsec CT verification inside the binsec Docker image.
#
# Tests both GCC and Clang in Release, RelWithDebInfo, and MinSizeRel.
# Debug is excluded because -O0 doesn't inline or optimize bitwise patterns
# into branchless code, so binsec correctly reports branches that only
# exist at -O0.
#
# Usage: ./tests/scripts/run_docker_binsec.sh

set -euo pipefail

IMAGE="binsec/binsec:latest"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"

COMPILERS=("g++" "clang++")
BUILD_TYPES=("Release" "RelWithDebInfo" "MinSizeRel")

OVERALL_PASS=true

for compiler in "${COMPILERS[@]}"; do
  for build_type in "${BUILD_TYPES[@]}"; do
    build_dir="build_binsec_${compiler//+/p}_${build_type,,}"
    echo "=============================================="
    echo "  ${compiler} / ${build_type}"
    echo "=============================================="

    docker run --rm --user root \
      -v "${PROJECT_DIR}:/work" -w /work \
      "$IMAGE" bash -c "
        apt-get update -qq && apt-get install -y -qq cmake g++ clang make > /dev/null 2>&1
        rm -rf /tmp/${build_dir}
        cmake -B /tmp/${build_dir} \
          -DCMAKE_CXX_COMPILER=${compiler} \
          -DCMAKE_BUILD_TYPE=${build_type} \
          -DSUBTLE_BUILD_TESTS=ON \
          -DSUBTLE_BINSEC=ON 2>&1
        cmake --build /tmp/${build_dir} -j 2>&1
        /work/tests/scripts/run_binsec.sh binsec /tmp/${build_dir}/subtle_ct_binsec
      " && status=0 || status=$?

    if [ "$status" -ne 0 ]; then
      OVERALL_PASS=false
      echo "  RESULT: FAILED (exit $status)"
    else
      echo "  RESULT: PASSED"
    fi
    echo ""
  done
done

if [ "$OVERALL_PASS" = true ]; then
  echo "All Binsec verification jobs passed."
  exit 0
else
  echo "Some Binsec verification jobs failed."
  exit 1
fi
