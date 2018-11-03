/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <functional>
#include <complex>
#include "psk.hh"
#include "qam.hh"
#include "ldpc.hh"
#include "dvb_s2_tables.hh"
#include "dvb_s2x_tables.hh"
#include "dvb_t2_tables.hh"

template <typename TYPE, typename CODE>
Modulation<TYPE, CODE> *create_modulation(char *name)
{
	if (!strcmp(name, "BPSK"))
		return new PhaseShiftKeying<2, TYPE, CODE>();
	if (!strcmp(name, "QPSK"))
		return new PhaseShiftKeying<4, TYPE, CODE>();
	if (!strcmp(name, "8PSK"))
		return new PhaseShiftKeying<8, TYPE, CODE>();
	if (!strcmp(name, "QAM16"))
		return new QuadratureAmplitudeModulation<16, TYPE, CODE>();
	if (!strcmp(name, "QAM64"))
		return new QuadratureAmplitudeModulation<64, TYPE, CODE>();
	if (!strcmp(name, "QAM256"))
		return new QuadratureAmplitudeModulation<256, TYPE, CODE>();
	if (!strcmp(name, "QAM1024"))
		return new QuadratureAmplitudeModulation<1024, TYPE, CODE>();
	return 0;
}

template <typename TYPE, int FACTOR>
LDPCInterface<TYPE> *create_decoder(char *standard, char prefix, int number)
{
	if (!strcmp(standard, "S2")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_B1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_S2_TABLE_B2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_S2_TABLE_B3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_S2_TABLE_B4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_S2_TABLE_B5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_S2_TABLE_B6, TYPE, FACTOR>();
			case 7:
				return new LDPC<DVB_S2_TABLE_B7, TYPE, FACTOR>();
			case 8:
				return new LDPC<DVB_S2_TABLE_B8, TYPE, FACTOR>();
			case 9:
				return new LDPC<DVB_S2_TABLE_B9, TYPE, FACTOR>();
			case 10:
				return new LDPC<DVB_S2_TABLE_B10, TYPE, FACTOR>();
			case 11:
				return new LDPC<DVB_S2_TABLE_B11, TYPE, FACTOR>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_C1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_S2_TABLE_C2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_S2_TABLE_C3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_S2_TABLE_C4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_S2_TABLE_C5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_S2_TABLE_C6, TYPE, FACTOR>();
			case 7:
				return new LDPC<DVB_S2_TABLE_C7, TYPE, FACTOR>();
			case 8:
				return new LDPC<DVB_S2_TABLE_C8, TYPE, FACTOR>();
			case 9:
				return new LDPC<DVB_S2_TABLE_C9, TYPE, FACTOR>();
			case 10:
				return new LDPC<DVB_S2_TABLE_C10, TYPE, FACTOR>();
			}
		}
	}
	if (!strcmp(standard, "S2X")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_B1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_B2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_B3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_B4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_B5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_B6, TYPE, FACTOR>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_B7, TYPE, FACTOR>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_B8, TYPE, FACTOR>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_B9, TYPE, FACTOR>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_B10, TYPE, FACTOR>();
			case 11:
				return new LDPC<DVB_S2X_TABLE_B11, TYPE, FACTOR>();
			case 12:
				return new LDPC<DVB_S2X_TABLE_B12, TYPE, FACTOR>();
			case 13:
				return new LDPC<DVB_S2X_TABLE_B13, TYPE, FACTOR>();
			case 14:
				return new LDPC<DVB_S2X_TABLE_B14, TYPE, FACTOR>();
			case 15:
				return new LDPC<DVB_S2X_TABLE_B15, TYPE, FACTOR>();
			case 16:
				return new LDPC<DVB_S2X_TABLE_B16, TYPE, FACTOR>();
			case 17:
				return new LDPC<DVB_S2X_TABLE_B17, TYPE, FACTOR>();
			case 18:
				return new LDPC<DVB_S2X_TABLE_B18, TYPE, FACTOR>();
			case 19:
				return new LDPC<DVB_S2X_TABLE_B19, TYPE, FACTOR>();
			case 20:
				return new LDPC<DVB_S2X_TABLE_B20, TYPE, FACTOR>();
			case 21:
				return new LDPC<DVB_S2X_TABLE_B21, TYPE, FACTOR>();
			case 22:
				return new LDPC<DVB_S2X_TABLE_B22, TYPE, FACTOR>();
			case 23:
				return new LDPC<DVB_S2X_TABLE_B23, TYPE, FACTOR>();
			case 24:
				return new LDPC<DVB_S2X_TABLE_B24, TYPE, FACTOR>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_C1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_C2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_C3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_C4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_C5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_C6, TYPE, FACTOR>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_C7, TYPE, FACTOR>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_C8, TYPE, FACTOR>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_C9, TYPE, FACTOR>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_C10, TYPE, FACTOR>();
			}
		}
	}
	if (!strcmp(standard, "T2")) {
		if (prefix == 'A') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_A1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_T2_TABLE_A2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_T2_TABLE_A3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_T2_TABLE_A4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_T2_TABLE_A5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_T2_TABLE_A6, TYPE, FACTOR>();
			}
		}
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_B1, TYPE, FACTOR>();
			case 2:
				return new LDPC<DVB_T2_TABLE_B2, TYPE, FACTOR>();
			case 3:
				return new LDPC<DVB_T2_TABLE_B3, TYPE, FACTOR>();
			case 4:
				return new LDPC<DVB_T2_TABLE_B4, TYPE, FACTOR>();
			case 5:
				return new LDPC<DVB_T2_TABLE_B5, TYPE, FACTOR>();
			case 6:
				return new LDPC<DVB_T2_TABLE_B6, TYPE, FACTOR>();
			case 7:
				return new LDPC<DVB_T2_TABLE_B7, TYPE, FACTOR>();
			case 8:
				return new LDPC<DVB_T2_TABLE_B8, TYPE, FACTOR>();
			case 9:
				return new LDPC<DVB_T2_TABLE_B9, TYPE, FACTOR>();
			}
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 5)
		return -1;
	typedef float value_type;
	typedef std::complex<value_type> complex_type;
