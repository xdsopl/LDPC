
CXXFLAGS = -stdlib=libc++ -std=c++17 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++

test: testbench
	./testbench 10

testbench: testbench.cc *.hh
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean all

clean:
	rm -f testbench

