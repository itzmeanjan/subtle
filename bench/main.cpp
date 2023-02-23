#include "bench_subtle.hpp"

BENCHMARK(bench_subtle::ct_eq<uint8_t>);
BENCHMARK(bench_subtle::native_eq<uint8_t>);
BENCHMARK(bench_subtle::ct_eq<uint16_t>);
BENCHMARK(bench_subtle::native_eq<uint16_t>);
BENCHMARK(bench_subtle::ct_eq<uint32_t>);
BENCHMARK(bench_subtle::native_eq<uint32_t>);
BENCHMARK(bench_subtle::ct_eq<uint64_t>);
BENCHMARK(bench_subtle::native_eq<uint64_t>);

BENCHMARK(bench_subtle::ct_ne<uint8_t>);
BENCHMARK(bench_subtle::native_ne<uint8_t>);
BENCHMARK(bench_subtle::ct_ne<uint16_t>);
BENCHMARK(bench_subtle::native_ne<uint16_t>);
BENCHMARK(bench_subtle::ct_ne<uint32_t>);
BENCHMARK(bench_subtle::native_ne<uint32_t>);
BENCHMARK(bench_subtle::ct_ne<uint64_t>);
BENCHMARK(bench_subtle::native_ne<uint64_t>);

BENCHMARK(bench_subtle::ct_select<uint8_t>);
BENCHMARK(bench_subtle::native_select<uint8_t>);
BENCHMARK(bench_subtle::ct_select<uint16_t>);
BENCHMARK(bench_subtle::native_select<uint16_t>);
BENCHMARK(bench_subtle::ct_select<uint32_t>);
BENCHMARK(bench_subtle::native_select<uint32_t>);
BENCHMARK(bench_subtle::ct_select<uint64_t>);
BENCHMARK(bench_subtle::native_select<uint64_t>);

BENCHMARK(bench_subtle::ct_swap<uint8_t>);
BENCHMARK(bench_subtle::native_swap<uint8_t>);
BENCHMARK(bench_subtle::ct_swap<uint16_t>);
BENCHMARK(bench_subtle::native_swap<uint16_t>);
BENCHMARK(bench_subtle::ct_swap<uint32_t>);
BENCHMARK(bench_subtle::native_swap<uint32_t>);
BENCHMARK(bench_subtle::ct_swap<uint64_t>);
BENCHMARK(bench_subtle::native_swap<uint64_t>);

BENCHMARK(bench_subtle::ct_le<uint8_t>);
BENCHMARK(bench_subtle::native_le<uint8_t>);
BENCHMARK(bench_subtle::ct_le<uint16_t>);
BENCHMARK(bench_subtle::native_le<uint16_t>);
BENCHMARK(bench_subtle::ct_le<uint32_t>);
BENCHMARK(bench_subtle::native_le<uint32_t>);
BENCHMARK(bench_subtle::ct_le<uint64_t>);
BENCHMARK(bench_subtle::native_le<uint64_t>);

BENCHMARK(bench_subtle::ct_gt<uint8_t>);
BENCHMARK(bench_subtle::native_gt<uint8_t>);
BENCHMARK(bench_subtle::ct_gt<uint16_t>);
BENCHMARK(bench_subtle::native_gt<uint16_t>);
BENCHMARK(bench_subtle::ct_gt<uint32_t>);
BENCHMARK(bench_subtle::native_gt<uint32_t>);
BENCHMARK(bench_subtle::ct_gt<uint64_t>);
BENCHMARK(bench_subtle::native_gt<uint64_t>);

BENCHMARK(bench_subtle::ct_ge<uint8_t>);
BENCHMARK(bench_subtle::native_ge<uint8_t>);
BENCHMARK(bench_subtle::ct_ge<uint16_t>);
BENCHMARK(bench_subtle::native_ge<uint16_t>);
BENCHMARK(bench_subtle::ct_ge<uint32_t>);
BENCHMARK(bench_subtle::native_ge<uint32_t>);
BENCHMARK(bench_subtle::ct_ge<uint64_t>);
BENCHMARK(bench_subtle::native_ge<uint64_t>);

BENCHMARK(bench_subtle::ct_lt<uint8_t>);
BENCHMARK(bench_subtle::native_lt<uint8_t>);
BENCHMARK(bench_subtle::ct_lt<uint16_t>);
BENCHMARK(bench_subtle::native_lt<uint16_t>);
BENCHMARK(bench_subtle::ct_lt<uint32_t>);
BENCHMARK(bench_subtle::native_lt<uint32_t>);
BENCHMARK(bench_subtle::ct_lt<uint64_t>);
BENCHMARK(bench_subtle::native_lt<uint64_t>);

BENCHMARK_MAIN();
