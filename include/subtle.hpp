#pragma once
#include "forceinline.hpp"
#include <algorithm>
#include <cstddef>
#include <limits>
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

// Constant-time operations on integer values.
namespace subtle {

// An operand type accepted by the constant-time routines below: any standard
// integer type -- signed or unsigned, of bit width 8, 16, 32 or 64 -- as well as
// the character types, but not bool.
template<typename T>
concept ct_operand = std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;

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

// Given two integers x, y of type operandT, this routine returns true ( if
// x == y ) or false ( in case x != y ) testing equality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_eq(const operandT x, const operandT y)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  using Uop = std::make_unsigned_t<operandT>;

  Uop a = static_cast<Uop>(static_cast<Uop>(x) ^ static_cast<Uop>(y));
  ct_barrier(a);

  const Uop b = static_cast<Uop>(a | static_cast<Uop>(-a));
  const Uop c = static_cast<Uop>(b >> (std::numeric_limits<Uop>::digits - 1)); // select only MSB
  const returnT d = static_cast<returnT>(c);
  const returnT e = static_cast<returnT>(d - static_cast<returnT>(1));

  return e;
}

// Given two integers x, y of type operandT, this routine returns true ( if
// x != y ) or false ( in case x == y ) testing inequality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_ne(const operandT x, const operandT y)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = ct_eq<operandT, returnT>(x, y);
  return static_cast<returnT>(~z); // bit-inverted result of equality check
}

// Given a branch value br ( of type branchT ) holding either truth or false
// value and two integers x, y of type operandT, this routine selects x if br is
// truth value or it returns y.
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
  requires(std::is_unsigned_v<branchT> && ct_operand<operandT>)
{
  using Uop = std::make_unsigned_t<operandT>;

  const branchT z = br >> (std::numeric_limits<branchT>::digits - 1); // select MSB
  Uop w = static_cast<Uop>(-static_cast<Uop>(z));
  ct_barrier(w);
  const Uop selected = static_cast<Uop>((static_cast<Uop>(x) & w) | static_cast<Uop>(static_cast<Uop>(y) & static_cast<Uop>(~w))); // br ? x : y

  return static_cast<operandT>(selected);
}

// Given a branch value br ( of type branchT ) holding either truth value (
// represented using value of type branchT s.t. all the bits are set to 1 ) or
// false value ( represented using value of type branchT s.t. all the bits are
// set to 0 ), this function swaps values held by x, y if br is truth value.
// Otherwise both x, y keep their values.
template<typename branchT, typename operandT>
forceinline constexpr void
ct_swap(const branchT br, operandT& x, operandT& y)
  requires(std::is_unsigned_v<branchT> && ct_operand<operandT>)
{
  using Uop = std::make_unsigned_t<operandT>;

  Uop mask = static_cast<Uop>(-static_cast<Uop>(br & 1));
  ct_barrier(mask);

  Uop xu = static_cast<Uop>(x);
  Uop yu = static_cast<Uop>(y);

  xu = static_cast<Uop>(xu ^ static_cast<Uop>(mask & yu));
  yu = static_cast<Uop>(yu ^ static_cast<Uop>(mask & xu));
  xu = static_cast<Uop>(xu ^ static_cast<Uop>(mask & yu));

  x = static_cast<operandT>(xu);
  y = static_cast<operandT>(yu);
}

// Given a branch value br ( of type branchT ) holding either truth value (
// represented using value of type branchT s.t. all the bits are set to 1 ) or
// false value ( represented using value of type branchT s.t. all the bits are
// set to 0 ) and two equal-length spans x, y of integers, this routine swaps the
// contents of x and y element-wise if br is truth value. Otherwise both spans
// keep their contents.
//
// If br takes any value other than these two, this is an undefined behaviour !
template<typename branchT, typename operandT, size_t N>
forceinline constexpr void
ct_swap(const branchT br, std::span<operandT, N> x, std::span<operandT, N> y)
  requires(std::is_unsigned_v<branchT> && ct_operand<operandT>)
{
  for (size_t i = 0; i < x.size(); i++) {
    ct_swap<branchT, operandT>(br, x[i], y[i]);
  }
}

