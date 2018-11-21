/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <cassert>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <functional>
#include <complex>
#include "psk.hh"
#include "qam.hh"
#include "ldpc.hh"
#include "generic.hh"
#include "interleaver.hh"
#include "modulation.hh"
#include "testbench.hh"

#ifdef __AVX2__
#include "avx2.hh"
#endif

#ifdef __ARM_NEON__
#include "neon.hh"
#endif

LDPCInterface *create_ldpc(char *standard, char prefix, int number);
Interleaver<code_type> *create_interleaver(char *modulation, char *standard, char prefix, int number);

template <typename TYPE, typename CODE, int LEN>
ModulationInterface<TYPE, CODE> *create_modulation(char *name)
{
	if (!strcmp(name, "BPSK"))
		return new Modulation<PhaseShiftKeying<2, TYPE, CODE>, LEN>();
	if (!strcmp(name, "QPSK"))
		return new Modulation<PhaseShiftKeying<4, TYPE, CODE>, LEN / 2>();
	if (!strcmp(name, "8PSK"))
		return new Modulation<PhaseShiftKeying<8, TYPE, CODE>, LEN / 3>();
	if (!strcmp(name, "QAM16"))
		return new Modulation<QuadratureAmplitudeModulation<16, TYPE, CODE>, LEN / 4>();
	if (!strcmp(name, "QAM64"))
		return new Modulation<QuadratureAmplitudeModulation<64, TYPE, CODE>, LEN / 6>();
	if (!strcmp(name, "QAM256"))
		return new Modulation<QuadratureAmplitudeModulation<256, TYPE, CODE>, LEN / 8>();
	if (!strcmp(name, "QAM1024"))
		return new Modulation<QuadratureAmplitudeModulation<1024, TYPE, CODE>, LEN / 10>();
	return 0;
}

