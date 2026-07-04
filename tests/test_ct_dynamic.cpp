#include "subtle.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <random>

// Constant-time verification via uninitialized-memory taint tracking.
//
// Two backends:
//
// 1) Clang MemorySanitizer (compile-time instrumentation)
// 2) Valgrind memcheck (runtime binary instrumentation).
//
// Both use the same technique: mark inputs as "uninitialized" (secret),
// then call the function under test. If it branches on secret data or
// uses it to compute a memory address, the tool aborts. Clean exit (return 0)
// means constant-time.
//
// No test framework -- the tool's abort-on-violation is the assertion mechanism.

#ifdef __has_feature
#if __has_feature(memory_sanitizer)
#define CT_USE_MSAN
#endif
#endif

#ifdef CT_USE_MSAN
#include <sanitizer/msan_interface.h>
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CT_POISON(addr, len) __msan_allocated_memory(addr, len)
#define CT_BACKEND_NAME "MSan"
#else
#include <valgrind/memcheck.h>
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CT_POISON(addr, len) VALGRIND_MAKE_MEM_UNDEFINED(addr, len)
#define CT_BACKEND_NAME "Valgrind"
#endif

namespace {

constexpr size_t ITERATIONS = 1UL << 16;

// --- Comparison operations ---

template<typename operandT, typename returnT>
void
verify_ct_eq()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_eq<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_ne()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_ne<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_is_zero()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);

    CT_POISON(&x, sizeof(x));

    volatile returnT sink = subtle::ct_is_zero<operandT, returnT>(x);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_is_zero_span()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<operandT, FIXED_BUF_SIZE> vals{};
    std::ranges::generate(vals, [&]() { return dis(gen); });

    CT_POISON(vals.data(), vals.size() * sizeof(operandT));

    volatile returnT sink = subtle::ct_is_zero<operandT, returnT>(std::span<const operandT, FIXED_BUF_SIZE>(vals));
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_le()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_le<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_gt()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_gt<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_ge()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_ge<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

template<typename operandT, typename returnT>
void
verify_ct_lt()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));

    volatile returnT sink = subtle::ct_lt<operandT, returnT>(x, y);
    static_cast<void>(sink);
  }
}

// --- Memory comparison ---

template<typename operandT, typename returnT>
void
verify_ct_memcmp()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<operandT, FIXED_BUF_SIZE> lhs{};
    std::array<operandT, FIXED_BUF_SIZE> rhs{};

    std::ranges::generate(lhs, [&]() { return dis(gen); });
    std::ranges::generate(rhs, [&]() { return dis(gen); });

    CT_POISON(lhs.data(), lhs.size() * sizeof(operandT));
    CT_POISON(rhs.data(), rhs.size() * sizeof(operandT));

    volatile returnT sink = subtle::ct_memcmp<operandT, returnT>(std::span<const operandT, FIXED_BUF_SIZE>(lhs), std::span<const operandT, FIXED_BUF_SIZE>(rhs));
    static_cast<void>(sink);
  }
}

// --- Conditional memcpy ---

template<typename operandT, typename branchT>
void
verify_ct_conditional_memcpy()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<operandT, FIXED_BUF_SIZE> dst{};
    std::array<operandT, FIXED_BUF_SIZE> src{};
    branchT br = static_cast<branchT>(-static_cast<branchT>(gen() & 1U));

    std::ranges::generate(dst, [&]() { return dis(gen); });
    std::ranges::generate(src, [&]() { return dis(gen); });

    CT_POISON(dst.data(), dst.size() * sizeof(operandT));
    CT_POISON(src.data(), src.size() * sizeof(operandT));
    CT_POISON(&br, sizeof(br));

    subtle::ct_conditional_memcpy<branchT, operandT>(br, std::span<operandT, FIXED_BUF_SIZE>(dst), std::span<const operandT, FIXED_BUF_SIZE>(src));

    volatile operandT sink = dst[0];
    static_cast<void>(sink);
  }
}

// --- Table lookup ---

