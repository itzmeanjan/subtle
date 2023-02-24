# subtle
Constant-Time Comparison, Conditional Selection and Swapping of Unsigned Integer Values

## Overview

In cryptographic library implementations we care about how much information gets leaked when certain procedure ( say key generation of public key encryption (PKE) or message signing using some digital signature algorithm (DSA) ) is executed in some environment. Based on what that specific environment is, various sorts of observation tactics an adversary can deploy for collecting leaked information, which can result in partial to full recovery of secrets. We would like to write implementations which leaks as little information ( regarding secrets in our implementation ) as possible i.e. it should not be dependent on some secret input

- What intructions to be executed next, because different instructions have different latencies and it can also fail CPU branch predictor resulting in pretty expensive rewinding - and it can be observed and measured.
- Or which memory addresses to be accessed, because it can result in cache miss and that increases latency, which can also be observed and measured.
- Or what the latency of some instruction ( say integer division ) is.

> **Note** Read more about need for constant-time implementations @ https://www.bearssl.org/constanttime.html.

Remember we're mostly writing programs in some higher level programming language ( say C++ ) which is passed through the compiler for producing final executable and the compiler is free to transform it to something totally different ( may be something that we're actually trying to avoid using constant-time implementation - that's the whole point ! ) as long as it produces intended side-effect. So constant-timeness is not trivial. Read more @ https://www.chosenplaintext.ca/articles/beginners-guide-constant-time-cryptography.html.

Just so that I don't need to write same boilerplate code again and again for achieving constant-timeness, I maintain this light-weight, header-only, generic C++ library which offers following functionalities

- constant-time comparison operations
- constant-time conditional selection
- constant-time conditional swapping

