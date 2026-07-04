// Binsec constant-time verification harness.
//
// Binsec performs relational symbolic execution on compiled binaries.
// It proves constant-time behavior for ALL inputs — not just exercised paths.
//
// Secret inputs are global variables referenced by symbol name in .cfg scripts.
// Each function is extern "C" for predictable (unmangled) symbol names.
// Binsec starts execution from the specified function, not main().
// Each wrapper calls _exit(0) so binsec has a clean halt point.
//
// Usage:
//   binsec -sse -checkct -sse-script check.cfg <this_binary>

#include "subtle.hpp"
#include <cstdint>
#include <unistd.h>

// --- Global secret inputs (one set per bitwidth) ---

uint8_t secret_x8;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint8_t secret_y8;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint8_t secret_br8; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

uint16_t secret_x16;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint16_t secret_y16;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint16_t secret_br16; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

uint32_t secret_x32;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint32_t secret_y32;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint32_t secret_br32; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

uint64_t secret_x64;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint64_t secret_y64;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
uint64_t secret_br64; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

// --- Global signed secret inputs (used by the ordering functions) ---

int8_t secret_ix8;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int8_t secret_iy8;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int16_t secret_ix16; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int16_t secret_iy16; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int32_t secret_ix32; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int32_t secret_iy32; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int64_t secret_ix64; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
int64_t secret_iy64; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

// --- Global secret buffer inputs (used by ct_zeroize and ct_memcmp) ---

constexpr size_t SECRET_BUF_LEN = 16;

