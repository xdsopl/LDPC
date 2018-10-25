/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>
#include <complex>
#include "psk.hh"
#include "qam.hh"
#include "ldpc.hh"
#include "dvb_s2_tables.hh"

typedef DVB_S2_TABLE_B4 TABLE;
constexpr int TABLE::DEG[];
constexpr int TABLE::LEN[];
constexpr int TABLE::POS[];

int main(int argc, char **argv)
{
	if (argc != 2)
		return -1;
	typedef float value_type;
	typedef std::complex<value_type> complex_type;
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
	//typedef PhaseShiftKeying<8, complex_type> MOD;
	typedef QuadratureAmplitudeModulation<16, complex_type> MOD;
	assert(TABLE::N%MOD::BITS == 0);
	const int SYMBOLS = TABLE::N / MOD::BITS;
	LDPC<TABLE, value_type> ldpc;
	value_type code[TABLE::N], orig[TABLE::N], noisy[TABLE::N];
	complex_type symb[SYMBOLS];
	const int SHOW = 0;

	//ldpc.examine();

	for (int i = 0; i < TABLE::K; ++i)
		code[i] = 1 - 2 * data();

	ldpc.encode(code, code + TABLE::K);

	for (int i = 0; i < TABLE::N; ++i)
		orig[i] = code[i];

	std::cerr << std::showpos;
	std::cerr << std::setprecision(3);

	if (SHOW) {
		std::cerr << "send:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << code[i+TABLE::K];
		std::cerr << std::endl;
	}

	for (int i = 0; i < SYMBOLS; ++i)
		symb[i] = MOD::map(code + i, SYMBOLS);

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i)
			std::cout << symb[i].real() << " " << symb[i].imag() << std::endl;
	}

	for (int i = 0; i < SYMBOLS; ++i)
		symb[i] += complex_type(awgn(), awgn());

	if (1) {
		value_type tmp[MOD::BITS];
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			MOD::hard(tmp, symb[i]);
			complex_type s = MOD::map(tmp);
			complex_type e = symb[i] - s;
			sp += std::norm(s);
			np += std::norm(e);
		}
		SNR = 10 * std::log10(sp / np);
		mean = std::sqrt(sp / SYMBOLS);
		sigma = std::sqrt(np / (2 * sp));
		std::cerr << SNR << " Es/N0, " << sigma << " sigma and " << mean << " mean estimated via hard decision." << std::endl;
	}

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i)
			std::cout << symb[i].real() << " " << symb[i].imag() << std::endl;
	}

	// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
	// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
	value_type precision = 1 / (sigma * sigma);
	for (int i = 0; i < SYMBOLS; ++i)
		MOD::soft(code + i, symb[i], precision, SYMBOLS);

	for (int i = 0; i < TABLE::N; ++i)
		noisy[i] = code[i];

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i) {
			std::cout << symb[i].real() << " " << symb[i].imag();
			for (int j = 0; j < MOD::BITS; ++j)
				std::cout << " " << code[i + j * SYMBOLS];
			std::cout << std::endl;
		}
	}

	if (SHOW) {
		std::cerr << std::setprecision(4);
		std::cerr << "recv:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << code[i+TABLE::K];
		std::cerr << std::endl;
		std::cerr << std::setprecision(3);
	}

	for (int i = 0; i < TABLE::N; ++i)
		assert(!std::isnan(code[i]));

	ldpc.decode(code, code + TABLE::K);

	for (int i = 0; i < TABLE::N; ++i)
		assert(!std::isnan(code[i]));

	int awgn_errors = 0;
	for (int i = 0; i < TABLE::N; ++i)
		awgn_errors += noisy[i] * orig[i] <= 0;
	int uncorrected_errors = 0;
	for (int i = 0; i < TABLE::N; ++i)
		uncorrected_errors += code[i] * orig[i] <= 0;
	int decoder_errors = 0;
	for (int i = 0; i < TABLE::N; ++i)
		decoder_errors += code[i] * orig[i] <= 0 && orig[i] * noisy[i] > 0;

	if (1) {
		for (int i = 0; i < TABLE::N; ++i)
			code[i] = code[i] < 0 ? -1 : 1;
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			complex_type s = MOD::map(code + i, SYMBOLS);
			complex_type e = symb[i] - s;
			sp += std::norm(s);
			np += std::norm(e);
		}
		SNR = 10 * std::log10(sp / np);
		mean = std::sqrt(sp / SYMBOLS);
		sigma = std::sqrt(np / (2 * sp));
		std::cerr << SNR << " Es/N0, " << sigma << " sigma and " << mean << " mean estimated from corrected symbols." << std::endl;
	}

	std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
	std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
	std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;

	return 0;
}

