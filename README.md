# subtle

Constant-time comparison, conditional selection and conditional swapping of unsigned integer values. C++20, header-only, fully `constexpr` library.

> [!NOTE]
> `constexpr` ? Yes, all functions can be evaluated at compile-time — useful for `static_assert` based test cases.

## Quick Start

Add `subtle` to your CMake project via `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
  subtle
  GIT_REPOSITORY https://github.com/itzmeanjan/subtle.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(subtle)

target_link_libraries(my_app PRIVATE subtle)
```

Compare two byte arrays in constant-time:

```cpp
#include "subtle.hpp"
#include <array>
#include <cstdint>

bool
ct_memcmp(const std::array<uint8_t, 16>& a, const std::array<uint8_t, 16>& b)
{
  uint32_t result = ~uint32_t{ 0 }; // all bits set = true
  for (size_t i = 0; i < a.size(); i++) {
    result &= subtle::ct_eq<uint8_t, uint32_t>(a[i], b[i]);
  }

  return result != 0;
}
```

`std::memcmp` would short-circuit, as early as possible. So it is data-dependent. We want to explicitly avoid being data-dependent. Because it can be used for comparing two secret material strings. We want to compare two secret materials such that it always takes exactly same time. The secret material values must not affect timing of comparison. If your usecase requires you to achieve this, you should rather use `ct_memcmp`.

See [examples/](./examples/) for complete standalone examples.

## Overview

In cryptographic library implementations we care about how much information gets leaked when certain procedure, which deals with secret key, is executed in some environment. It can be key generation of public key encryption (PKE) or message signing using some digital signature algorithm (DSA). Based on what that specific environment is, various sorts of observation tactics an adversary can deploy for collecting leaked information. It can result in partial or full recovery of secret materials. We would like to write implementations which leaks as little secret information as possible. Simply put, we do **not** want to do following based on value of secret key material.

- What instructions to be executed next, because different instructions have different latencies and it can also fail CPU branch predictor resulting in pretty expensive rewinding - and it can be observed and measured.
- Which memory addresses to be accessed, because it can result in cache miss and that increases latency, which can also be observed and measured.
- What the latency of some instruction ( say integer division ) is.

> [!NOTE]
> Read more about need for constant-time implementations @ <https://www.bearssl.org/constanttime.html>.

We mostly write programs in some high-level language, say C++. That program is passed down to a compiler for producing machine executable instructions. The compiler is free to transform it to something totally different, as long as it produces intended observable side-effects. It may even produce some instructions that we're explicitly trying to avoid, by using this whole "constant-time" implementation. So achieving constant-timeness is not trivial. Read more @ <https://www.chosenplaintext.ca/articles/beginners-guide-constant-time-cryptography.html>.

I don't want to write same boilerplate code again and again for achieving constant-timeness. That's why I maintain this minimal, header-only, portable, fully `constexpr` C++ library which offers following functionalities.

- Constant-time comparison operations (`==`, `!=`, `<`, `>`, `<=`, `>=`)
- Constant-time conditional selection
- Constant-time conditional swapping

These operations work over `uint8_t`, `uint16_t`, `uint32_t` and `uint64_t`. This is a best effort mechanism to achieve constant-timeness and it's not guaranteed that if you use this, your cryptographic implementation becomes constant-time. It's always good idea to target some specific architecture, compile with debug info and then disassemble object file, with interleaved source code lines, to inspect what the compiler generated.

> [!NOTE]
> This library collects motivation from both <https://github.com/dalek-cryptography/subtle> and <https://github.com/golang/go/blob/ddb423a7/src/crypto/subtle/constant_time.go>.

## Prerequisites

- A C++ compiler such as `clang++`/ `g++`, with support for compiling C++20 programs.
- CMake 3.28 or later.
- For testing, `google-test` is required. It can be installed globally or fetched automatically by setting `-DSUBTLE_FETCH_DEPS=ON`.
- For benchmarking, `google-benchmark` is required. It can be installed globally or fetched automatically by setting `-DSUBTLE_FETCH_DEPS=ON`.
- For static analysis, you'll need `clang-tidy`.
- For code formatting, you'll need `clang-format`.

## Building

### CMake Options

| Option | Description | Default |
| :--- | :--- | :--- |
| `SUBTLE_BUILD_TESTS` | Build tests | `OFF` |
| `SUBTLE_BUILD_BENCHMARKS` | Build benchmarks | `OFF` |
| `SUBTLE_BUILD_EXAMPLES` | Build examples | `OFF` |
| `SUBTLE_FETCH_DEPS` | Fetch missing dependencies (GTest, Benchmark) | `OFF` |
| `SUBTLE_ASAN` | Enable AddressSanitizer | `OFF` |
| `SUBTLE_UBSAN` | Enable UndefinedBehaviorSanitizer | `OFF` |
| `SUBTLE_NATIVE_OPT` | Enable `-march=native` (not suitable for cross-compilation) | `OFF` |
| `SUBTLE_ENABLE_LTO` | Enable Interprocedural Optimization (LTO) | `ON` |

