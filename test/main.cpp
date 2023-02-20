#include "test_subtle.hpp"
#include <iostream>

int
main()
{
  subtle_test::test_ct_eq<uint8_t, uint8_t>();
  subtle_test::test_ct_eq<uint16_t, uint8_t>();
  subtle_test::test_ct_eq<uint32_t, uint8_t>();
  subtle_test::test_ct_eq<uint64_t, uint8_t>();

  subtle_test::test_ct_eq<uint8_t, uint16_t>();
  subtle_test::test_ct_eq<uint16_t, uint16_t>();
  subtle_test::test_ct_eq<uint32_t, uint16_t>();
  subtle_test::test_ct_eq<uint64_t, uint16_t>();

  subtle_test::test_ct_eq<uint8_t, uint32_t>();
  subtle_test::test_ct_eq<uint16_t, uint32_t>();
  subtle_test::test_ct_eq<uint32_t, uint32_t>();
  subtle_test::test_ct_eq<uint64_t, uint32_t>();

  subtle_test::test_ct_eq<uint8_t, uint64_t>();
  subtle_test::test_ct_eq<uint16_t, uint64_t>();
  subtle_test::test_ct_eq<uint32_t, uint64_t>();
  subtle_test::test_ct_eq<uint64_t, uint64_t>();
  std::cout << "[test] Equality test\n";

  subtle_test::test_ct_ne<uint8_t, uint8_t>();
  subtle_test::test_ct_ne<uint16_t, uint8_t>();
  subtle_test::test_ct_ne<uint32_t, uint8_t>();
  subtle_test::test_ct_ne<uint64_t, uint8_t>();

  subtle_test::test_ct_ne<uint8_t, uint16_t>();
  subtle_test::test_ct_ne<uint16_t, uint16_t>();
  subtle_test::test_ct_ne<uint32_t, uint16_t>();
  subtle_test::test_ct_ne<uint64_t, uint16_t>();

  subtle_test::test_ct_ne<uint8_t, uint32_t>();
  subtle_test::test_ct_ne<uint16_t, uint32_t>();
  subtle_test::test_ct_ne<uint32_t, uint32_t>();
  subtle_test::test_ct_ne<uint64_t, uint32_t>();

  subtle_test::test_ct_ne<uint8_t, uint64_t>();
  subtle_test::test_ct_ne<uint16_t, uint64_t>();
  subtle_test::test_ct_ne<uint32_t, uint64_t>();
  subtle_test::test_ct_ne<uint64_t, uint64_t>();
  std::cout << "[test] Inequality test\n";

  subtle_test::test_ct_le<uint8_t, uint8_t>();
  subtle_test::test_ct_le<uint16_t, uint8_t>();
  subtle_test::test_ct_le<uint32_t, uint8_t>();
  subtle_test::test_ct_le<uint64_t, uint8_t>();

  subtle_test::test_ct_le<uint8_t, uint16_t>();
  subtle_test::test_ct_le<uint16_t, uint16_t>();
  subtle_test::test_ct_le<uint32_t, uint16_t>();
  subtle_test::test_ct_le<uint64_t, uint16_t>();

  subtle_test::test_ct_le<uint8_t, uint32_t>();
  subtle_test::test_ct_le<uint16_t, uint32_t>();
  subtle_test::test_ct_le<uint32_t, uint32_t>();
  subtle_test::test_ct_le<uint64_t, uint32_t>();

  subtle_test::test_ct_le<uint8_t, uint64_t>();
  subtle_test::test_ct_le<uint16_t, uint64_t>();
  subtle_test::test_ct_le<uint32_t, uint64_t>();
  subtle_test::test_ct_le<uint64_t, uint64_t>();
  std::cout << "[test] Lesser than equality test\n";

  subtle_test::test_ct_gt<uint8_t, uint8_t>();
  subtle_test::test_ct_gt<uint16_t, uint8_t>();
  subtle_test::test_ct_gt<uint32_t, uint8_t>();
  subtle_test::test_ct_gt<uint64_t, uint8_t>();

  subtle_test::test_ct_gt<uint8_t, uint16_t>();
  subtle_test::test_ct_gt<uint16_t, uint16_t>();
  subtle_test::test_ct_gt<uint32_t, uint16_t>();
  subtle_test::test_ct_gt<uint64_t, uint16_t>();

  subtle_test::test_ct_gt<uint8_t, uint32_t>();
  subtle_test::test_ct_gt<uint16_t, uint32_t>();
  subtle_test::test_ct_gt<uint32_t, uint32_t>();
  subtle_test::test_ct_gt<uint64_t, uint32_t>();

  subtle_test::test_ct_gt<uint8_t, uint64_t>();
  subtle_test::test_ct_gt<uint16_t, uint64_t>();
  subtle_test::test_ct_gt<uint32_t, uint64_t>();
  subtle_test::test_ct_gt<uint64_t, uint64_t>();
  std::cout << "[test] Greater than test\n";

  subtle_test::test_ct_ge<uint8_t, uint8_t>();
  subtle_test::test_ct_ge<uint16_t, uint8_t>();
  subtle_test::test_ct_ge<uint32_t, uint8_t>();
  subtle_test::test_ct_ge<uint64_t, uint8_t>();

  subtle_test::test_ct_ge<uint8_t, uint16_t>();
  subtle_test::test_ct_ge<uint16_t, uint16_t>();
  subtle_test::test_ct_ge<uint32_t, uint16_t>();
  subtle_test::test_ct_ge<uint64_t, uint16_t>();

  subtle_test::test_ct_ge<uint8_t, uint32_t>();
  subtle_test::test_ct_ge<uint16_t, uint32_t>();
  subtle_test::test_ct_ge<uint32_t, uint32_t>();
  subtle_test::test_ct_ge<uint64_t, uint32_t>();

  subtle_test::test_ct_ge<uint8_t, uint64_t>();
  subtle_test::test_ct_ge<uint16_t, uint64_t>();
  subtle_test::test_ct_ge<uint32_t, uint64_t>();
  subtle_test::test_ct_ge<uint64_t, uint64_t>();
  std::cout << "[test] Greater than equality test\n";

  subtle_test::test_ct_lt<uint8_t, uint8_t>();
  subtle_test::test_ct_lt<uint16_t, uint8_t>();
  subtle_test::test_ct_lt<uint32_t, uint8_t>();
  subtle_test::test_ct_lt<uint64_t, uint8_t>();

  subtle_test::test_ct_lt<uint8_t, uint16_t>();
  subtle_test::test_ct_lt<uint16_t, uint16_t>();
  subtle_test::test_ct_lt<uint32_t, uint16_t>();
  subtle_test::test_ct_lt<uint64_t, uint16_t>();

  subtle_test::test_ct_lt<uint8_t, uint32_t>();
  subtle_test::test_ct_lt<uint16_t, uint32_t>();
  subtle_test::test_ct_lt<uint32_t, uint32_t>();
  subtle_test::test_ct_lt<uint64_t, uint32_t>();

  subtle_test::test_ct_lt<uint8_t, uint64_t>();
  subtle_test::test_ct_lt<uint16_t, uint64_t>();
  subtle_test::test_ct_lt<uint32_t, uint64_t>();
  subtle_test::test_ct_lt<uint64_t, uint64_t>();
  std::cout << "[test] Lesser than test\n";

  return 0;
}
