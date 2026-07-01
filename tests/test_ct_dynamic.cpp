#include "subtle.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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
  std::uniform_int_distribution<operandT> dis;

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
  std::uniform_int_distribution<operandT> dis;

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
verify_ct_le()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<operandT> dis;

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
  std::uniform_int_distribution<operandT> dis;

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
  std::uniform_int_distribution<operandT> dis;

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
  std::uniform_int_distribution<operandT> dis;

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

template<typename T>
void
verify_ct_memcmp()
{
  constexpr size_t FIXED_BUF_SIZE = 16;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<T> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    std::array<T, FIXED_BUF_SIZE> lhs{};
    std::array<T, FIXED_BUF_SIZE> rhs{};

    std::ranges::generate(lhs, [&]() { return dis(gen); });
    std::ranges::generate(rhs, [&]() { return dis(gen); });

    CT_POISON(lhs.data(), lhs.size() * sizeof(T));
    CT_POISON(rhs.data(), rhs.size() * sizeof(T));

    volatile T sink = subtle::ct_memcmp<T, T>(std::span<const T, FIXED_BUF_SIZE>(lhs), std::span<const T, FIXED_BUF_SIZE>(rhs));
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
  std::uniform_int_distribution<T> dis;

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
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);
    returnT br = -static_cast<returnT>(dis(gen) & 1U);

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
  std::uniform_int_distribution<operandT> dis;

  for (size_t i = 0; i < ITERATIONS; i++) {
    operandT x = dis(gen);
    operandT y = dis(gen);
    returnT br = -static_cast<returnT>(dis(gen) & 1U);

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

} // anonymous namespace

int
main()
{
  std::puts("Running constant-time verification with " CT_BACKEND_NAME "...");

  std::puts("  ct_eq...");
  verify_all_types<eq_wrapper>();

  std::puts("  ct_ne...");
  verify_all_types<ne_wrapper>();

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

  std::puts("  ct_memcmp...");
  verify_ct_memcmp<uint8_t>();
  verify_ct_memcmp<uint16_t>();
  verify_ct_memcmp<uint32_t>();
  verify_ct_memcmp<uint64_t>();

  std::puts("  ct_zeroize...");
  verify_ct_zeroize<uint8_t>();
  verify_ct_zeroize<uint16_t>();
  verify_ct_zeroize<uint32_t>();
  verify_ct_zeroize<uint64_t>();

  std::puts("All constant-time checks passed.");
  return EXIT_SUCCESS;
}