over 8, 16, 32 and 64 -bit unsigned integer type. This is a best effort mechanism to achieve constant-timeness and it's not guaranteed that if you also use this, your cryptographic implementation becomes constant-time. It's always good idea to target some specific architecture, compile with debug info and then disassemble object file ( with interleaved source, yes we can do it, because we've debug info ) to inspect what did the compiler do, more [here](#usage).

> **Note** This library collects motivation from both https://github.com/dalek-cryptography/subtle and https://github.com/golang/go/blob/ddb423a7/src/crypto/subtle/constant_time.go.

## Prerequisites

- A C++ compiler such as `g++`/ `clang++` with C++20 standard library

```bash
$ clang++ --version
Apple clang version 14.0.0 (clang-1400.0.29.202)

$ g++ --version
g++ (Homebrew GCC 12.2.0) 12.2.0
```

- System development utilities such as `make`, `cmake`

```bash
$ make --version
GNU Make 3.81

$ cmake --version
cmake version 3.25.2
```

- For benchmarking implementation against baseline, `google-benchmark` headers and library must be installed globally. Follow [this](https://github.com/google/benchmark/tree/3b19d722#installation) guide.

## Testing

For ensuring that all these constant-time routines such as

- comparison operations ( s.t. `==`, `!=`, `<`, `<=`, `>` and `>=` )
- conditional selection
- conditional swapping

implemented in this C++ library, over unsigned integer types of 8, 16, 32 and 64 -bits, are functionally correct, issue

```bash
make

[test] Equality test
[test] Inequality test
[test] Conditional selection test
[test] Conditional swap test
[test] Lesser than equality test
[test] Greater than test
[test] Greater than equality test
[test] Lesser than test
```

## Benchmarking

For understanding how much expensive ( from respective operation's latency point of view ) does it become for us to use constant-time routines ( so that we don't leak much information via side-channel ) compared to non-constant-time implementations ( say using `std::swap` along with conditional blocks or ternary operators or actual relational operators on operands of type `uint{8, 16, 32, 64}_t` ), issue

```bash
make benchmark
```

> **Warning** Ensure you don't have CPU scaling enabled during benchmarking, follow [this](https://github.com/google/benchmark/blob/3b19d722/docs/user_guide.md#disabling-cpu-frequency-scaling) guide.

### On Intel(R) Core(TM) i5-8279U CPU @ 2.40GHz ( when compiled with Clang )

```bash
2023-02-23T19:45:35+04:00
Running ./bench/a.out
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB
  L1 Instruction 32 KiB
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB
Load Average: 2.87, 2.32, 2.04
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
bench_subtle::ct_eq<uint8_t>               0.685 ns        0.685 ns    956977046
bench_subtle::native_eq<uint8_t>           0.748 ns        0.747 ns    932289169
bench_subtle::ct_eq<uint16_t>              0.689 ns        0.686 ns   1000000000
bench_subtle::native_eq<uint16_t>          0.994 ns        0.993 ns    696496622
bench_subtle::ct_eq<uint32_t>              0.992 ns        0.991 ns    686160150
bench_subtle::native_eq<uint32_t>          0.992 ns        0.991 ns    694885642
bench_subtle::ct_eq<uint64_t>              0.999 ns        0.997 ns    670767933
bench_subtle::native_eq<uint64_t>          0.995 ns        0.994 ns    698275260
bench_subtle::ct_ne<uint8_t>               0.683 ns        0.683 ns   1000000000
bench_subtle::native_ne<uint8_t>           0.745 ns        0.744 ns    930937720
bench_subtle::ct_ne<uint16_t>              0.682 ns        0.681 ns    994600739
bench_subtle::native_ne<uint16_t>          0.992 ns        0.991 ns    704076604
bench_subtle::ct_ne<uint32_t>              0.994 ns        0.993 ns    693446927
bench_subtle::native_ne<uint32_t>          0.991 ns        0.990 ns    685938266
bench_subtle::ct_ne<uint64_t>              0.993 ns        0.993 ns    692842933
bench_subtle::native_ne<uint64_t>          0.992 ns        0.991 ns    696268004
bench_subtle::ct_select<uint8_t>           0.804 ns        0.803 ns    849483635
bench_subtle::native_select<uint8_t>       0.744 ns        0.743 ns    922497068
bench_subtle::ct_select<uint16_t>          0.755 ns        0.755 ns    866894536
bench_subtle::native_select<uint16_t>      0.622 ns        0.622 ns   1000000000
bench_subtle::ct_select<uint32_t>          0.625 ns        0.624 ns   1000000000
bench_subtle::native_select<uint32_t>      0.623 ns        0.622 ns   1000000000
bench_subtle::ct_select<uint64_t>           1.24 ns         1.23 ns    560237541
bench_subtle::native_select<uint64_t>      0.995 ns        0.994 ns    693495017
bench_subtle::ct_swap<uint8_t>              2.01 ns         2.01 ns    344244236
bench_subtle::native_swap<uint8_t>          1.33 ns         1.33 ns    521827290
bench_subtle::ct_swap<uint16_t>             2.00 ns         2.00 ns    347342827
bench_subtle::native_swap<uint16_t>         1.16 ns         1.16 ns   1000000000
bench_subtle::ct_swap<uint32_t>             1.99 ns         1.99 ns    347891776
bench_subtle::native_swap<uint32_t>        0.499 ns        0.498 ns   1000000000
bench_subtle::ct_swap<uint64_t>             2.00 ns         2.00 ns    347610180
bench_subtle::native_swap<uint64_t>        0.507 ns        0.506 ns   1000000000
bench_subtle::ct_le<uint8_t>                2.19 ns         2.19 ns    317895712
bench_subtle::native_le<uint8_t>           0.742 ns        0.741 ns    903435637
bench_subtle::ct_le<uint16_t>               2.59 ns         2.59 ns    265408881
bench_subtle::native_le<uint16_t>          0.747 ns        0.747 ns    919214203
bench_subtle::ct_le<uint32_t>               2.99 ns         2.99 ns    231759685
bench_subtle::native_le<uint32_t>          0.623 ns        0.622 ns   1000000000
bench_subtle::ct_le<uint64_t>               3.20 ns         3.20 ns    218581283
bench_subtle::native_le<uint64_t>          0.619 ns        0.618 ns   1000000000
bench_subtle::ct_gt<uint8_t>                2.17 ns         2.17 ns    317745640
bench_subtle::native_gt<uint8_t>           0.990 ns        0.990 ns    702233101
bench_subtle::ct_gt<uint16_t>               2.59 ns         2.59 ns    269109670
bench_subtle::native_gt<uint16_t>          0.561 ns        0.561 ns   1000000000
bench_subtle::ct_gt<uint32_t>               2.73 ns         2.73 ns    255682545
bench_subtle::native_gt<uint32_t>          0.992 ns        0.991 ns    700210063
bench_subtle::ct_gt<uint64_t>               3.23 ns         3.22 ns    215385278
bench_subtle::native_gt<uint64_t>          0.997 ns        0.996 ns    700539415
bench_subtle::ct_ge<uint8_t>                2.99 ns         2.99 ns    229340515
bench_subtle::native_ge<uint8_t>           0.742 ns        0.742 ns    913826190
bench_subtle::ct_ge<uint16_t>               3.00 ns         2.99 ns    234277472
bench_subtle::native_ge<uint16_t>          0.746 ns        0.746 ns    929392708
bench_subtle::ct_ge<uint32_t>               2.88 ns         2.88 ns    239845950
bench_subtle::native_ge<uint32_t>          0.622 ns        0.621 ns   1000000000
bench_subtle::ct_ge<uint64_t>               3.48 ns         3.48 ns    202006210
bench_subtle::native_ge<uint64_t>          0.619 ns        0.618 ns   1000000000
bench_subtle::ct_lt<uint8_t>                2.98 ns         2.97 ns    233947054
bench_subtle::native_lt<uint8_t>           0.990 ns        0.989 ns    700890130
bench_subtle::ct_lt<uint16_t>               2.77 ns         2.77 ns    253012658
bench_subtle::native_lt<uint16_t>          0.564 ns        0.564 ns   1000000000
bench_subtle::ct_lt<uint32_t>               3.24 ns         3.23 ns    217284686
bench_subtle::native_lt<uint32_t>          0.993 ns        0.993 ns    691405826
bench_subtle::ct_lt<uint64_t>               3.35 ns         3.35 ns    206227481
bench_subtle::native_lt<uint64_t>          0.995 ns        0.995 ns    697113948
```

## Usage

`subtle` is a light-weight, header-only, generic ( templated over types `uint{8, 16, 32 64}_t` ) C++ library, which offers constant-time implementation of following functionalities, which are of interest, generally in cryptographic context.

- Comparison operations with operands of type `uint{8, 16, 32, 64}_t`
- Conditional selection operation s.t. `cond ? val0 : val1`
- Conditional swap operation s.t. `if (cond) { std::swap(val0, val1); }`

> **Note** `subtle` being a template library you can ( or need to ) specify what type of operands you're working with and what type of result you want from these constant-time functions.

Say for example, you've two values of type `uint8_t` held in variables $a$, $b$ and you want to compare their values to see if $a$ equals to $b$ or not. You can simply write following

```cpp
uint8_t a, b;

std::cin >> a;
std::cin >> b;

const bool flg = a == b;
```

Which is perfectly fine, but in constant-time implementation, we want to avoid using relational operators ( resulting in boolean value ) and only rely on integer addition, subtraction and bit-wise operations. It's because whenever booleans are involved there is high chance that it can be transformed into some conditional jump, which can result in instruction cache miss and it can be measured. While if we can only rely on constant-time instructions ( say integer addition, subtraction or bit-wise operations etc. ) laid next to each other in instruction cache, all the intructions will be executed in order and final result will be acccumulated as some integer value and hopefully compiler won't be able to recognize and optimize it away. We can write

```cpp
const uint8_t flg = subtle::ct_eq<uint8_t, uint8_t>(a, b);
```

With above implementation, we get flg = 0xff, in case $a == b$, otherwise flg = 0x00.

Constant-time implementation becomes more practical and interesting, when say we've to compare a set to values to another set to values to find whether they are same or not. Let's say we've two byte arrays $a$, $b$ ( of same length, for sake of simplicity ) and we want to compare them to figure if $a == b$. Now we want to keep a few things in mind,

- Don't decide which code path to take ( ofcourse in runtime ) based on contents of byte arrays $a$ or $b$.
- Don't decide which memory addresses to access based on contents of byte arrays $a$ or $b$.

That's because these two byte arrays are holding secret data. If we do any of above mentioned things, during the comparison, then it can leak some information about content of those byte arrays to some adversary who might be observing. There are various possible ways to collect these sort of leaked information in various different execution environments.

So we can write

```cpp
uint8_t arr_a[16];
uint8_t arr_b[16];

// fill arr_a, arr_b with some content

bool flg = true;

for(size_t i = 0; i < 16; i++) {
    if (arr_a[i] != arr_b[i]) { 
        flg = false;
        break;
    }
}
```

But you understand why it's not something we want to do in cryptographic context. We're exactly deciding when to break out of the loop ( i.e. which code path to take or which instructions to execute next ) based on content of byte arrays $a$ and $b$.

Let's make it better

```cpp

bool flg = false;

for(size_t i = 0; i < 16; i++) {
    flg |= static_cast<bool>(arr_a[i] ^ arr_b[i]);
}

const bool is_a_match = !flg;
```

Now we're doing it better, because it doesn't matter what the content of those two byte arrays are, we always execute same set to instructions.

Now $flg$, in above code snippet, is a secret boolean, because it holds accumulated result of comparison of two secret bytearrays $a$, $b$.

Now we write following, using $flg$

```cpp
if (flg) {
    std::memcpy(dst, src, 16);
} else {
    std::memset(dst, 0, 16);
}

// Or fancier alternative
// Generally results into conditional move intructions

std::memcpy(dst, src, 16 * flg);
std::memset(dst, 0, 16 * !flg);
```

We're branching out ( i.e. deciding which instruction to execute ) based on secret value $flg$. 

We can avoid this using `subtle`. First let us perform comparison of two secret bytearrays.

```cpp
uint32_t flg = -1u; // = 0xffffffff

for(size_t i = 0; i < 16; i++) {
    flg &= subtle::ct_eq<uint8_t, uint32_t>(arr_a[i], arr_b[i]);
}
```

If content of those two bytearrays match, flg = 0xffffffff, otherwise flg = 0. 

> **Note** You may want `flg` to be `uint64_t`, in that case truth value is denoted by 0xffffffffffffffff and false value by 0. Read API documentation in [subtle.hpp](./include/subtle.hpp).

Again $flg$ is a secret piece of data ( holds accumulated result of secret comparison ) and we can't branch on its value. We can use `subtle` for conditionally selecting ( condition is truthness or falseness of value held in $flg$ ) what value to write in $dst$, in constant-time.

```cpp
for(size_t i = 0; i < 16; i++) {
    dst[i] = subtle::ct_select<uint32_t, uint8_t>(flg, src[i], 0);
}
```

I maintain a few examples, where I demonstrate how you can use `subtle` API for writing constant-time code.

- Comparison of two byte arrays @ [here](./example/ct_compare.cpp)
- Conditional copying of byte array @ [here](./example/ct_copy.cpp)
- Conditional swapping of two values @ [here](./example/ct_swap.cpp)

For more examples, try going through [test cases](./include/test_subtle.hpp).

---

A note on `subtle` - almost all of the functions are `constexpr` i.e. we get to enjoy some compile-time computation benefits. It may not be very helpful for real-world cryptographic use cases, but can help in writing `static_assert` based test cases.