// Given two integers x, y of type operandT, this routine returns truth value (
// if x <= y ) or false value ( in case x > y ) based on result of lesser than
// equal comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
//
// For signed operands the sign bit of both values is flipped up front: this maps
// the signed order monotonically onto the unsigned order, so the unsigned
// magnitude comparison below yields the correct signed result. The flip is a
// compile-time decision, so no data-dependent branch is introduced.
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
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  using Uop = std::make_unsigned_t<operandT>;
  constexpr Uop sign_bit = std::is_signed_v<operandT> ? static_cast<Uop>(Uop{ 1 } << (std::numeric_limits<Uop>::digits - 1)) : Uop{ 0 };

  const Uop xu = static_cast<Uop>(static_cast<Uop>(x) ^ sign_bit);
  const Uop yu = static_cast<Uop>(static_cast<Uop>(y) ^ sign_bit);

  const Uop gt_bits = static_cast<Uop>(xu & static_cast<Uop>(~yu));
  Uop lt_bits = static_cast<Uop>(static_cast<Uop>(~xu) & yu);

  for (size_t pow = 1; pow < std::numeric_limits<Uop>::digits;) {
    lt_bits = static_cast<Uop>(lt_bits | static_cast<Uop>(lt_bits >> pow));
    pow += pow;
  }

  Uop bit = static_cast<Uop>(gt_bits & static_cast<Uop>(~lt_bits));

  for (size_t pow = 1; pow < std::numeric_limits<Uop>::digits;) {
    bit = static_cast<Uop>(bit | static_cast<Uop>(bit >> pow));
    pow += pow;
  }

  ct_barrier(bit);
  const returnT z = static_cast<returnT>(-static_cast<returnT>((bit & 1) ^ 1));
  return z;
}

// Given two integers x, y of type operandT, this routine returns truth value (
// if x > y ) or false value ( in case x <= y ) based on result of greater than
// comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_gt(const operandT x, const operandT y)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = ct_le<operandT, returnT>(x, y);
  return static_cast<returnT>(~z); // bit-inverted result of <= test
}

// Given two integers x, y of type operandT, this routine returns truth value (
// if x >= y ) or false value ( in case x < y ) based on result of greater than
// equal comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_ge(const operandT x, const operandT y)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z0 = ct_gt<operandT, returnT>(x, y);
  const returnT z1 = ct_eq<operandT, returnT>(x, y);
  return static_cast<returnT>(z0 | z1); // (x > y) | (x == y)
}

// Given two integers x, y of type operandT, this routine returns truth value (
// if x < y ) or false value ( in case x >= y ) based on result of lesser than
// comparison test.
//
// We represent truth value using maximum number that can be represented using
// type returnT i.e. all bits of returnT are set to one. While for false value,
// we set all bits of returnT to zero.
template<typename operandT, typename returnT>
forceinline constexpr returnT
ct_lt(const operandT x, const operandT y)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
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

// Constant-time comparison of two equal-length spans of integers.
// Returns truth value (all bits set) if the spans are element-wise equal,
// or false value (all bits zero) otherwise.
//
// Composes from ct_eq: AND-accumulates per-element equality results.
// Each ct_eq call contains an optimization barrier, making its return value
// opaque to the compiler — the loop cannot be short-circuited.
template<typename operandT, typename returnT, size_t N>
forceinline constexpr returnT
ct_memcmp(std::span<const operandT, N> lhs, std::span<const operandT, N> rhs)
  requires(ct_operand<operandT> && std::is_unsigned_v<returnT>)
{
  returnT acc = static_cast<returnT>(~returnT{ 0 });
  for (size_t i = 0; i < lhs.size(); i++) {
    acc = static_cast<returnT>(acc & ct_eq<operandT, returnT>(lhs[i], rhs[i]));
  }

  return acc;
}

// Given a branch value br ( of type branchT ) holding either truth value (
// represented using value of type branchT s.t. all the bits are set to 1 ) or
// false value ( represented using value of type branchT s.t. all the bits are
// set to 0 ) and two equal-length spans dst, src of integers, this routine
// overwrites contents of dst with those of src if br is truth value. Otherwise dst
// retains its original contents.
//
// Memory-level analogue of ct_select: composes from ct_select element-wise, so
// every element of dst is touched regardless of br -- neither the code path nor
// the accessed memory addresses depend on br. Each ct_select call contains an
// optimization barrier, keeping the branch value opaque to the compiler.
//
// If br takes any value other than these two, this is an undefined behaviour !
template<typename branchT, typename operandT, size_t N>
forceinline constexpr void
ct_conditional_memcpy(const branchT br, std::span<operandT, N> dst, std::span<const operandT, N> src)
  requires(std::is_unsigned_v<branchT> && ct_operand<operandT>)
{
  for (size_t i = 0; i < dst.size(); i++) {
    dst[i] = ct_select<branchT, operandT>(br, src[i], dst[i]);
  }
}

// Given a secret index idx ( of type indexT ) and a table of integers,
// this routine returns table[idx] without ever using idx to address
// memory -- defeating cache-timing side channels that a plain table[idx] would expose.
//
// idx is expected to be in range [0, table.size()); an out-of-range idx matches
// no slot and yields a zero result.
template<typename indexT, typename operandT, size_t N>
forceinline constexpr operandT
ct_lookup(const indexT idx, std::span<const operandT, N> table)
  requires(std::is_unsigned_v<indexT> && ct_operand<operandT>)
{
  using Umask = std::make_unsigned_t<operandT>;

  operandT result = operandT{ 0 };
  for (size_t j = 0; j < table.size(); j++) {
    const Umask mask = ct_eq<indexT, Umask>(static_cast<indexT>(j), idx);
    result = ct_select<Umask, operandT>(mask, table[j], result);
  }

  return result;
}

}