template<typename operandT, typename indexT>
void
verify_ct_lookup()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<operandT, FIXED_BUF_SIZE> table{};
    indexT idx = static_cast<indexT>(gen() % FIXED_BUF_SIZE);

    std::ranges::generate(table, [&]() { return dis(gen); });

    CT_POISON(table.data(), table.size() * sizeof(operandT));
    CT_POISON(&idx, sizeof(idx));

    volatile operandT sink = subtle::ct_lookup<indexT, operandT>(idx, std::span<const operandT, FIXED_BUF_SIZE>(table));
    static_cast<void>(sink);
  }
}

// --- Zeroize ---

template<typename T>
void
verify_ct_zeroize()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<T, FIXED_BUF_SIZE> buf{};
    std::ranges::generate(buf, [&]() { return dis(gen); });

    CT_POISON(buf.data(), buf.size() * sizeof(T));
    subtle::ct_zeroize(std::span<T, FIXED_BUF_SIZE>(buf));

    volatile T sink = buf[0];
    static_cast<void>(sink);
  }
}

// --- Conditional selection ---

template<typename operandT, typename returnT>
void
verify_ct_select()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);
    returnT br = static_cast<returnT>(-static_cast<returnT>(static_cast<returnT>(dis(gen)) & 1U));

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));
    CT_POISON(&br, sizeof(br));

    volatile operandT sink = subtle::ct_select(br, x, y);
    static_cast<void>(sink);
  }
}

// --- Conditional swap ---

template<typename operandT, typename returnT>
void
verify_ct_swap()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);
    returnT br = static_cast<returnT>(-static_cast<returnT>(static_cast<returnT>(dis(gen)) & 1U));

    CT_POISON(&x, sizeof(x));
    CT_POISON(&y, sizeof(y));
    CT_POISON(&br, sizeof(br));

    subtle::ct_swap(br, x, y);

    volatile operandT sink_x = x;
    volatile operandT sink_y = y;
    static_cast<void>(sink_x);
    static_cast<void>(sink_y);
  }
}

// --- Conditional swap (span overload) ---

template<typename operandT, typename branchT>
void
verify_ct_swap_span()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis(std::numeric_limits<operandT>::min(), std::numeric_limits<operandT>::max());

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<operandT, FIXED_BUF_SIZE> x{};
    std::array<operandT, FIXED_BUF_SIZE> y{};
    branchT br = static_cast<branchT>(-static_cast<branchT>(gen() & 1U));

    std::ranges::generate(x, [&]() { return dis(gen); });
    std::ranges::generate(y, [&]() { return dis(gen); });

    CT_POISON(x.data(), x.size() * sizeof(operandT));
    CT_POISON(y.data(), y.size() * sizeof(operandT));
    CT_POISON(&br, sizeof(br));

    subtle::ct_swap<branchT, operandT>(br, std::span<operandT, FIXED_BUF_SIZE>(x), std::span<operandT, FIXED_BUF_SIZE>(y));

    volatile operandT sink_x = x[0];
    volatile operandT sink_y = y[0];
    static_cast<void>(sink_x);
    static_cast<void>(sink_y);
  }
}

// Run a verify function for all 16 type combinations.
template<template<typename, typename> class VerifyFn>
void
verify_all_types()
{
  VerifyFn<uint8_t, uint8_t>()();
  VerifyFn<uint16_t, uint8_t>()();
  VerifyFn<uint32_t, uint8_t>()();
  VerifyFn<uint64_t, uint8_t>()();
  VerifyFn<uint8_t, uint16_t>()();
  VerifyFn<uint16_t, uint16_t>()();
  VerifyFn<uint32_t, uint16_t>()();
  VerifyFn<uint64_t, uint16_t>()();
  VerifyFn<uint8_t, uint32_t>()();
  VerifyFn<uint16_t, uint32_t>()();
  VerifyFn<uint32_t, uint32_t>()();
  VerifyFn<uint64_t, uint32_t>()();
  VerifyFn<uint8_t, uint64_t>()();
  VerifyFn<uint16_t, uint64_t>()();
  VerifyFn<uint32_t, uint64_t>()();
  VerifyFn<uint64_t, uint64_t>()();

  VerifyFn<int8_t, uint8_t>()();
  VerifyFn<int16_t, uint8_t>()();
  VerifyFn<int32_t, uint8_t>()();
  VerifyFn<int64_t, uint8_t>()();
  VerifyFn<int8_t, uint16_t>()();
  VerifyFn<int16_t, uint16_t>()();
  VerifyFn<int32_t, uint16_t>()();
  VerifyFn<int64_t, uint16_t>()();
  VerifyFn<int8_t, uint32_t>()();
  VerifyFn<int16_t, uint32_t>()();
  VerifyFn<int32_t, uint32_t>()();
  VerifyFn<int64_t, uint32_t>()();
  VerifyFn<int8_t, uint64_t>()();
  VerifyFn<int16_t, uint64_t>()();
  VerifyFn<int32_t, uint64_t>()();
  VerifyFn<int64_t, uint64_t>()();
}

