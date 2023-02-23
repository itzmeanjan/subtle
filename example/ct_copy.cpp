#include "subtle.hpp"
#include <cassert>
#include <random>

// Note:
//
// This example shows how to use subtle header-only C++ library for copying
// a source byte array into another one, based on some conditional flag, in
// constant-time. It's fairly similar to
// https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L41-L54
//
// Compile it with and execute it with
//
// $ g++ -std=c++20 -Wall -O3 -march=native -I include/ example/ct_copy.cpp
// $ ./a.out
//
// If interested in studying the assembly generated for ct_copy routine, by
// compiler, try following
//
// 1. First compile source with debug info ( and high optimization )
// $ g++ -std=c++20 -O3 -march=native -I include/ -g3 -c example/ct_copy.cpp
// 2. Disassemble object file ( along with unmangled and interleaved source )
// $ objdump -dwCS -Mintel ct_copy.o
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

// Copy bytes from source byte array to destination byte array, based on what
// value of conditional variable br is. If br is truth value, then bytes are
// copied from src to dst. Otherwise bytes of dst are set to zeros.
//
// Truth value is denoted by all bits of uint32_t set to 1, while false value is
// denoted by all bits of uint32_t set to 0.
uint32_t
ct_copy(const uint32_t br,
        const uint8_t* const __restrict src,
        const size_t src_len,
        uint8_t* const __restrict dst,
        const size_t dst_len)
{
  // This shouldn't affect constant-timeness, because we're not inpecting
  // content of source byte array.
  if (src_len != dst_len) {
    return 0u;
  }

  for (size_t i = 0; i < src_len; i++) {
    dst[i] = subtle::ct_select<uint32_t, uint8_t>(br, src[i], dst[i]);
  }

  return -1u;
}

int
main()
{
  constexpr size_t dlen = 16;

  uint8_t src[dlen];
  uint8_t dst[dlen];

  gen_rand_data(src, dlen);
  gen_rand_data(dst, dlen);

  // random branch value, so that compiler can't do optimization !
  const uint32_t br = -static_cast<uint32_t>((src[0] ^ dst[0]) & 1);
  ct_copy(br, src, dlen, dst, dlen);

  for (size_t i = 0; i < dlen; i++) {
    assert(dst[i] == (static_cast<bool>(br) ? src[i] : dst[i]));
  }

  return 0;
}
