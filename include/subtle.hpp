#pragma once
#include <cstdint>
#include <type_traits>

// Compare signed and unsigned integer type values in constant-time.
namespace subtle {

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns true ( if x == y ) or false ( in case x != y )
// testing equality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
static inline constexpr returnT
is_unsigned_equal(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const operandT a = x ^ y;
  const operandT b = a | (-a);
  const operandT c = b >> ((sizeof(operandT) * 8) - 1); // select only MSB
  const returnT d = -static_cast<returnT>(c ^ 1);       // prepare result

  return d;
}

// Given two unsigned integers x, y of type operandT ( of bitwidth 8, 16, 32 or
// 64 ), this routine returns true ( if x != y ) or false ( in case x == y )
// testing inequality of two values.
//
// We represent truth value using maximum number that can be represented using
// returnT i.e. all bits of returnT are set to one. While for false value, we
// set all bits of returnT to zero.
template<typename operandT, typename returnT>
static inline constexpr returnT
is_unsigned_not_equal(const operandT x, const operandT y)
  requires(std::is_unsigned_v<operandT> && std::is_unsigned_v<returnT>)
{
  const returnT z = is_unsigned_equal<operandT, returnT>(x, y);
  return ~z; // bit-reverse result of equality check
}

}
