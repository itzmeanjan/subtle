#include "bench_helper.hpp"
#include "subtle.hpp"
#include <cstring>
#include <random>
#include <span>

namespace {

// Constant-time conditional copy: if br is all-ones, copies src into dst
void
ct_byte_copy(uint32_t br, std::span<const uint8_t> src, std::span<uint8_t> dst)
{
  for (size_t i = 0; i < src.size(); i++) {
    dst[i] = subtle::ct_select<uint32_t, uint8_t>(br, src[i], dst[i]);
  }
}

// Constant-time conditional copy using subtle::ct_select
void
ct_conditional_copy(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> src(len);
  std::vector<uint8_t> dst(len);

  std::ranges::generate(src, [&] { return dis(gen); });
  std::ranges::generate(dst, [&] { return dis(gen); });

  size_t idx = 0;
  for (auto _iter : state) {
    uint32_t br = subtle::ct_gt<uint8_t, uint32_t>(src[idx % len], BRANCH_THRESHOLD);
    idx++;

    ct_byte_copy(br, src, dst);

    benchmark::DoNotOptimize(dst.data());
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

// Native conditional copy using branched std::memcpy
void
native_conditional_copy(benchmark::State& state)
{
  const auto len = static_cast<size_t>(state.range(0));

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  std::vector<uint8_t> src(len);
  std::vector<uint8_t> dst(len);

  std::ranges::generate(src, [&] { return dis(gen); });
  std::ranges::generate(dst, [&] { return dis(gen); });

  size_t idx = 0;
  for (auto _iter : state) {
    if (src[idx % len] > BRANCH_THRESHOLD) {
      std::memcpy(dst.data(), src.data(), len);
    }
    idx++;

    benchmark::DoNotOptimize(dst.data());
    benchmark::ClobberMemory();
  }

  state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(len));
}

}

BENCHMARK(ct_conditional_copy)->Name("ct_conditional_copy")->Apply(apply_sizes);         // NOLINT(cert-err58-cpp)
BENCHMARK(native_conditional_copy)->Name("native_conditional_copy")->Apply(apply_sizes); // NOLINT(cert-err58-cpp)
