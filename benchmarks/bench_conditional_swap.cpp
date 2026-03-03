#include "bench_helper.hpp"
#include "subtle.hpp"
#include <random>
#include <span>

namespace {

// Constant-time conditional swap: if br is all-ones, swaps a and b
void
ct_byte_swap(uint32_t br, std::span<uint8_t> a, std::span<uint8_t> b)
{
  for (size_t i = 0; i < a.size(); i++) {
    subtle::ct_swap<uint32_t, uint8_t>(br, a[i], b[i]);
  }
}

// Constant-time conditional swap of two byte arrays using subtle::ct_swap
void
ct_conditional_swap(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> a(len);
  std::vector<uint8_t> b(len);

  std::ranges::generate(a, [&] { return dis(gen); });
  std::ranges::generate(b, [&] { return dis(gen); });

  size_t idx = 0;
  for (auto _iter : state) {
    uint32_t br = subtle::ct_gt<uint8_t, uint32_t>(a[idx % len], BRANCH_THRESHOLD);
    idx++;

    ct_byte_swap(br, a, b);

    benchmark::DoNotOptimize(a.data());
    benchmark::DoNotOptimize(b.data());
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

// Native conditional swap using branched std::swap_ranges
void
native_conditional_swap(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> a(len);
  std::vector<uint8_t> b(len);

  std::ranges::generate(a, [&] { return dis(gen); });
  std::ranges::generate(b, [&] { return dis(gen); });

  size_t idx = 0;
  for (auto _iter : state) {
    if (a[idx % len] > BRANCH_THRESHOLD) {
      std::swap_ranges(a.begin(), a.end(), b.begin());
    }
    idx++;

    benchmark::DoNotOptimize(a.data());
    benchmark::DoNotOptimize(b.data());
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

}

BENCHMARK(ct_conditional_swap)->Name("ct_conditional_swap")->Apply(apply_sizes);         // NOLINT(cert-err58-cpp)
BENCHMARK(native_conditional_swap)->Name("native_conditional_swap")->Apply(apply_sizes); // NOLINT(cert-err58-cpp)
