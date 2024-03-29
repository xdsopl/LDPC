
CXXFLAGS = -std=c++17 -W -Wall -O2 -fno-exceptions -fno-rtti -ffast-math -ftree-vectorize

CXX = clang++ -stdlib=libc++ -march=native

#CXX = g++ -march=native

#CXX = armv7a-hardfloat-linux-gnueabi-g++ -static -mfpu=neon -march=armv7-a
#QEMU = qemu-arm

#CXX = aarch64-unknown-linux-gnu-g++ -static -march=armv8-a+crc+simd -mtune=cortex-a72
#QEMU = qemu-aarch64

test: testbench
	$(QEMU) ./testbench 10 T2 A1 QAM16 32

testbench: testbench.cc tables_handler.o itls_handler.o mods_handler.o *.hh Makefile
	$(CXX) $(CXXFLAGS) testbench.cc tables_handler.o itls_handler.o mods_handler.o -o $@

tables_handler.o: tables_handler.cc *_tables.hh ldpc.hh Makefile
	$(CXX) $(CXXFLAGS) tables_handler.cc -c -o $@

itls_handler.o: itls_handler.cc testbench.hh interleaver.hh Makefile
	$(CXX) $(CXXFLAGS) itls_handler.cc -c -o $@

mods_handler.o: mods_handler.cc testbench.hh qam.hh psk.hh modulation.hh Makefile
	$(CXX) $(CXXFLAGS) mods_handler.cc -c -o $@

.PHONY: clean all

clean:
	rm -f testbench *.o

