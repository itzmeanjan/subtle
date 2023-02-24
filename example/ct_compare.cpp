#include "subtle.hpp"
#include <iostream>
#include <random>

// Note:
//
// This example shows how to use subtle header-only C++ library for comparing
// two byte arrays in constant-time. It's fairly similar to
// https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L9-L25
//
// Compile it with and execute it with
//
// $ g++ -std=c++20 -Wall -O3 -march=native -I include/ example/ct_compare.cpp
// $ ./a.out
//
// If interested in studying the assembly generated for ct_compare routine, by
// compiler, try following
//
// 1. First compile source with debug info ( and high optimization )
// $ g++ -std=c++20 -O3 -march=native -I include/ -g3 -c example/ct_compare.cpp
// 2. Disassemble object file ( along with unmangled and interleaved source )
// $ objdump -dwCS -Mintel ct_compare.o
// 3. Now study assembly.

// Generates N (>=0) -many random bytes using Mersenne Twister engine, seeded
// with system randomness.
void
gen_rand_data(uint8_t* const data, const size_t dlen)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  for (size_t i = 0; i < dlen; i++) {
    data[i] = dis(gen);
  }
}

// Compare two byte arrays ( can be of different length ), with secret data, in
// constant-time i.e. how long does it take to compare them is not anyhow
// dictated by the contents of those two byte arrays.
//
// In case content of both of those byte arrays match, truth value is returned (
// denoted by all bits of uint32_t set to 1 ), otherwise it return false (
// denoted by all bits of uint32_t set to 0 ).
uint32_t
ct_compare(const uint8_t* const __restrict tag_a,
           const size_t tag_a_len,
           const uint8_t* const __restrict tag_b,
           const size_t tag_b_len)
{
  // This is a comparison of length, not content of byte array.
  if (tag_a_len != tag_b_len) {
    return 0u;
  }

  uint32_t res = -1u;

  for (size_t i = 0; i < tag_a_len; i++) {
    res &= subtle::ct_eq<uint8_t, uint32_t>(tag_a[i], tag_b[i]);
  }

  return res;
}

int
main()
{
  constexpr size_t tag_len = 16;

  uint8_t tag_a[tag_len];
  uint8_t tag_b[tag_len];

  gen_rand_data(tag_a, tag_len);
  gen_rand_data(tag_b, tag_len);

  const auto res = ct_compare(tag_a, tag_len, tag_b, tag_len);
  std::cout << res << std::endl;

  return 0;
}