template <typename TYPE, typename CODE>
ModulationInterface<TYPE, CODE> *create_modulation(char *name, int len)
{
	switch (len) {
	case 16200:
		return create_modulation<TYPE, CODE, 16200>(name);
	case 32400:
		return create_modulation<TYPE, CODE, 32400>(name);
	case 64800:
		return create_modulation<TYPE, CODE, 64800>(name);
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 6)
		return -1;

	//typedef NormalUpdate<simd_type> update_type;
	typedef SelfCorrectedUpdate<simd_type> update_type;

	typedef MinSumAlgorithm<simd_type, update_type> algorithm_type;
	//typedef MinSumCAlgorithm<simd_type, update_type, FACTOR> algorithm_type;
	//typedef LogDomainSPA<simd_type, update_type> algorithm_type;
	//typedef LambdaMinAlgorithm<simd_type, update_type, 3> algorithm_type;
	//typedef SumProductAlgorithm<simd_type, update_type> algorithm_type;

	LDPCInterface *ldpc = create_ldpc(argv[2], argv[3][0], atoi(argv[3]+1));
	if (!ldpc) {
		std::cerr << "no such table!" << std::endl;
		return -1;
	}
	const int CODE_LEN = ldpc->code_len();
	const int DATA_LEN = ldpc->data_len();
	std::cerr << "testing LDPC(" << CODE_LEN << ", " << DATA_LEN << ") code." << std::endl;

	ModulationInterface<complex_type, code_type> *mod = create_modulation<complex_type, code_type>(argv[4], CODE_LEN);
	if (!mod) {
		std::cerr << "no such modulation!" << std::endl;
		return -1;
	}
	const int MOD_BITS = mod->bits();
	assert(CODE_LEN % MOD_BITS == 0);
	const int SYMBOLS = CODE_LEN / MOD_BITS;

	Interleaver<code_type> *itl = create_interleaver(argv[4], argv[2], argv[3][0], atoi(argv[3]+1));
	assert(itl);

	value_type SNR = atof(argv[1]);
	value_type mean = 1;
	value_type sigma = std::sqrt(mean * mean / (2 * std::pow(10, SNR / 10)));
	//value_type SNR = 10 * std::log10(mean * mean / (2 * sigma * sigma));
	std::cerr << SNR << " Es/N0 => standard deviation of " << sigma << " with mean " << mean << std::endl;

	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> uniform;
	typedef std::normal_distribution<value_type> normal;
	auto data = std::bind(uniform(0, 1), generator);
	auto awgn = std::bind(normal(0.0, sigma), generator);

	int BLOCKS = atoi(argv[5]);
	if (BLOCKS < 1)
		return -1;
	void *aligned_buffer = aligned_alloc(sizeof(simd_type), sizeof(simd_type) * CODE_LEN);
	simd_type *simd = reinterpret_cast<simd_type *>(aligned_buffer);
	code_type *code = new code_type[BLOCKS * CODE_LEN];
	code_type *orig = new code_type[BLOCKS * CODE_LEN];
	code_type *noisy = new code_type[BLOCKS * CODE_LEN];
	complex_type *symb = new complex_type[BLOCKS * SYMBOLS];

	for (int j = 0; j < BLOCKS; ++j)
		for (int i = 0; i < DATA_LEN; ++i)
			code[j * CODE_LEN + i] = 1 - 2 * data();

	LDPCEncoder<code_type> encode(ldpc);
	for (int j = 0; j < BLOCKS; ++j)
		encode(code + j * CODE_LEN, code + j * CODE_LEN + DATA_LEN);

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		orig[i] = code[i];

	for (int i = 0; i < BLOCKS; ++i)
		itl->fwd(code + i * CODE_LEN);

	for (int j = 0; j < BLOCKS; ++j)
		mod->mapN(symb + j * SYMBOLS, code + j * CODE_LEN);

	for (int i = 0; i < BLOCKS * SYMBOLS; ++i)
		symb[i] += complex_type(awgn(), awgn());

	if (1) {
		code_type tmp[MOD_BITS];
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			mod->hard(tmp, symb[i]);
			complex_type s = mod->map(tmp);
			complex_type e = symb[i] - s;
			sp += std::norm(s);
			np += std::norm(e);
		}
		value_type snr = 10 * std::log10(sp / np);
		mean = std::sqrt(sp / SYMBOLS);
		sigma = std::sqrt(np / (2 * sp));
		std::cerr << snr << " Es/N0, " << sigma << " sigma and " << mean << " mean estimated via hard decision." << std::endl;
	}

	// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
	// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
	value_type precision = FACTOR / (sigma * sigma);
	for (int j = 0; j < BLOCKS; ++j)
		mod->softN(code + j * CODE_LEN, symb + j * SYMBOLS, precision);

	for (int i = 0; i < BLOCKS; ++i)
		itl->bwd(code + i * CODE_LEN);

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		noisy[i] = code[i];

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		assert(!std::isnan(code[i]));

	LDPCDecoder<simd_type, algorithm_type> decode(ldpc);
	const int SIMD_WIDTH = algorithm_type::SIMD_WIDTH;
	int iterations = 0;
	auto start = std::chrono::system_clock::now();
	for (int j = 0; j < BLOCKS; j += SIMD_WIDTH) {
		int blocks = j + SIMD_WIDTH > BLOCKS ? BLOCKS - j : SIMD_WIDTH;
		for (int n = 0; n < blocks; ++n)
			for (int i = 0; i < CODE_LEN; ++i)
				reinterpret_cast<code_type *>(simd+i)[n] = code[(j+n)*CODE_LEN+i];
		int trials = 50;
		int count = decode(simd, simd + DATA_LEN, trials, blocks);
		for (int n = 0; n < blocks; ++n)
			for (int i = 0; i < CODE_LEN; ++i)
				code[(j+n)*CODE_LEN+i] = reinterpret_cast<code_type *>(simd+i)[n];
		if (count < 0) {
			iterations += blocks * trials;
			std::cerr << "decoder failed at converging to a code word!" << std::endl;
		} else {
			iterations += blocks * (trials - count);
			std::cerr << trials - count << " iterations were needed." << std::endl;
		}
	}
	auto end = std::chrono::system_clock::now();
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	int kbs = (BLOCKS * DATA_LEN + msec.count() / 2) / msec.count();
	std::cerr << kbs << " kilobit per second." << std::endl;
	float avg_iter = (float)iterations / (float)BLOCKS;
	std::cerr << avg_iter << " average iterations per block." << std::endl;

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		assert(!std::isnan(code[i]));

	int awgn_errors = 0;
	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		awgn_errors += noisy[i] * orig[i] < 0;
	int quantization_erasures = 0;
	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		quantization_erasures += !noisy[i];
	int uncorrected_errors = 0;
	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		uncorrected_errors += code[i] * orig[i] <= 0;
	int decoder_errors = 0;
	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		decoder_errors += code[i] * orig[i] <= 0 && orig[i] * noisy[i] > 0;
	float bit_error_rate = (float)uncorrected_errors / (float)(BLOCKS * CODE_LEN);

	if (1) {
		for (int i = 0; i < CODE_LEN; ++i)
			code[i] = code[i] < 0 ? -1 : 1;
		itl->fwd(code);
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			complex_type s = mod->map(code + i * MOD_BITS);
			complex_type e = symb[i] - s;
			sp += std::norm(s);
			np += std::norm(e);
		}
		value_type snr = 10 * std::log10(sp / np);
		mean = std::sqrt(sp / SYMBOLS);
		sigma = std::sqrt(np / (2 * sp));
		std::cerr << snr << " Es/N0, " << sigma << " sigma and " << mean << " mean estimated from corrected symbols." << std::endl;
	}

	std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
	std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
	std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
	std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
	std::cerr << bit_error_rate << " bit error rate." << std::endl;

	if (0) {
		std::cout << SNR << " " << bit_error_rate << " " << avg_iter << std::endl;
	}

	delete ldpc;
	delete mod;
	delete itl;

	free(aligned_buffer);
	delete[] code;
	delete[] orig;
	delete[] noisy;
	delete[] symb;

	return 0;
}

