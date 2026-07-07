#include "subtle.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

/**
 * Note:
 *
 * This example shows how to use subtle header-only C++ library for copying
 * a source byte array into another one, based on some conditional flag, in
 * constant-time. It's fairly similar to
 * https://github.com/golang/go/blob/f07fafacef04e8a8eaf2c8d2e1a14dc88e8683ee/src/crypto/subtle/constant_time.go#L41-L54
 */

namespace {

constexpr size_t DLEN = 16;

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
  std::array<uint8_t, DLEN> src{};
  std::array<uint8_t, DLEN> dst{};

  std::random_device rd;
  std::mt19937_64 gen(rd());

  gen_rand_data(gen, src);
  gen_rand_data(gen, dst);

  // random branch value, so that compiler can't do optimization !
  const uint32_t br = -static_cast<uint32_t>((src[0] ^ dst[0]) & 1U);
  std::cout << "branch = " << br << '\n';

  subtle::ct_conditional_memcpy<uint32_t, uint8_t, DLEN>(br, dst, src);
  std::cout << (src == dst ? "copied" : "not copied") << '\n';

  return 0;
}