> [!NOTE]
> `SUBTLE_ASAN`, `SUBTLE_UBSAN`, `SUBTLE_NATIVE_OPT` and `SUBTLE_ENABLE_LTO` are only available when building `subtle` as the top-level project. They are not exposed when consumed via `FetchContent` or `add_subdirectory`.

### Testing

To build and run the tests, use the following CMake commands:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSUBTLE_BUILD_TESTS=ON -DSUBTLE_FETCH_DEPS=ON
cmake --build build -j

ctest --test-dir build -j --output-on-failure
```

To run tests with sanitizers, reconfigure the build with the desired sanitizer option:

```bash
# With AddressSanitizer, in Release mode
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSUBTLE_BUILD_TESTS=ON -DSUBTLE_FETCH_DEPS=ON -DSUBTLE_ASAN=ON
cmake --build build -j
ctest --test-dir build -j --output-on-failure

# With UndefinedBehaviorSanitizer, in Release mode
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSUBTLE_BUILD_TESTS=ON -DSUBTLE_FETCH_DEPS=ON -DSUBTLE_UBSAN=ON
cmake --build build -j
ctest --test-dir build -j --output-on-failure

# With Clang, explicitly set, in Debug mode
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DSUBTLE_BUILD_TESTS=ON -DSUBTLE_FETCH_DEPS=ON -DSUBTLE_UBSAN=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build -j
ctest --test-dir build -j --output-on-failure
```

### Benchmarking

To run the benchmarks (using Google Benchmark):

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSUBTLE_BUILD_BENCHMARKS=ON -DSUBTLE_FETCH_DEPS=ON -DSUBTLE_NATIVE_OPT=ON
cmake --build build -j
```

> [!CAUTION]
> When benchmarking, ensure that you've disabled CPU frequency scaling, by following guide @ <https://github.com/google/benchmark/blob/main/docs/reducing_variance.md>.

Each benchmark can be run individually using `--benchmark_filter` and results can be exported to JSON for comparison. For example, to benchmark `memcmp`:

```bash
./build/subtle_benchmarks \
  --benchmark_min_warmup_time=.05 \
  --benchmark_enable_random_interleaving=true \
  --benchmark_repetitions=10 \
  --benchmark_min_time=0.1s \
  --benchmark_display_aggregates_only=true \
  --benchmark_report_aggregates_only=true \
  --benchmark_out_format=json \
  --benchmark_out=memcmp.json \
  --benchmark_counters_tabular=true \
  --benchmark_filter='memcmp'
```

Replace `memcmp` with `conditional_copy` or `conditional_swap` (and the output filename) to benchmark the other operations.

To compare native vs. constant-time performance, use Google Benchmark's `compare.py` tool:

```bash
pushd /path/to/benchmark/tools/
python3 -m venv .venv                       # First time, setup virtual environment
source .venv/bin/activate                   # Once setup, just enable it
python -m pip install -r requirements.txt   # Also first time

python compare.py filters memcmp.json native_memcmp ct_memcmp

popd
```

#### Benchmark Comparison

native `std::memcmp` vs. Constant-time memcmp — `std::memcmp` is heavily SIMD-optimized, so the constant-time byte-by-byte loop pays a significant overhead, especially at larger buffer sizes.

![benchcmp_memcmp](./benchcmp_memcmp.png)

native branching copy vs. Constant-time conditional copy — the constant-time version always touches every byte via `ct_select`, while the native version skips the copy entirely when the branch is false.

![benchcmp_conditional_copy](./benchcmp_conditional_copy.png)

native branching swap vs. Constant-time conditional swap — similar to copy, the constant-time version always performs the swap work regardless of the condition, eliminating the branch but costing throughput.

![benchcmp_conditional_swap](./benchcmp_conditional_swap.png)

### Integration

You can easily integrate `subtle` into your project using CMake.

```bash
# Install system-wide (default prefix: /usr/local)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build

# Install to custom directory (e.g., ./dist)
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./dist
cmake --build build
cmake --install build
```

Or using `FetchContent` in your `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
  subtle
  GIT_REPOSITORY https://github.com/itzmeanjan/subtle.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(subtle)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE subtle)
```

### Development Tools

> [!NOTE]
> The `tidy` and `format` targets are only registered if `clang-tidy` / `clang-format` are found at configure time. If you install them after configuring, re-run the `cmake -B build ...` step.

```bash
# Configure
cmake -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release -DSUBTLE_BUILD_TESTS=ON -DSUBTLE_BUILD_EXAMPLES=ON -DSUBTLE_FETCH_DEPS=ON

# Static analysis (requires clang-tidy)
cmake --build build --target tidy

# Format source code (requires clang-format)
cmake --build build --target format
```

## Usage

`subtle` is a minimal, header-only, generic (templated over `uint8_t`, `uint16_t`, `uint32_t` and `uint64_t`), fully `constexpr` C++ library, which offers constant-time implementation of following functionalities. These are of great interest in high-assurance cryptographic software development context.

- Comparison operations (`==`, `!=`, `<`, `>`, `<=`, `>=`) with operands of type `uint{8, 16, 32, 64}_t`
- Conditional selection operation s.t. `cond ? val0 : val1`
- Conditional swap operation s.t. `if (cond) { std::swap(val0, val1); }`

