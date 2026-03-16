#pragma once
#include "subtle.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <vector>

// Test cases for ensuring functional correctness of constant-time comparison
// and selection operations
namespace test_subtle {

constexpr size_t ITERATIONS = 1UL << 16;

// Test functional correctness of constant-time equality operation over unsigned
// integer types, checking result against native comparison operator ( i.e. == )
template<typename operandT, typename returnT>
void
test_ct_eq()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_eq<operandT, returnT>(x, y);

    ASSERT_EQ(z, (x == y ? truthv : falsev));
    ASSERT_EQ(truthv, (subtle::ct_eq<operandT, returnT>(x, x)));
    ASSERT_EQ(truthv, (subtle::ct_eq<operandT, returnT>(y, y)));
  }
}

// Test functional correctness of constant-time inequality operation over
// unsigned integer types, checking result against native comparison operator
template<typename operandT, typename returnT>
void
test_ct_ne()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_ne<operandT, returnT>(x, y);
    ASSERT_EQ(z, (x != y ? truthv : falsev));
  }
}

// Test functional correctness of constant-time conditional selection operation
// over unsigned integer types.
template<typename operandT, typename returnT>
void
test_ct_select()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    ASSERT_EQ(x, (subtle::ct_select(truthv, x, y)));
    ASSERT_EQ(y, (subtle::ct_select(falsev, x, y)));
  }
}

// Test that constant-time swap with truth value actually swaps.
template<typename operandT, typename returnT>
void
test_ct_swap_truth()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    const auto tmpx = x;
    const auto tmpy = y;

    subtle::ct_swap(truthv, x, y);

    ASSERT_EQ(tmpx, y);
    ASSERT_EQ(tmpy, x);
  }
}

// Test that two consecutive truth-swaps round-trip back to original values.
template<typename operandT, typename returnT>
void
test_ct_swap_roundtrip()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    const auto tmpx = x;
    const auto tmpy = y;

    subtle::ct_swap(truthv, x, y);
    subtle::ct_swap(truthv, x, y);

    ASSERT_EQ(tmpx, x);
    ASSERT_EQ(tmpy, y);
  }
}

// Test that constant-time swap with false value is a no-op.
template<typename operandT, typename returnT>
void
test_ct_swap_false()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    const auto tmpx = x;
    const auto tmpy = y;

    subtle::ct_swap(falsev, x, y); // x, y = x, y (no-op)

    ASSERT_EQ(tmpx, x);
    ASSERT_EQ(tmpy, y);
  }
}

// Test functional correctness of constant-time lesser than equality operation
// over unsigned integer types, checking result against native comparison op.
template<typename operandT, typename returnT>
void
test_ct_le()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_le<operandT, returnT>(x, y);

    ASSERT_EQ(z, (x <= y ? truthv : falsev));
    ASSERT_EQ(truthv, (subtle::ct_le<operandT, returnT>(x, x)));
    ASSERT_EQ(truthv, (subtle::ct_le<operandT, returnT>(y, y)));
  }
}

// Test functional correctness of constant-time greater than operation over
// unsigned integer types, checking result against native comparison operator.
template<typename operandT, typename returnT>
void
test_ct_gt()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_gt<operandT, returnT>(x, y);
    ASSERT_EQ(z, (x > y ? truthv : falsev));
  }
}

// Test functional correctness of constant-time greater than equality operation
// over unsigned integer types, checking result against native comparison op.
template<typename operandT, typename returnT>
void
test_ct_ge()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_ge<operandT, returnT>(x, y);

    ASSERT_EQ(z, (x >= y ? truthv : falsev));
    ASSERT_EQ(truthv, (subtle::ct_ge<operandT, returnT>(x, x)));
    ASSERT_EQ(truthv, (subtle::ct_ge<operandT, returnT>(y, y)));
  }
}

// Test functional correctness of constant-time lesser than operation over
// unsigned integer types, checking result against native comparison operation.
template<typename operandT, typename returnT>
void
test_ct_lt()
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    const operandT x = dis(gen);
    const operandT y = dis(gen);

    const returnT z = subtle::ct_lt<operandT, returnT>(x, y);
    ASSERT_EQ(z, (x < y ? truthv : falsev));
  }
}

// Test functional correctness of constant-time zeroize operation,
// verifying all elements of a span are zeroed after the operation.
template<typename T>
void
test_ct_zeroize()
{
  constexpr size_t MIN_SIZE = 0;
  constexpr size_t MAX_SIZE = 1024;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<size_t> size_dis(MIN_SIZE, MAX_SIZE);

  for (size_t i = 0; i < ITERATIONS; i++) {
    const size_t len = size_dis(gen);
    std::vector<T> buf(len);

    std::ranges::generate(buf, [&]() -> T { return static_cast<T>(gen()); });
    subtle::ct_zeroize(std::span<T>(buf));

    for (const auto& elem : buf) {
      ASSERT_EQ(elem, T{ 0 });
    }
  }
}

}