// clang-format off
uint8_t  secret_buf1_u8[SECRET_BUF_LEN];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint16_t secret_buf1_u16[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint32_t secret_buf1_u32[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint64_t secret_buf1_u64[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint8_t  secret_buf2_u8[SECRET_BUF_LEN];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint16_t secret_buf2_u16[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint32_t secret_buf2_u32[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
uint64_t secret_buf2_u64[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int8_t   secret_buf1_i8[SECRET_BUF_LEN];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int16_t  secret_buf1_i16[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int32_t  secret_buf1_i32[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int64_t  secret_buf1_i64[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int8_t   secret_buf2_i8[SECRET_BUF_LEN];  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int16_t  secret_buf2_i16[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int32_t  secret_buf2_i32[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
int64_t  secret_buf2_i64[SECRET_BUF_LEN]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-avoid-c-arrays,misc-use-internal-linkage,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
// clang-format on

// --- Volatile sinks (prevent dead code elimination) ---

volatile uint8_t sink8;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
volatile uint16_t sink16; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
volatile uint32_t sink32; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)
volatile uint64_t sink64; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables,misc-use-internal-linkage)

// --- ct_eq ---

extern "C" void
binsec_ct_eq_u8()
{
  sink8 = subtle::ct_eq<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_eq_u16()
{
  sink16 = subtle::ct_eq<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_eq_u32()
{
  sink32 = subtle::ct_eq<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_eq_u64()
{
  sink64 = subtle::ct_eq<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- ct_ne ---

extern "C" void
binsec_ct_ne_u8()
{
  sink8 = subtle::ct_ne<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_ne_u16()
{
  sink16 = subtle::ct_ne<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_ne_u32()
{
  sink32 = subtle::ct_ne<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_ne_u64()
{
  sink64 = subtle::ct_ne<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- ct_le ---

extern "C" void
binsec_ct_le_u8()
{
  sink8 = subtle::ct_le<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_le_u16()
{
  sink16 = subtle::ct_le<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_le_u32()
{
  sink32 = subtle::ct_le<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_le_u64()
{
  sink64 = subtle::ct_le<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- ct_gt ---

extern "C" void
binsec_ct_gt_u8()
{
  sink8 = subtle::ct_gt<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_gt_u16()
{
  sink16 = subtle::ct_gt<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_gt_u32()
{
  sink32 = subtle::ct_gt<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_gt_u64()
{
  sink64 = subtle::ct_gt<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- ct_ge ---

extern "C" void
binsec_ct_ge_u8()
{
  sink8 = subtle::ct_ge<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_ge_u16()
{
  sink16 = subtle::ct_ge<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_ge_u32()
{
  sink32 = subtle::ct_ge<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_ge_u64()
{
  sink64 = subtle::ct_ge<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- ct_lt ---

extern "C" void
binsec_ct_lt_u8()
{
  sink8 = subtle::ct_lt<uint8_t, uint8_t>(secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_lt_u16()
{
  sink16 = subtle::ct_lt<uint16_t, uint16_t>(secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_lt_u32()
{
  sink32 = subtle::ct_lt<uint32_t, uint32_t>(secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_lt_u64()
{
  sink64 = subtle::ct_lt<uint64_t, uint64_t>(secret_x64, secret_y64);
  _exit(0);
}

// --- Ordering over signed operands (both operands secret) ---
//
// Signed support adds a compile-time sign-bit flip in ct_le; gt/ge/lt derive
// from it. These prove that flip lowers to branchless code for all inputs.

extern "C" void
binsec_ct_le_i8()
{
  sink8 = subtle::ct_le<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_le_i16()
{
  sink16 = subtle::ct_le<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_le_i32()
{
  sink32 = subtle::ct_le<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_le_i64()
{
  sink64 = subtle::ct_le<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

extern "C" void
binsec_ct_gt_i8()
{
  sink8 = subtle::ct_gt<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_gt_i16()
{
  sink16 = subtle::ct_gt<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_gt_i32()
{
  sink32 = subtle::ct_gt<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_gt_i64()
{
  sink64 = subtle::ct_gt<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

extern "C" void
binsec_ct_ge_i8()
{
  sink8 = subtle::ct_ge<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_ge_i16()
{
  sink16 = subtle::ct_ge<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_ge_i32()
{
  sink32 = subtle::ct_ge<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_ge_i64()
{
  sink64 = subtle::ct_ge<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

extern "C" void
binsec_ct_lt_i8()
{
  sink8 = subtle::ct_lt<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_lt_i16()
{
  sink16 = subtle::ct_lt<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_lt_i32()
{
  sink32 = subtle::ct_lt<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_lt_i64()
{
  sink64 = subtle::ct_lt<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

// --- Signed operands: equality, selection, swap, and the span operations ---

extern "C" void
binsec_ct_eq_i8()
{
  sink8 = subtle::ct_eq<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_eq_i16()
{
  sink16 = subtle::ct_eq<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_eq_i32()
{
  sink32 = subtle::ct_eq<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_eq_i64()
{
  sink64 = subtle::ct_eq<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

extern "C" void
binsec_ct_ne_i8()
{
  sink8 = subtle::ct_ne<int8_t, uint8_t>(secret_ix8, secret_iy8);
  _exit(0);
}
extern "C" void
binsec_ct_ne_i16()
{
  sink16 = subtle::ct_ne<int16_t, uint16_t>(secret_ix16, secret_iy16);
  _exit(0);
}
extern "C" void
binsec_ct_ne_i32()
{
  sink32 = subtle::ct_ne<int32_t, uint32_t>(secret_ix32, secret_iy32);
  _exit(0);
}
extern "C" void
binsec_ct_ne_i64()
{
  sink64 = subtle::ct_ne<int64_t, uint64_t>(secret_ix64, secret_iy64);
  _exit(0);
}

extern "C" void
binsec_ct_select_i8()
{
  sink8 = static_cast<uint8_t>(subtle::ct_select<uint8_t, int8_t>(secret_br8, secret_ix8, secret_iy8));
  _exit(0);
}
extern "C" void
binsec_ct_select_i16()
{
  sink16 = static_cast<uint16_t>(subtle::ct_select<uint16_t, int16_t>(secret_br16, secret_ix16, secret_iy16));
  _exit(0);
}
extern "C" void
binsec_ct_select_i32()
{
  sink32 = static_cast<uint32_t>(subtle::ct_select<uint32_t, int32_t>(secret_br32, secret_ix32, secret_iy32));
  _exit(0);
}
extern "C" void
binsec_ct_select_i64()
{
  sink64 = static_cast<uint64_t>(subtle::ct_select<uint64_t, int64_t>(secret_br64, secret_ix64, secret_iy64));
  _exit(0);
}

extern "C" void
binsec_ct_swap_i8()
{
  subtle::ct_swap<uint8_t, int8_t>(secret_br8, secret_ix8, secret_iy8);
  sink8 = static_cast<uint8_t>(secret_ix8);
  _exit(0);
}
extern "C" void
binsec_ct_swap_i16()
{
  subtle::ct_swap<uint16_t, int16_t>(secret_br16, secret_ix16, secret_iy16);
  sink16 = static_cast<uint16_t>(secret_ix16);
  _exit(0);
}
extern "C" void
binsec_ct_swap_i32()
{
  subtle::ct_swap<uint32_t, int32_t>(secret_br32, secret_ix32, secret_iy32);
  sink32 = static_cast<uint32_t>(secret_ix32);
  _exit(0);
}
extern "C" void
binsec_ct_swap_i64()
{
  subtle::ct_swap<uint64_t, int64_t>(secret_br64, secret_ix64, secret_iy64);
  sink64 = static_cast<uint64_t>(secret_ix64);
  _exit(0);
}

extern "C" void
binsec_ct_swap_span_i8()
{
  subtle::ct_swap<uint8_t, int8_t>(secret_br8, std::span<int8_t, SECRET_BUF_LEN>(secret_buf1_i8), std::span<int8_t, SECRET_BUF_LEN>(secret_buf2_i8));
  sink8 = static_cast<uint8_t>(secret_buf1_i8[0]);
  _exit(0);
}
extern "C" void
binsec_ct_swap_span_i16()
{
  subtle::ct_swap<uint16_t, int16_t>(secret_br16, std::span<int16_t, SECRET_BUF_LEN>(secret_buf1_i16), std::span<int16_t, SECRET_BUF_LEN>(secret_buf2_i16));
  sink16 = static_cast<uint16_t>(secret_buf1_i16[0]);
  _exit(0);
}
extern "C" void
binsec_ct_swap_span_i32()
{
  subtle::ct_swap<uint32_t, int32_t>(secret_br32, std::span<int32_t, SECRET_BUF_LEN>(secret_buf1_i32), std::span<int32_t, SECRET_BUF_LEN>(secret_buf2_i32));
  sink32 = static_cast<uint32_t>(secret_buf1_i32[0]);
  _exit(0);
}
extern "C" void
binsec_ct_swap_span_i64()
{
  subtle::ct_swap<uint64_t, int64_t>(secret_br64, std::span<int64_t, SECRET_BUF_LEN>(secret_buf1_i64), std::span<int64_t, SECRET_BUF_LEN>(secret_buf2_i64));
  sink64 = static_cast<uint64_t>(secret_buf1_i64[0]);
  _exit(0);
}

extern "C" void
binsec_ct_memcmp_i8()
{
  sink8 = subtle::ct_memcmp<int8_t, uint8_t>(std::span<const int8_t, SECRET_BUF_LEN>(secret_buf1_i8), std::span<const int8_t, SECRET_BUF_LEN>(secret_buf2_i8));
  _exit(0);
}
extern "C" void
binsec_ct_memcmp_i16()
{
  sink16 = subtle::ct_memcmp<int16_t, uint16_t>(std::span<const int16_t, SECRET_BUF_LEN>(secret_buf1_i16), std::span<const int16_t, SECRET_BUF_LEN>(secret_buf2_i16));
  _exit(0);
}
extern "C" void
binsec_ct_memcmp_i32()
{
  sink32 = subtle::ct_memcmp<int32_t, uint32_t>(std::span<const int32_t, SECRET_BUF_LEN>(secret_buf1_i32), std::span<const int32_t, SECRET_BUF_LEN>(secret_buf2_i32));
  _exit(0);
}
extern "C" void
binsec_ct_memcmp_i64()
{
  sink64 = subtle::ct_memcmp<int64_t, uint64_t>(std::span<const int64_t, SECRET_BUF_LEN>(secret_buf1_i64), std::span<const int64_t, SECRET_BUF_LEN>(secret_buf2_i64));
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_i8()
{
  subtle::ct_conditional_memcpy<uint8_t, int8_t>(secret_br8, std::span<int8_t, SECRET_BUF_LEN>(secret_buf1_i8), std::span<const int8_t, SECRET_BUF_LEN>(secret_buf2_i8));
  sink8 = static_cast<uint8_t>(secret_buf1_i8[0]);
  _exit(0);
}
extern "C" void
binsec_ct_conditional_memcpy_i16()
{
  subtle::ct_conditional_memcpy<uint16_t, int16_t>(
    secret_br16, std::span<int16_t, SECRET_BUF_LEN>(secret_buf1_i16), std::span<const int16_t, SECRET_BUF_LEN>(secret_buf2_i16));
  sink16 = static_cast<uint16_t>(secret_buf1_i16[0]);
  _exit(0);
}
extern "C" void
binsec_ct_conditional_memcpy_i32()
{
  subtle::ct_conditional_memcpy<uint32_t, int32_t>(
    secret_br32, std::span<int32_t, SECRET_BUF_LEN>(secret_buf1_i32), std::span<const int32_t, SECRET_BUF_LEN>(secret_buf2_i32));
  sink32 = static_cast<uint32_t>(secret_buf1_i32[0]);
  _exit(0);
}
extern "C" void
binsec_ct_conditional_memcpy_i64()
{
  subtle::ct_conditional_memcpy<uint64_t, int64_t>(
    secret_br64, std::span<int64_t, SECRET_BUF_LEN>(secret_buf1_i64), std::span<const int64_t, SECRET_BUF_LEN>(secret_buf2_i64));
  sink64 = static_cast<uint64_t>(secret_buf1_i64[0]);
  _exit(0);
}

extern "C" void
binsec_ct_lookup_i8()
{
  sink8 = static_cast<uint8_t>(subtle::ct_lookup<uint8_t, int8_t>(secret_x8, std::span<const int8_t, SECRET_BUF_LEN>(secret_buf1_i8)));
  _exit(0);
}
extern "C" void
binsec_ct_lookup_i16()
{
  sink16 = static_cast<uint16_t>(subtle::ct_lookup<uint16_t, int16_t>(secret_x16, std::span<const int16_t, SECRET_BUF_LEN>(secret_buf1_i16)));
  _exit(0);
}
extern "C" void
binsec_ct_lookup_i32()
{
  sink32 = static_cast<uint32_t>(subtle::ct_lookup<uint32_t, int32_t>(secret_x32, std::span<const int32_t, SECRET_BUF_LEN>(secret_buf1_i32)));
  _exit(0);
}
extern "C" void
binsec_ct_lookup_i64()
{
  sink64 = static_cast<uint64_t>(subtle::ct_lookup<uint64_t, int64_t>(secret_x64, std::span<const int64_t, SECRET_BUF_LEN>(secret_buf1_i64)));
  _exit(0);
}

// --- ct_select (x, y, and br are all secret) ---

extern "C" void
binsec_ct_select_u8()
{
  sink8 = subtle::ct_select<uint8_t, uint8_t>(secret_br8, secret_x8, secret_y8);
  _exit(0);
}
extern "C" void
binsec_ct_select_u16()
{
  sink16 = subtle::ct_select<uint16_t, uint16_t>(secret_br16, secret_x16, secret_y16);
  _exit(0);
}
extern "C" void
binsec_ct_select_u32()
{
  sink32 = subtle::ct_select<uint32_t, uint32_t>(secret_br32, secret_x32, secret_y32);
  _exit(0);
}
extern "C" void
binsec_ct_select_u64()
{
  sink64 = subtle::ct_select<uint64_t, uint64_t>(secret_br64, secret_x64, secret_y64);
  _exit(0);
}

// --- ct_swap (x, y, and br are all secret; x and y are modified in place) ---

extern "C" void
binsec_ct_swap_u8()
{
  subtle::ct_swap<uint8_t, uint8_t>(secret_br8, secret_x8, secret_y8);
  sink8 = secret_x8;
  _exit(0);
}

extern "C" void
binsec_ct_swap_u16()
{
  subtle::ct_swap<uint16_t, uint16_t>(secret_br16, secret_x16, secret_y16);
  sink16 = secret_x16;
  _exit(0);
}

extern "C" void
binsec_ct_swap_u32()
{
  subtle::ct_swap<uint32_t, uint32_t>(secret_br32, secret_x32, secret_y32);
  sink32 = secret_x32;
  _exit(0);
}

extern "C" void
binsec_ct_swap_u64()
{
  subtle::ct_swap<uint64_t, uint64_t>(secret_br64, secret_x64, secret_y64);
  sink64 = secret_x64;
  _exit(0);
}

// --- ct_swap span overload (both buffers and br are all secret; buffers modified in place) ---

extern "C" void
binsec_ct_swap_span_u8()
{
  subtle::ct_swap<uint8_t, uint8_t>(secret_br8, std::span<uint8_t, SECRET_BUF_LEN>(secret_buf1_u8), std::span<uint8_t, SECRET_BUF_LEN>(secret_buf2_u8));
  sink8 = secret_buf1_u8[0];
  _exit(0);
}

extern "C" void
binsec_ct_swap_span_u16()
{
  subtle::ct_swap<uint16_t, uint16_t>(secret_br16, std::span<uint16_t, SECRET_BUF_LEN>(secret_buf1_u16), std::span<uint16_t, SECRET_BUF_LEN>(secret_buf2_u16));
  sink16 = secret_buf1_u16[0];
  _exit(0);
}

extern "C" void
binsec_ct_swap_span_u32()
{
  subtle::ct_swap<uint32_t, uint32_t>(secret_br32, std::span<uint32_t, SECRET_BUF_LEN>(secret_buf1_u32), std::span<uint32_t, SECRET_BUF_LEN>(secret_buf2_u32));
  sink32 = secret_buf1_u32[0];
  _exit(0);
}

extern "C" void
binsec_ct_swap_span_u64()
{
  subtle::ct_swap<uint64_t, uint64_t>(secret_br64, std::span<uint64_t, SECRET_BUF_LEN>(secret_buf1_u64), std::span<uint64_t, SECRET_BUF_LEN>(secret_buf2_u64));
  sink64 = secret_buf1_u64[0];
  _exit(0);
}

// --- ct_memcmp (both buffers are secret) ---

extern "C" void
binsec_ct_memcmp_u8()
{
  sink8 = subtle::ct_memcmp<uint8_t, uint8_t>(std::span<const uint8_t, SECRET_BUF_LEN>(secret_buf1_u8), std::span<const uint8_t, SECRET_BUF_LEN>(secret_buf2_u8));
  _exit(0);
}

extern "C" void
binsec_ct_memcmp_u16()
{
  sink16 = subtle::ct_memcmp<uint16_t, uint16_t>(std::span<const uint16_t, SECRET_BUF_LEN>(secret_buf1_u16), std::span<const uint16_t, SECRET_BUF_LEN>(secret_buf2_u16));
  _exit(0);
}

extern "C" void
binsec_ct_memcmp_u32()
{
  sink32 = subtle::ct_memcmp<uint32_t, uint32_t>(std::span<const uint32_t, SECRET_BUF_LEN>(secret_buf1_u32), std::span<const uint32_t, SECRET_BUF_LEN>(secret_buf2_u32));
  _exit(0);
}

extern "C" void
binsec_ct_memcmp_u64()
{
  sink64 = subtle::ct_memcmp<uint64_t, uint64_t>(std::span<const uint64_t, SECRET_BUF_LEN>(secret_buf1_u64), std::span<const uint64_t, SECRET_BUF_LEN>(secret_buf2_u64));
  _exit(0);
}

// --- ct_conditional_memcpy (dst buffer, src buffer, and br are all secret) ---

extern "C" void
binsec_ct_conditional_memcpy_u8()
{
  subtle::ct_conditional_memcpy<uint8_t, uint8_t>(secret_br8, std::span<uint8_t, SECRET_BUF_LEN>(secret_buf1_u8), std::span<const uint8_t, SECRET_BUF_LEN>(secret_buf2_u8));
  sink8 = secret_buf1_u8[0];
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_u16()
{
  subtle::ct_conditional_memcpy<uint16_t, uint16_t>(
    secret_br16, std::span<uint16_t, SECRET_BUF_LEN>(secret_buf1_u16), std::span<const uint16_t, SECRET_BUF_LEN>(secret_buf2_u16));
  sink16 = secret_buf1_u16[0];
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_u32()
{
  subtle::ct_conditional_memcpy<uint32_t, uint32_t>(
    secret_br32, std::span<uint32_t, SECRET_BUF_LEN>(secret_buf1_u32), std::span<const uint32_t, SECRET_BUF_LEN>(secret_buf2_u32));
  sink32 = secret_buf1_u32[0];
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_u64()
{
  subtle::ct_conditional_memcpy<uint64_t, uint64_t>(
    secret_br64, std::span<uint64_t, SECRET_BUF_LEN>(secret_buf1_u64), std::span<const uint64_t, SECRET_BUF_LEN>(secret_buf2_u64));
  sink64 = secret_buf1_u64[0];
  _exit(0);
}

// --- ct_lookup (secret index into a secret table) ---

extern "C" void
binsec_ct_lookup_u8()
{
  sink8 = subtle::ct_lookup<uint8_t, uint8_t>(secret_x8, std::span<const uint8_t, SECRET_BUF_LEN>(secret_buf1_u8));
  _exit(0);
}

extern "C" void
binsec_ct_lookup_u16()
{
  sink16 = subtle::ct_lookup<uint16_t, uint16_t>(secret_x16, std::span<const uint16_t, SECRET_BUF_LEN>(secret_buf1_u16));
  _exit(0);
}

extern "C" void
binsec_ct_lookup_u32()
{
  sink32 = subtle::ct_lookup<uint32_t, uint32_t>(secret_x32, std::span<const uint32_t, SECRET_BUF_LEN>(secret_buf1_u32));
  _exit(0);
}

extern "C" void
binsec_ct_lookup_u64()
{
  sink64 = subtle::ct_lookup<uint64_t, uint64_t>(secret_x64, std::span<const uint64_t, SECRET_BUF_LEN>(secret_buf1_u64));
  _exit(0);
}

// --- ct_zeroize (buffer contents are secret) ---

extern "C" void
binsec_ct_zeroize_u8()
{
  subtle::ct_zeroize(std::span<uint8_t, SECRET_BUF_LEN>(secret_buf1_u8));
  sink8 = secret_buf1_u8[0];
  _exit(0);
}

extern "C" void
binsec_ct_zeroize_u16()
{
  subtle::ct_zeroize(std::span<uint16_t, SECRET_BUF_LEN>(secret_buf1_u16));
  sink16 = secret_buf1_u16[0];
  _exit(0);
}

extern "C" void
binsec_ct_zeroize_u32()
{
  subtle::ct_zeroize(std::span<uint32_t, SECRET_BUF_LEN>(secret_buf1_u32));
  sink32 = secret_buf1_u32[0];
  _exit(0);
}

extern "C" void
binsec_ct_zeroize_u64()
{
  subtle::ct_zeroize(std::span<uint64_t, SECRET_BUF_LEN>(secret_buf1_u64));
  sink64 = secret_buf1_u64[0];
  _exit(0);
}

// Binsec starts from the specified function, not main.
int
main()
{
  return 0;
}
