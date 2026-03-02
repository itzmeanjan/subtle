#include "subtle.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

// Note:
//
// This example shows how to use subtle header-only C++ library for copying
// a source byte array into another one, based on some conditional flag, in
// constant-time. It's fairly similar to
// https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L41-L54

namespace {

constexpr size_t DLEN = 16;

// Generates N (>=0) -many random bytes using Mersenne Twister engine, seeded
// with system randomness.
void
gen_rand_data(std::array<uint8_t, DLEN>& data)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  for (auto& byte : data) {
    byte = dis(gen);
  }
}

// Copy bytes from source byte array to destination byte array, based on what
// value of conditional variable br is. If br is truth value, then bytes are
// copied from src to dst. Otherwise bytes of dst are unchanged.
//
// Truth value is denoted by all bits of uint32_t set to 1, while false value is
// denoted by all bits of uint32_t set to 0.
void
ct_copy(const uint32_t br, const std::array<uint8_t, DLEN>& src, std::array<uint8_t, DLEN>& dst)
{
  for (size_t i = 0; i < DLEN; i++) {
    dst.at(i) = subtle::ct_select<uint32_t, uint8_t>(br, src.at(i), dst.at(i));
  }
}

} // namespace

int
main()
{
  std::array<uint8_t, DLEN> src{};
  std::array<uint8_t, DLEN> dst{};

  gen_rand_data(src);
  gen_rand_data(dst);

  // random branch value, so that compiler can't do optimization !
  const uint32_t br = -static_cast<uint32_t>((src.at(0) ^ dst.at(0)) & 1U);

  std::cout << br << '\n';
  ct_copy(br, src, dst);
  std::cout << (src == dst ? "copied" : "not copied") << '\n';

  return 0;
}
