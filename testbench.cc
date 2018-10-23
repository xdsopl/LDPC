/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include "ldpc.hh"
#include "dvb_s2_tables.hh"

int main(int argc, char **argv)
{
	if (argc != 2)
		return -1;
	float SNR = atof(argv[1]);
	float sigma = std::sqrt(1 / std::pow(10, SNR / 10));
	//float SNR = 10 * log10(1 / (sigma * sigma));
	std::cerr << SNR << " Eb/N0 => standard deviation of " << sigma << " with mean 1" << std::endl;

	std::random_device rd;
	std::default_random_engine generator(rd());
	typedef std::uniform_int_distribution<int> uniform;
	typedef std::normal_distribution<float> normal;
	auto data = std::bind(uniform(0, 1), generator);
	auto awgn = std::bind(normal(0.0, sigma), generator);
	typedef DVB_S2_TABLE_B4 TABLE;
	LDPC<TABLE, float> ldpc;
	float code[TABLE::N], orig[TABLE::N], noisy[TABLE::N];
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

	for (int i = 0; i < TABLE::N; ++i)
		code[i] += awgn();

	for (int i = 0; i < TABLE::N; ++i)
		noisy[i] = code[i];

	if (SHOW) {
		std::cerr << "recv:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << code[i+TABLE::K];
		std::cerr << std::endl;
	}

	// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
	// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
	for (int i = 0; i < TABLE::N; ++i)
		code[i] = 2 * code[i] / (sigma * sigma);

	if (SHOW) {
		std::cerr << std::setprecision(4);
		std::cerr << "pllr:";
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
	std::cerr << awgn_errors << " errors caused by AWGN." << std::endl;
	std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
	std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;

	return 0;
}