> [!NOTE]
> `subtle` being a template library you can ( or need to ) specify what type of operands you're working with and what type of result you want from these constant-time functions. These can be any combination of `uint8_t`, `uint16_t`, `uint32_t` and `uint64_t`.

Say for example, you've two values of type `uint8_t` held in variables $a$, $b$. You want to compare their values to see if $a$ equals $b$ or not. You can simply write following.

```cpp
uint8_t a, b;

std::cin >> a;
std::cin >> b;

const bool flg = a == b;
```

It is perfectly fine, if you do not need to care about constant-timeness. In cryptographic context, when we deal with secret key materials, we want to avoid using relational operators, resulting in boolean value. We want to only rely on integer addition, subtraction and bit-wise operations. It's because whenever booleans are involved there is high chance that it can be transformed into some conditional operation. It can result in instruction cache miss. It can be measured by an adversary. An attack vector. While if we can only rely on constant-time instructions, such as integer addition, subtraction or bit-wise operations etc. laid next to each other in instruction cache - all the intructions will be executed in-order. No skipping. Final result will be acccumulated as some integer value and hopefully compiler won't be able to recognize and optimize it away. We can write.

```cpp
const uint8_t flag = subtle::ct_eq<uint8_t, uint8_t>(a, b);
```

With above implementation, we get `flag = 0xff`, in case $a == b$, otherwise `flag = 0x00`.

Constant-time implementation becomes more practical and interesting, when we've to compare a set to values to another set to values to find whether they are same or not. Let's say we've two byte arrays $a$, $b$ of equal length. We want to compare them to figure if $a == b$. Now we want to keep a few things in mind.

- Must **not** decide which code path to take based on contents of byte arrays $a$ or $b$.
- Must **not** decide which memory addresses to access based on contents of byte arrays $a$ or $b$.

Say, those two byte arrays are holding secret data. If we **do** any of the above mentioned things during the comparison, it can leak information about content of those byte arrays to some adversary who might be observing. There are various possible ways to collect these leaked information in various different execution environments.

We start with.

```cpp
uint8_t arr_a[16];
uint8_t arr_b[16];

// Fill arr_a, arr_b with some secret key material

bool flag = true;

for(size_t i = 0; i < 16; i++) {
    if (arr_a[i] != arr_b[i]) {
        flag = false;
        break;
    }
}
```

We do **not** want to do this in cryptographic context. We're deciding when to break out of the loop ( i.e. which code path to take or which instructions to execute next ) based on content of byte arrays $a$ and $b$, which are filled with secret key material.

Let's make it better.

```cpp

bool flag = false;

for(size_t i = 0; i < 16; i++) {
    flag |= static_cast<bool>(arr_a[i] ^ arr_b[i]);
}

const bool is_a_match = !flag;
```

Now we're doing it better, because it doesn't matter what the content of those two byte arrays are, we always execute same set to instructions. $flag$, in above code snippet, is a "secret boolean", because it holds accumulated result of comparing two secret bytearrays $a$, $b$.

Now we write following, using $flag$.

```cpp
if (flag) {
    std::memcpy(dst, src, 16);
} else {
    std::memset(dst, 0, 16);
}

// Or fancier alternative
// Generally results into conditional move intructions

std::memcpy(dst, src, 16 * flag);
std::memset(dst, 0, 16 * !flag);
```

We're branching out ( i.e. deciding which instruction to execute ) based on secret boolean value $flag$.
We can avoid this using `subtle`. First let us perform comparison of two secret bytearrays.

```cpp
uint32_t flag = ~uint32_t {0}; // = 0xffffffff

for(size_t i = 0; i < 16; i++) {
    flag &= subtle::ct_eq<uint8_t, uint32_t>(arr_a[i], arr_b[i]);
}
```

If content of those two bytearrays match, `flag = 0xffffffff`, else `flag = 0`.

> [!NOTE]
> You may want `flag` to be of type `uint64_t`. In that case truth value is denoted by `0xffffffffffffffff` and false value by `0`. Read API documentation in [subtle.hpp](./include/subtle.hpp).

Again $flag$ is a secret piece of data. We can't branch on its value. We can use `subtle` for conditionally selecting ( condition is truthness or falseness of the value held in $flag$ ) what value to write in $dst$, in constant-time.

```cpp
for(size_t i = 0; i < 16; i++) {
    dst[i] = subtle::ct_select<uint32_t, uint8_t>(flag, src[i], 0);
}
```

I maintain a few examples, where I demonstrate how you can use `subtle` API for writing constant-time code.

- [Comparison of two byte arrays](./examples/ct_compare.cpp)
- [Conditional copying of byte array](./examples/ct_copy.cpp)
- [Conditional swapping of two values](./examples/ct_swap.cpp)

For more examples, try going through [test cases](./tests/).

---

All functions in `subtle` are `constexpr` i.e. we get to enjoy compile-time computation benefits. It may not be very helpful for real-world cryptographic use cases. As we want constant-timeness when executing in runtime. But it can help in writing `static_assert` based test cases.
