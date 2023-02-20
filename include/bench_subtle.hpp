#pragma once
#include "subtle.hpp"
#include <benchmark/benchmark.h>
#include <random>

// Benchmark constant-time comparison and selection operations, targeting CPU
namespace bench_subtle {

// Benchmark constant-time equality check operation
template<typename operandT>
void
ct_eq(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = subtle::ct_eq<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native equality check operator
template<typename operandT>
void
native_eq(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = -static_cast<uint32_t>(x == y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time inequality check operation
template<typename operandT>
void
ct_ne(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = subtle::ct_ne<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native inequality check operator
template<typename operandT>
void
native_ne(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = -static_cast<uint32_t>(x != y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time lesser than equality check operation
template<typename operandT>
void
ct_le(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = subtle::ct_le<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native lesser than equality check operator
template<typename operandT>
void
native_le(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    const uint32_t z = -static_cast<uint32_t>(x <= y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();

    x += (z >> 31);
    y -= (z >> 31);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

}
