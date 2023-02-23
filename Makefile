CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -march=native -mtune=native
IFLAGS = -I ./include

all: testing

test/a.out: test/main.cpp include/*.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $< -o $@

testing: test/a.out
	./$<

bench/a.out: bench/main.cpp include/*.hpp
	# make sure you've google-benchmark globally installed;
	# see https://github.com/google/benchmark/tree/84c71fa#installation
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $< -o $@ -lbenchmark

benchmark: bench/a.out
	./$<

clean:
	find . -name '*.out*' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf

format:
	find . -name '*.hpp' -o -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla
