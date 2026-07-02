#pragma once
#include "subtle.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <limits>
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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

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

// Test functional correctness of constant-time memory comparison operation,
// checking result against element-wise equality via std::equal.
template<typename operandT, typename returnT>
void
test_ct_memcmp()
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  constexpr returnT truthv = static_cast<returnT>(~returnT{ 0 });
  constexpr returnT falsev = 0;

  constexpr size_t MIN_SIZE = 0;
  constexpr size_t MAX_SIZE = 1024;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());
  std::uniform_int_distribution<size_t> size_dis(MIN_SIZE, MAX_SIZE);

  for (size_t i = 0; i < ITERATIONS; i++) {
    const size_t len = size_dis(gen);

    std::vector<operandT> lhs(len);
    std::vector<operandT> rhs(len);

    std::ranges::generate(lhs, [&]() { return dis(gen); });
    std::ranges::generate(rhs, [&]() { return dis(gen); });

    const returnT z = subtle::ct_memcmp<operandT, returnT>(std::span<const operandT>(lhs), std::span<const operandT>(rhs));
    const bool expected = std::equal(lhs.begin(), lhs.end(), rhs.begin());
    ASSERT_EQ(z, (expected ? truthv : falsev));

    // Identical spans must return truth value
    const returnT self = subtle::ct_memcmp<operandT, returnT>(std::span<const operandT>(lhs), std::span<const operandT>(lhs));
    ASSERT_EQ(self, truthv);
  }
}

// Test functional correctness of constant-time conditional memcpy operation,
// verifying that dst is overwritten with src on truth value and left unchanged
// on false value.
template<typename operandT, typename branchT>
void
test_ct_conditional_memcpy()
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<branchT>)
{
  constexpr branchT truthv = static_cast<branchT>(~branchT{ 0 });
  constexpr branchT falsev = 0;

  constexpr size_t MIN_SIZE = 0;
  constexpr size_t MAX_SIZE = 1024;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());
  std::uniform_int_distribution<size_t> size_dis(MIN_SIZE, MAX_SIZE);

  for (size_t i = 0; i < ITERATIONS; i++) {
    const size_t len = size_dis(gen);

    std::vector<operandT> src(len);
    std::vector<operandT> dst(len);

    std::ranges::generate(src, [&]() { return dis(gen); });
    std::ranges::generate(dst, [&]() { return dis(gen); });

    // Truth value: dst must become an exact copy of src.
    std::vector<operandT> dst_truth = dst;
    subtle::ct_conditional_memcpy<branchT, operandT>(truthv, std::span<operandT>(dst_truth), std::span<const operandT>(src));
    ASSERT_EQ(dst_truth, src);

    // False value: dst must retain its original contents.
    std::vector<operandT> dst_false = dst;
    subtle::ct_conditional_memcpy<branchT, operandT>(falsev, std::span<operandT>(dst_false), std::span<const operandT>(src));
    ASSERT_EQ(dst_false, dst);
  }
}

// Test functional correctness of constant-time table lookup operation, checking
// the scanned result against a plain table[idx] access.
template<typename operandT, typename indexT>
void
test_ct_lookup()
  requires(subtle::ct_operand<operandT> && std::is_unsigned_v<indexT>)
{
  constexpr size_t MIN_SIZE = 1;
  constexpr size_t MAX_SIZE = 256; // fits the narrowest ( uint8_t ) index range

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());
  std::uniform_int_distribution<size_t> size_dis(MIN_SIZE, MAX_SIZE);

  for (size_t i = 0; i < ITERATIONS; i++) {
    const size_t len = size_dis(gen);

    std::vector<operandT> table(len);
    std::ranges::generate(table, [&]() { return dis(gen); });

    std::uniform_int_distribution<size_t> idx_dis(0, len - 1);
    const size_t idx = idx_dis(gen);

    const operandT z = subtle::ct_lookup<indexT, operandT>(static_cast<indexT>(idx), std::span<const operandT>(table));
    ASSERT_EQ(z, table[idx]);
  }
}

}
