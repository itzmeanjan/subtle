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

// Binsec starts from the specified function, not main.
int
main()
{
  return 0;
}
