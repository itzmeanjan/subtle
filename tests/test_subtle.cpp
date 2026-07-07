#include "test_helper.hpp"
#include <gtest/gtest.h>

using TypeCombinations = ::testing::Types<
  // signed
  std::pair<int8_t, uint8_t>,
  std::pair<int16_t, uint8_t>,
  std::pair<int32_t, uint8_t>,
  std::pair<int64_t, uint8_t>,
  std::pair<int8_t, uint16_t>,
  std::pair<int16_t, uint16_t>,
  std::pair<int32_t, uint16_t>,
  std::pair<int64_t, uint16_t>,
  std::pair<int8_t, uint32_t>,
  std::pair<int16_t, uint32_t>,
  std::pair<int32_t, uint32_t>,
  std::pair<int64_t, uint32_t>,
  std::pair<int8_t, uint64_t>,
  std::pair<int16_t, uint64_t>,
  std::pair<int32_t, uint64_t>,
  std::pair<int64_t, uint64_t>,
  // unsigned
  std::pair<uint8_t, uint8_t>,
  std::pair<uint16_t, uint8_t>,
  std::pair<uint32_t, uint8_t>,
  std::pair<uint64_t, uint8_t>,
  std::pair<uint8_t, uint16_t>,
  std::pair<uint16_t, uint16_t>,
  std::pair<uint32_t, uint16_t>,
  std::pair<uint64_t, uint16_t>,
  std::pair<uint8_t, uint32_t>,
  std::pair<uint16_t, uint32_t>,
  std::pair<uint32_t, uint32_t>,
  std::pair<uint64_t, uint32_t>,
  std::pair<uint8_t, uint64_t>,
  std::pair<uint16_t, uint64_t>,
  std::pair<uint32_t, uint64_t>,
  std::pair<uint64_t, uint64_t>,
  // char
  std::pair<char, uint8_t>,
  std::pair<char8_t, uint8_t>,
  std::pair<char16_t, uint8_t>,
  std::pair<char32_t, uint8_t>,
  std::pair<char, uint16_t>,
  std::pair<char8_t, uint16_t>,
  std::pair<char16_t, uint16_t>,
  std::pair<char32_t, uint16_t>,
  std::pair<char, uint32_t>,
  std::pair<char8_t, uint32_t>,
  std::pair<char16_t, uint32_t>,
  std::pair<char32_t, uint32_t>,
  std::pair<char, uint64_t>,
  std::pair<char8_t, uint64_t>,
  std::pair<char16_t, uint64_t>,
  std::pair<char32_t, uint64_t>>;

// --- ct_eq tests ---

template<typename T>
class CtEqTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtEqTest, TypeCombinations);

TYPED_TEST(CtEqTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_eq<operandT, returnT>();
}

// --- ct_ne tests ---

template<typename T>
class CtNeTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtNeTest, TypeCombinations);

TYPED_TEST(CtNeTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_ne<operandT, returnT>();
}

// --- ct_is_zero tests ---

template<typename T>
class CtIsZeroTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtIsZeroTest, TypeCombinations);

TYPED_TEST(CtIsZeroTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_is_zero<operandT, returnT>();
}

// --- ct_is_zero (span overload) tests ---

template<typename T>
class CtIsZeroSpanTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtIsZeroSpanTest, TypeCombinations);

TYPED_TEST(CtIsZeroSpanTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_is_zero_span<operandT, returnT>();
}

// --- ct_select tests ---

template<typename T>
class CtSelectTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSelectTest, TypeCombinations);

TYPED_TEST(CtSelectTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_select<operandT, returnT>();
}

// --- ct_swap tests ---

template<typename T>
class CtSwapTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSwapTest, TypeCombinations);

TYPED_TEST(CtSwapTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_swap<operandT, returnT>();
}

// --- ct_swap (span overload) tests ---

template<typename T>
class CtSwapSpanTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSwapSpanTest, TypeCombinations);

TYPED_TEST(CtSwapSpanTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using branchT = typename TypeParam::second_type;

  test_subtle::test_ct_swap_span<operandT, branchT>();
}

// --- ct_le tests ---

template<typename T>
class CtLeTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtLeTest, TypeCombinations);

TYPED_TEST(CtLeTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_le<operandT, returnT>();
}

// --- ct_gt tests ---

template<typename T>
class CtGtTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtGtTest, TypeCombinations);

TYPED_TEST(CtGtTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_gt<operandT, returnT>();
}

// --- ct_ge tests ---

template<typename T>
class CtGeTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtGeTest, TypeCombinations);

TYPED_TEST(CtGeTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_ge<operandT, returnT>();
}

// --- ct_lt tests ---

template<typename T>
class CtLtTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtLtTest, TypeCombinations);

TYPED_TEST(CtLtTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_lt<operandT, returnT>();
}

// --- ct_min tests ---

template<typename T>
class CtMinTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtMinTest, TypeCombinations);

TYPED_TEST(CtMinTest, Correctness)
{
  using operandT = typename TypeParam::first_type;

  test_subtle::test_ct_min<operandT>();
}

// --- ct_max tests ---

template<typename T>
class CtMaxTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtMaxTest, TypeCombinations);

TYPED_TEST(CtMaxTest, Correctness)
{
  using operandT = typename TypeParam::first_type;

  test_subtle::test_ct_max<operandT>();
}

// --- ct_zeroize tests ---

using ElementTypes =
  ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, char, unsigned char, signed char, char8_t, char16_t, char32_t, std::byte>;

template<typename T>
class CtZeroizeTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtZeroizeTest, ElementTypes);

TYPED_TEST(CtZeroizeTest, Correctness)
{
  test_subtle::test_ct_zeroize<TypeParam>();
}

// --- ct_memcmp tests ---

template<typename T>
class CtMemcmpTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtMemcmpTest, TypeCombinations);

TYPED_TEST(CtMemcmpTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_memcmp<operandT, returnT>();
}

// --- ct_conditional_memcpy tests ---

template<typename T>
class CtConditionalMemcpyTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtConditionalMemcpyTest, TypeCombinations);

TYPED_TEST(CtConditionalMemcpyTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using branchT = typename TypeParam::second_type;

  test_subtle::test_ct_conditional_memcpy<operandT, branchT>();
}

// --- ct_conditional_memset tests ---

template<typename T>
class CtConditionalMemsetTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtConditionalMemsetTest, TypeCombinations);

TYPED_TEST(CtConditionalMemsetTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using branchT = typename TypeParam::second_type;

  test_subtle::test_ct_conditional_memset<operandT, branchT>();
}

// --- ct_lookup tests ---

template<typename T>
class CtLookupTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtLookupTest, TypeCombinations);

TYPED_TEST(CtLookupTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using indexT = typename TypeParam::second_type;

  test_subtle::test_ct_lookup<operandT, indexT>();
}
