#pragma once
#include "forceinline.hpp"
#include <algorithm>
#include <cstddef>
#include <span>
#include <type_traits>

// MSan detection: the "+r" asm constraint reads the value, which triggers
// false positives on poisoned (secret) inputs. MSan operates at LLVM IR
// level (before backend optimizations), so the barrier is unnecessary.
#ifdef __has_feature
#if __has_feature(memory_sanitizer)
#define SUBTLE_MSAN_ACTIVE_
#endif
#endif

// Constant-time comparison and selection of unsigned integer values.
namespace subtle {

// Optimization barrier: prevents the compiler from reasoning about `val`,
// blocking pattern recognition that would restructure constant-time code into branches
// or secret-dependent memory accesses. Generates zero machine instructions.
template<typename T>
forceinline constexpr void
ct_barrier([[maybe_unused]] T& val)
  requires(std::is_unsigned_v<T>)
{
#if !defined(SUBTLE_MSAN_ACTIVE_)
  if (!std::is_constant_evaluated()) {
#if defined(_MSC_VER)
    volatile T* vp = &val;
    val = *vp;
#else
    asm volatile("" : "+r"(val)); // NOLINT(hicpp-no-assembler)
#endif
  }
#endif
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns true ( if x == y ) or false ( in case x != y )
// testing equality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_eq(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  operandT a = x ^ y;
  ct_barrier(a);

  const operandT b = static_cast<operandT>(a | static_cast<operandT>(-a));
  const operandT c = b >> ((sizeof(operandT) * 8) - 1); // select only MSB
  const returnT d = static_cast<returnT>(c);
  const returnT e = static_cast<returnT>(d - static_cast<returnT>(1));

  return e;
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns true ( if x != y ) or false ( in case x == y )
// testing inequality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_ne(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = ct_eq<operandT, returnT>(x, y);
  return static_cast<returnT>(~z); // bit-inverted result of equality check
}

// Given a branch value br ( of type branchT ) holding either truth or false
// value and two unsigned integers x, y ( of bitwidth 8, 16, 32 or 64 ), this
// routine selects x if br is truth value or it returns y.
//
// Branch value br can have either of two values
//
// - truth value is represented using all bits of type branchT set to 1
// - false value is represented using all bits of type branchT set to 0
//
// If br takes any other value, this is an undefined behaviour !
template<typename branchT, typename operandT>
forceinline constexpr operandT
ct_select(const branchT br, const operandT x, const operandT y)
  requires(std::is_unsigned_v<branchT> && std::is_unsigned_v<operandT>)
{
  const branchT z = br >> ((sizeof(branchT) * 8) - 1); // select MSB
  operandT w = -static_cast<operandT>(z);              // bw(br) = bw(x) = bw(y)
  ct_barrier(w);
  const operandT selected = static_cast<operandT>((x & w) | static_cast<operandT>(y & static_cast<operandT>(~w))); // br ? x : y

  return selected;
}

// Given a branch value br ( of type branchT ) holding either truth value (
// represented using value of type branchT s.t. all the bits are set to 1 ) or
// false value ( represented using value of type branchT s.t. all the bits are
// set to 0 ), this function swaps values held by x, y if br is truth value.
// Otherwise both x, y keep their values.
//
// branchT and operandT can be unsigned integers of bit width 8, 16, 32 or 64.
template<typename branchT, typename operandT>
forceinline constexpr void
ct_swap(const branchT br, operandT& x, operandT& y)
  requires(std::is_unsigned_v<branchT> && std::is_unsigned_v<operandT>)
{
  operandT mask = static_cast<operandT>(-static_cast<operandT>(br & 1));
  ct_barrier(mask);

  x = static_cast<operandT>(x ^ static_cast<operandT>(mask & y));
  y = static_cast<operandT>(y ^ static_cast<operandT>(mask & x));
  x = static_cast<operandT>(x ^ static_cast<operandT>(mask & y));
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns truth value ( if x <= y ) or false value ( in case
// x > y ) based on result of lesser than equal comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
//
// This implementation collects a lot of inspiration from
// https://github.com/dalek-cryptography/subtle/blob/bd282be01f1c2da8ab03922e03457102a76a0e05/src/lib.rs#L808-L841
//
// I also found
// https://cs.opensource.google/go/go/+/refs/tags/go1.20.1:src/crypto/subtle/constant_time.go;l=56-62
// to be pretty helpful.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_le(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const operandT gt_bits = static_cast<operandT>(x & static_cast<operandT>(~y));
  operandT lt_bits = static_cast<operandT>(static_cast<operandT>(~x) & y);

  for (size_t pow = 1; pow < sizeof(operandT) * 8;) {
    lt_bits = static_cast<operandT>(lt_bits | static_cast<operandT>(lt_bits >> pow));
    pow += pow;
  }

  operandT bit = static_cast<operandT>(gt_bits & static_cast<operandT>(~lt_bits));

  for (size_t pow = 1; pow < sizeof(operandT) * 8;) {
    bit = static_cast<operandT>(bit | static_cast<operandT>(bit >> pow));
    pow += pow;
  }

  ct_barrier(bit);
  const returnT z = static_cast<returnT>(-static_cast<returnT>((bit & 1) ^ 1));
  return z;
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns truth value ( if x > y ) or false value ( in case
// x <= y ) based on result of greater than comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_gt(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = ct_le<operandT, returnT>(x, y);
  return static_cast<returnT>(~z); // bit-inverted result of <= test
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns truth value ( if x >= y ) or false value ( in case
// x < y ) based on result of greater than equal comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_ge(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z0 = ct_gt<operandT, returnT>(x, y);
  const returnT z1 = ct_eq<operandT, returnT>(x, y);
  return static_cast<returnT>(z0 | z1); // (x > y) | (x == y)
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns truth value ( if x < y ) or false value ( in case
// x >= y ) based on result of lesser than comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_lt(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = ct_ge<operandT, returnT>(x, y);
  return static_cast<returnT>(~z); // bit-inverted result of >= test
}

// Securely zeroizes a std::span, preventing the compiler from optimizing away the operation.
// At compile-time: a plain fill suffices -- there is no real memory to protect.
// At runtime: the fill is followed by a compiler barrier (empty asm with "memory" clobber)
// that forces the compiler to treat the array's memory as externally observable, preventing
// dead store elimination. The barrier generates zero instructions.
template<typename T, size_t N>
forceinline constexpr void
ct_zeroize(std::span<T, N> vals)
{
  std::ranges::fill(vals, T{});
  if (!std::is_constant_evaluated()) {
    asm volatile("" : : "r"(vals.data()) : "memory"); // NOLINT(hicpp-no-assembler)
  }
}

}