template<template<typename> class VerifyFn>
void
verify_all_element_types()
{
  VerifyFn<uint8_t>()();
  VerifyFn<uint16_t>()();
  VerifyFn<uint32_t>()();
  VerifyFn<uint64_t>()();
  VerifyFn<int8_t>()();
  VerifyFn<int16_t>()();
  VerifyFn<int32_t>()();
  VerifyFn<int64_t>()();
}

// Wrapper structs to pass function templates as template-template parameters.
template<typename OpT, typename RetT>
struct eq_wrapper
{
  void operator()() { verify_ct_eq<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct ne_wrapper
{
  void operator()() { verify_ct_ne<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct is_zero_wrapper
{
  void operator()() { verify_ct_is_zero<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct is_zero_span_wrapper
{
  void operator()() { verify_ct_is_zero_span<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct le_wrapper
{
  void operator()() { verify_ct_le<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct gt_wrapper
{
  void operator()() { verify_ct_gt<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct ge_wrapper
{
  void operator()() { verify_ct_ge<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct lt_wrapper
{
  void operator()() { verify_ct_lt<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct select_wrapper
{
  void operator()() { verify_ct_select<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct swap_wrapper
{
  void operator()() { verify_ct_swap<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct swap_span_wrapper
{
  void operator()() { verify_ct_swap_span<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct memcmp_wrapper
{
  void operator()() { verify_ct_memcmp<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct conditional_memcpy_wrapper
{
  void operator()() { verify_ct_conditional_memcpy<OpT, RetT>(); }
};

template<typename OpT, typename RetT>
struct lookup_wrapper
{
  void operator()() { verify_ct_lookup<OpT, RetT>(); }
};

template<typename T>
struct zeroize_wrapper
{
  void operator()() { verify_ct_zeroize<T>(); }
};

} // anonymous namespace

int
main()
{
  std::puts("Running constant-time verification with " CT_BACKEND_NAME "...");

  std::puts("  ct_eq...");
  verify_all_types<eq_wrapper>();

  std::puts("  ct_ne...");
  verify_all_types<ne_wrapper>();

  std::puts("  ct_is_zero...");
  verify_all_types<is_zero_wrapper>();

  std::puts("  ct_is_zero (span overload)...");
  verify_all_types<is_zero_span_wrapper>();

  std::puts("  ct_le...");
  verify_all_types<le_wrapper>();

  std::puts("  ct_gt...");
  verify_all_types<gt_wrapper>();

  std::puts("  ct_ge...");
  verify_all_types<ge_wrapper>();

  std::puts("  ct_lt...");
  verify_all_types<lt_wrapper>();

  std::puts("  ct_select...");
  verify_all_types<select_wrapper>();

  std::puts("  ct_swap...");
  verify_all_types<swap_wrapper>();

  std::puts("  ct_swap (span overload)...");
  verify_all_types<swap_span_wrapper>();

  std::puts("  ct_memcmp...");
  verify_all_types<memcmp_wrapper>();

  std::puts("  ct_conditional_memcpy...");
  verify_all_types<conditional_memcpy_wrapper>();

  std::puts("  ct_lookup...");
  verify_all_types<lookup_wrapper>();

  std::puts("  ct_zeroize...");
  verify_all_element_types<zeroize_wrapper>();

  std::puts("All constant-time checks passed.");
  return EXIT_SUCCESS;
}
