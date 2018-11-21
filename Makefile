
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti

CXX = clang++ -stdlib=libc++ -march=native

#CXX = g++ -march=native

#CXX = armv7a-hardfloat-linux-gnueabi-g++ -static -mfpu=neon -march=armv7-a
#QEMU = qemu-arm

test: testbench
	$(QEMU) ./testbench 10 T2 A1 QAM16 32

testbench: testbench.cc *_tables.cc *.hh Makefile
	$(CXX) $(CXXFLAGS) testbench.cc *_tables.cc -o $@

.PHONY: clean all

clean:
	rm -f testbench

