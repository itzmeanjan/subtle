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
  std::cout << "[test] Equality Test\n";

  return 0;
}
