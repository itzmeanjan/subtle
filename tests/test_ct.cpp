#include "subtle.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <sanitizer/msan_interface.h>

// Constant-time verification using Clang MemorySanitizer.
//
// Technique: mark inputs as "uninitialized" (secret) via MSan, then call the
// function under test. If the function branches on secret data or uses it to
// compute a memory address, MSan aborts with a use-of-uninitialized-value
// error. Clean exit (return 0) means all functions are constant-time.
//
// No test framework -- MSan's abort-on-violation IS the assertion mechanism.
// This avoids GTest + MSan incompatibilities (RTTI, getenv, glibc false positives).

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));

    volatile returnT sink = subtle::ct_lt<operandT, returnT>(x, y);
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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));
    __msan_allocated_memory(&br, sizeof(br));

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

    __msan_allocated_memory(&x, sizeof(x));
    __msan_allocated_memory(&y, sizeof(y));
    __msan_allocated_memory(&br, sizeof(br));

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
  std::puts("Running constant-time verification with MSan...");

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

  std::puts("All constant-time checks passed.");
  return EXIT_SUCCESS;
}
