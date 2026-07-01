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
  subtle::ct_conditional_memcpy<uint16_t, uint16_t>(secret_br16, std::span<uint16_t, SECRET_BUF_LEN>(secret_buf1_u16), std::span<const uint16_t, SECRET_BUF_LEN>(secret_buf2_u16));
  sink16 = secret_buf1_u16[0];
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_u32()
{
  subtle::ct_conditional_memcpy<uint32_t, uint32_t>(secret_br32, std::span<uint32_t, SECRET_BUF_LEN>(secret_buf1_u32), std::span<const uint32_t, SECRET_BUF_LEN>(secret_buf2_u32));
  sink32 = secret_buf1_u32[0];
  _exit(0);
}

extern "C" void
binsec_ct_conditional_memcpy_u64()
{
  subtle::ct_conditional_memcpy<uint64_t, uint64_t>(secret_br64, std::span<uint64_t, SECRET_BUF_LEN>(secret_buf1_u64), std::span<const uint64_t, SECRET_BUF_LEN>(secret_buf2_u64));
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
