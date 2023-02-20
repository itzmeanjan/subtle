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

BENCHMARK_MAIN();
