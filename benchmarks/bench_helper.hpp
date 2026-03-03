#pragma once
#include <algorithm>
#include <array>
#include <benchmark/benchmark.h>
#include <vector>

constexpr auto compute_min = [](const std::vector<double>& values) -> double { return *std::min_element(values.begin(), values.end()); };
constexpr auto compute_max = [](const std::vector<double>& values) -> double { return *std::max_element(values.begin(), values.end()); };

// Gives ~50/50 true/false split on random data
constexpr uint8_t BRANCH_THRESHOLD = 127;

constexpr std::array<int64_t, 6> BENCHMARK_SIZES = {
  1L << 5,  // 32 B
  1L << 10, // 1 KiB
  1L << 15, // 32 KiB
  1L << 20, // 1 MiB
  1L << 25, // 32 MiB
  1L << 30, // 1 GiB
};

// Common benchmark configuration: buffer sizes and statistics
inline void
apply_sizes(::benchmark::Benchmark* bench)
{
  for (auto size : BENCHMARK_SIZES) {
    bench->Arg(size);
  }

  bench->ComputeStatistics("min", compute_min)->ComputeStatistics("max", compute_max);
}
