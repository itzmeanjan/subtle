cmake_minimum_required(VERSION 3.28)
project(subtle-example LANGUAGES CXX)

# --- Fetch subtle ---
# If subtle is installed on the system, find_package will find it.
# Otherwise, fetch it from the parent directory (or a remote repository).
find_package(subtle QUIET)

if(NOT subtle_FOUND)
  message(STATUS "subtle not found via find_package, using FetchContent...")
  include(FetchContent)
  FetchContent_Declare(
    subtle
    # Option A: For local development, point to the parent directory:
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
    # Option B: For production use, point to the GitHub repository:
    #
    # GIT_REPOSITORY https://github.com/itzmeanjan/subtle.git
    # GIT_TAG master
    # GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(subtle)
endif()

# --- Example Executable ---
add_executable(ct_swap_example ct_swap.cpp)
target_link_libraries(ct_swap_example PRIVATE subtle)
target_compile_features(ct_swap_example PRIVATE cxx_std_20)
