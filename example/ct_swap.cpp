#include "subtle.hpp"
#include <iostream>
#include <random>

// Demonstrates how to use subtle API for swapping two values held in two values
// ( say a, b of type uint64_t ), based on value held in a conditional variable
// br. This conditional variable represents truth value using all bits set to 1
// and false value using all bits set to 0.
//
// Compile and run using
//
// g++ -std=c++20 -Wall -O3 -march=native -I include/ example/ct_swap.cpp
//
// For checking what code is generated for conditional swap function, issue
//
// g++ -std=c++20 -Wall -O3 -march=native -I include/ -g3 -c example/ct_swap.cpp
// objdump -dwCS -Mintel ct_swap.o

int
main()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis;

  // generate two random 64 -bit unsigned integers
  uint64_t a = dis(gen);
  uint64_t b = dis(gen);

  // generate random branch condition, based on that it'll be decided whether to
  // swap values of a, b or not
  const uint32_t br = -static_cast<uint32_t>(dis(gen) & 1);

  std::cout << br << "\t" << a << ", " << b << "\n";
  subtle::ct_swap(br, a, b);
  std::cout << br << "\t" << a << ", " << b << "\n";

  return 0;
}
