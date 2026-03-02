# Subtle Examples

A standalone example project demonstrating how to use the [subtle](https://github.com/itzmeanjan/subtle) library.

## Quick Start

```bash
cd examples

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/ct_swap_example
```

## Using `subtle` in Your Own Project

The simplest way to integrate `subtle` into your CMake project is via `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.28)
project(my_app LANGUAGES CXX)

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
target_compile_features(my_app PRIVATE cxx_std_20)
```

Alternatively, install `subtle` system-wide and use `find_package`:

```bash
# From the subtle root directory
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

```cmake
find_package(subtle REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE subtle)
```
