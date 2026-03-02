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
    uint32_t z = subtle::ct_eq<operandT, uint32_t>(x, y);

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
    uint32_t z = -static_cast<uint32_t>(x == y);

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
    uint32_t z = subtle::ct_ne<operandT, uint32_t>(x, y);

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
    uint32_t z = -static_cast<uint32_t>(x != y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time conditional selection
template<typename operandT>
void
ct_select(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  uint32_t br = -static_cast<uint32_t>(dis(gen) & 1);
  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    operandT z = subtle::ct_select(br, x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(br);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark non-constant-time conditional selection using ternary operator
template<typename operandT>
void
native_select(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  uint32_t br = -static_cast<uint32_t>(dis(gen) & 1);
  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    operandT z = br ? x : y;

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(br);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time conditional swap
template<typename operandT>
void
ct_swap(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  uint32_t br = -static_cast<uint32_t>(dis(gen) & 1);
  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    subtle::ct_swap(br, x, y);

    benchmark::DoNotOptimize(br);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark non-constant-time conditional swap
template<typename operandT>
void
native_swap(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  uint32_t br = -static_cast<uint32_t>(dis(gen) & 1);
  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    if (br) {
      std::swap(x, y);
    }

    benchmark::DoNotOptimize(br);
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
    uint32_t z = subtle::ct_le<operandT, uint32_t>(x, y);

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
    uint32_t z = -static_cast<uint32_t>(x <= y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time greater than check operation
template<typename operandT>
void
ct_gt(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = subtle::ct_gt<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native greater than check operator
template<typename operandT>
void
native_gt(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = -static_cast<uint32_t>(x > y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time greater than equality check operation
template<typename operandT>
void
ct_ge(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = subtle::ct_ge<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native greater than equality check operator
template<typename operandT>
void
native_ge(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = -static_cast<uint32_t>(x >= y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark constant-time lesser than check operation
template<typename operandT>
void
ct_lt(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = subtle::ct_lt<operandT, uint32_t>(x, y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

// Benchmark native lesser than check operator
template<typename operandT>
void
native_lt(benchmark::State& state)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  operandT x = dis(gen);
  operandT y = dis(gen);

  for (auto _ : state) {
    uint32_t z = -static_cast<uint32_t>(x < y);

    benchmark::DoNotOptimize(z);
    benchmark::DoNotOptimize(x);
    benchmark::DoNotOptimize(y);
    benchmark::ClobberMemory();
  }
}

}