#if 0
	typedef int8_t code_type;
	const int factor = 2;
#else
	typedef float code_type;
	const int factor = 1;
#endif

	LDPCInterface<code_type> *ldpc = create_decoder<code_type, factor>(argv[2], argv[3][0], atoi(argv[3]+1));
	if (!ldpc) {
		std::cerr << "no such table!" << std::endl;
		return -1;
	}
	std::cerr << "testing LDPC(" << ldpc->code_len() << ", " << ldpc->data_len() << ") code." << std::endl;

	Modulation<complex_type, code_type> *mod = create_modulation<complex_type, code_type>(argv[4]);
	if (!mod) {
		std::cerr << "no such modulation!" << std::endl;
		return -1;
	}

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

	assert(ldpc->code_len() % mod->bits() == 0);
	const int SYMBOLS = ldpc->code_len() / mod->bits();
	code_type code[ldpc->code_len()], orig[ldpc->code_len()], noisy[ldpc->code_len()];
	complex_type symb[SYMBOLS];
	const int SHOW = 0;

	//ldpc->examine();

	for (int i = 0; i < ldpc->data_len(); ++i)
		code[i] = 1 - 2 * data();

	ldpc->encode(code, code + ldpc->data_len());

	for (int i = 0; i < ldpc->code_len(); ++i)
		orig[i] = code[i];

	std::cerr << std::showpos;
	std::cerr << std::setprecision(3);

	if (SHOW) {
		std::cerr << "send:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << +code[i+ldpc->data_len()];
		std::cerr << std::endl;
	}

	for (int i = 0; i < SYMBOLS; ++i)
		symb[i] = mod->map(code + i, SYMBOLS);

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i)
			std::cout << symb[i].real() << " " << symb[i].imag() << std::endl;
	}

	for (int i = 0; i < SYMBOLS; ++i)
		symb[i] += complex_type(awgn(), awgn());

	if (1) {
		code_type tmp[mod->bits()];
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			mod->hard(tmp, symb[i]);
			complex_type s = mod->map(tmp);
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
	value_type precision = factor / (sigma * sigma);
	for (int i = 0; i < SYMBOLS; ++i)
		mod->soft(code + i, symb[i], precision, SYMBOLS);

	for (int i = 0; i < ldpc->code_len(); ++i)
		noisy[i] = code[i];

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i) {
			std::cout << symb[i].real() << " " << symb[i].imag();
			for (int j = 0; j < mod->bits(); ++j)
				std::cout << " " << +code[i + j * SYMBOLS];
			std::cout << std::endl;
		}
	}

	if (SHOW) {
		std::cerr << std::setprecision(4);
		std::cerr << "recv:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << +code[i+ldpc->data_len()];
		std::cerr << std::endl;
		std::cerr << std::setprecision(3);
	}

	for (int i = 0; i < ldpc->code_len(); ++i)
		assert(!std::isnan(code[i]));

	int trials = 50;
	int count = ldpc->decode(code, code + ldpc->data_len(), trials);
	if (count < 0)
		std::cerr << "decoder failed at converging to a code word!" << std::endl;
	else
		std::cerr << trials - count << " iterations were needed." << std::endl;

	for (int i = 0; i < ldpc->code_len(); ++i)
		assert(!std::isnan(code[i]));

	int awgn_errors = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		awgn_errors += noisy[i] * orig[i] < 0;
	int quantization_erasures = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		quantization_erasures += !noisy[i];
	int uncorrected_errors = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		uncorrected_errors += code[i] * orig[i] <= 0;
	int decoder_errors = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		decoder_errors += code[i] * orig[i] <= 0 && orig[i] * noisy[i] > 0;
	float bit_error_rate = (float)uncorrected_errors / (float)ldpc->code_len();

	if (1) {
		for (int i = 0; i < ldpc->code_len(); ++i)
			code[i] = code[i] < 0 ? -1 : 1;
		value_type sp = 0, np = 0;
		for (int i = 0; i < SYMBOLS; ++i) {
			complex_type s = mod->map(code + i, SYMBOLS);
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
	std::cerr << quantization_erasures << " erasures caused by quantization." << std::endl;
	std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
	std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;
	std::cerr << bit_error_rate << " bit error rate." << std::endl;

	delete ldpc;

	return 0;
}

