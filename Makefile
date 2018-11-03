
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

test: testbench
	./testbench 10 S2 B4 QAM16 1

testbench: testbench.cc *_tables.cc *.hh
	$(CXX) $(CXXFLAGS) testbench.cc *_tables.cc -o $@

.PHONY: clean all

clean:
	rm -f testbench

