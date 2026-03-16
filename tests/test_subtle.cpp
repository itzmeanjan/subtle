#include "test_helper.hpp"
#include <gtest/gtest.h>

using TypeCombinations = ::testing::Types<std::pair<uint8_t, uint8_t>,
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
                                          std::pair<uint64_t, uint64_t>>;

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

// --- ct_swap_truth tests ---

template<typename T>
class CtSwapTruthTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSwapTruthTest, TypeCombinations);

TYPED_TEST(CtSwapTruthTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_swap_truth<operandT, returnT>();
}

// --- ct_swap_roundtrip tests ---

template<typename T>
class CtSwapRoundtripTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSwapRoundtripTest, TypeCombinations);

TYPED_TEST(CtSwapRoundtripTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_swap_roundtrip<operandT, returnT>();
}

// --- ct_swap_false tests ---

template<typename T>
class CtSwapFalseTest : public ::testing::Test
{};

TYPED_TEST_SUITE(CtSwapFalseTest, TypeCombinations);

TYPED_TEST(CtSwapFalseTest, Correctness)
{
  using operandT = typename TypeParam::first_type;
  using returnT = typename TypeParam::second_type;

  test_subtle::test_ct_swap_false<operandT, returnT>();
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
