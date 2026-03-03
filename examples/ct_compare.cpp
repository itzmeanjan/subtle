#include "subtle.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

// Note:
//
// This example shows how to use subtle header-only C++ library for comparing
// two byte arrays in constant-time. It's fairly similar to
// https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L9-L25

namespace {

constexpr size_t TAG_LEN = 16;

// Generates N (>=0) -many random bytes using Mersenne Twister engine, seeded
// with system randomness.
void
gen_rand_data(std::array<uint8_t, TAG_LEN>& data)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint8_t> dis;

  for (auto& byte : data) {
    byte = dis(gen);
  }
}

// Compare two byte arrays in constant-time i.e. how long does it take to
// compare them is not anyhow dictated by the contents of those two byte arrays.
//
// In case content of both of those byte arrays match, truth value is returned (
// denoted by all bits of uint32_t set to 1 ), otherwise it return false (
// denoted by all bits of uint32_t set to 0 ).
uint32_t
ct_compare(const std::array<uint8_t, TAG_LEN>& tag_a, const std::array<uint8_t, TAG_LEN>& tag_b)
{
  uint32_t res = -1U;

  for (size_t i = 0; i < TAG_LEN; i++) {
    res &= subtle::ct_eq<uint8_t, uint32_t>(tag_a.at(i), tag_b.at(i));
  }

  return res;
}

} // namespace

int
main()
{
  std::array<uint8_t, TAG_LEN> tag_a{};
  std::array<uint8_t, TAG_LEN> tag_b{};

  gen_rand_data(tag_a);
  gen_rand_data(tag_b);

  const auto res = ct_compare(tag_a, tag_b);
  std::cout << res << '\n';

  return 0;
}
