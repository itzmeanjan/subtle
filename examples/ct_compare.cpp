#include "subtle.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

/**
 * Note:
 *
 * This example shows how to use subtle header-only C++ library for comparing
 * two byte arrays in constant-time. It's fairly similar to
 * https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L9-L25.
 */

namespace {

constexpr size_t TAG_LEN = 16;

void
gen_rand_data(std::mt19937_64& gen, std::span<uint8_t> data)
{
  std::uniform_int_distribution<uint8_t> dis;
  std::ranges::generate(data, [&]() { return dis(gen); });
}

} // namespace

int
main()
{
  std::array<uint8_t, TAG_LEN> tag_a{};
  std::array<uint8_t, TAG_LEN> tag_b{};

  std::random_device rd;
  std::mt19937_64 gen(rd());

  gen_rand_data(gen, tag_a);
  gen_rand_data(gen, tag_b);

  const auto res = subtle::ct_memcmp<uint8_t, uint32_t, TAG_LEN>(tag_a, tag_b);
  std::cout << res << '\n';

  return 0;
}
