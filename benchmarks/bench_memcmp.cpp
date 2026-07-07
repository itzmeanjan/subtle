#include "bench_helper.hpp"
#include "subtle.hpp"
#include <cstring>
#include <random>
#include <span>

namespace {

// Constant-time byte-array equality comparison using subtle::ct_eq
void
ct_memcmp(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> a(len);
  std::ranges::generate(a, [&] { return dis(gen); });
  auto b = a; // equal arrays = worst case (must scan all bytes)

  for (auto _iter : state) {
    auto result = subtle::ct_memcmp<uint8_t, uint32_t>(std::span<const uint8_t>(a), std::span<const uint8_t>(b));

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

// Native byte-array equality comparison using std::memcmp
void
native_memcmp(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> a(len);
  std::ranges::generate(a, [&] { return dis(gen); });
  auto b = a;

  for (auto _iter : state) {
    int result = std::memcmp(a.data(), b.data(), len);

    benchmark::DoNotOptimize(result);
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

}

BENCHMARK(ct_memcmp)->Name("ct_memcmp")->Apply(apply_sizes);         // NOLINT(cert-err58-cpp)
BENCHMARK(native_memcmp)->Name("native_memcmp")->Apply(apply_sizes); // NOLINT(cert-err58-cpp)
