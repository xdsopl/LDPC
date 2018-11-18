
CXXFLAGS = -std=c++17 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

test: testbench
	./testbench 10 T2 A1 QAM16 32

testbench: testbench.cc *_tables.cc *.hh
	$(CXX) $(CXXFLAGS) testbench.cc *_tables.cc -o $@

.PHONY: clean all

clean:
	rm -f testbench

