
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

test: testbench
	./testbench 10

testbench: testbench.cc *.hh
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean all

clean:
	rm -f testbench

