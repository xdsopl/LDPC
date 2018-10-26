
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

test: testbench
	./testbench 10 B4 QAM16

testbench: testbench.cc dvb_s2_tables.cc *.hh
	$(CXX) $(CXXFLAGS) testbench.cc dvb_s2_tables.cc -o $@

.PHONY: clean all

clean:
	rm -f testbench

