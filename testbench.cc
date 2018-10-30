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

template <typename TYPE>
Modulation<TYPE> *create_modulation(char *name)
{
	if (!strcmp(name, "BPSK"))
		return new PhaseShiftKeying<2, TYPE>();
	if (!strcmp(name, "QPSK"))
		return new PhaseShiftKeying<4, TYPE>();
	if (!strcmp(name, "8PSK"))
		return new PhaseShiftKeying<8, TYPE>();
	if (!strcmp(name, "QAM16"))
		return new QuadratureAmplitudeModulation<16, TYPE>();
	if (!strcmp(name, "QAM64"))
		return new QuadratureAmplitudeModulation<64, TYPE>();
	if (!strcmp(name, "QAM256"))
		return new QuadratureAmplitudeModulation<256, TYPE>();
	if (!strcmp(name, "QAM1024"))
		return new QuadratureAmplitudeModulation<1024, TYPE>();
	return 0;
}

template <typename TYPE>
LDPCInterface<TYPE> *create_decoder(char *standard, char prefix, int number)
{
	if (!strcmp(standard, "S2")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_B1, TYPE>();
			case 2:
				return new LDPC<DVB_S2_TABLE_B2, TYPE>();
			case 3:
				return new LDPC<DVB_S2_TABLE_B3, TYPE>();
			case 4:
				return new LDPC<DVB_S2_TABLE_B4, TYPE>();
			case 5:
				return new LDPC<DVB_S2_TABLE_B5, TYPE>();
			case 6:
				return new LDPC<DVB_S2_TABLE_B6, TYPE>();
			case 7:
				return new LDPC<DVB_S2_TABLE_B7, TYPE>();
			case 8:
				return new LDPC<DVB_S2_TABLE_B8, TYPE>();
			case 9:
				return new LDPC<DVB_S2_TABLE_B9, TYPE>();
			case 10:
				return new LDPC<DVB_S2_TABLE_B10, TYPE>();
			case 11:
				return new LDPC<DVB_S2_TABLE_B11, TYPE>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_C1, TYPE>();
			case 2:
				return new LDPC<DVB_S2_TABLE_C2, TYPE>();
			case 3:
				return new LDPC<DVB_S2_TABLE_C3, TYPE>();
			case 4:
				return new LDPC<DVB_S2_TABLE_C4, TYPE>();
			case 5:
				return new LDPC<DVB_S2_TABLE_C5, TYPE>();
			case 6:
				return new LDPC<DVB_S2_TABLE_C6, TYPE>();
			case 7:
				return new LDPC<DVB_S2_TABLE_C7, TYPE>();
			case 8:
				return new LDPC<DVB_S2_TABLE_C8, TYPE>();
			case 9:
				return new LDPC<DVB_S2_TABLE_C9, TYPE>();
			case 10:
				return new LDPC<DVB_S2_TABLE_C10, TYPE>();
			}
		}
	}
	if (!strcmp(standard, "S2X")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_B1, TYPE>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_B2, TYPE>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_B3, TYPE>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_B4, TYPE>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_B5, TYPE>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_B6, TYPE>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_B7, TYPE>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_B8, TYPE>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_B9, TYPE>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_B10, TYPE>();
			case 11:
				return new LDPC<DVB_S2X_TABLE_B11, TYPE>();
			case 12:
				return new LDPC<DVB_S2X_TABLE_B12, TYPE>();
			case 13:
				return new LDPC<DVB_S2X_TABLE_B13, TYPE>();
			case 14:
				return new LDPC<DVB_S2X_TABLE_B14, TYPE>();
			case 15:
				return new LDPC<DVB_S2X_TABLE_B15, TYPE>();
			case 16:
				return new LDPC<DVB_S2X_TABLE_B16, TYPE>();
			case 17:
				return new LDPC<DVB_S2X_TABLE_B17, TYPE>();
			case 18:
				return new LDPC<DVB_S2X_TABLE_B18, TYPE>();
			case 19:
				return new LDPC<DVB_S2X_TABLE_B19, TYPE>();
			case 20:
				return new LDPC<DVB_S2X_TABLE_B20, TYPE>();
			case 21:
				return new LDPC<DVB_S2X_TABLE_B21, TYPE>();
			case 22:
				return new LDPC<DVB_S2X_TABLE_B22, TYPE>();
			case 23:
				return new LDPC<DVB_S2X_TABLE_B23, TYPE>();
			case 24:
				return new LDPC<DVB_S2X_TABLE_B24, TYPE>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_C1, TYPE>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_C2, TYPE>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_C3, TYPE>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_C4, TYPE>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_C5, TYPE>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_C6, TYPE>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_C7, TYPE>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_C8, TYPE>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_C9, TYPE>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_C10, TYPE>();
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

	LDPCInterface<value_type> *ldpc = create_decoder<value_type>(argv[2], argv[3][0], atoi(argv[3]+1));
	if (!ldpc) {
		std::cerr << "no such table!" << std::endl;
		return -1;
	}
	std::cerr << "testing LDPC(" << ldpc->code_len() << ", " << ldpc->data_len() << ") code." << std::endl;

	Modulation<complex_type> *mod = create_modulation<complex_type>(argv[4]);
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
	value_type code[ldpc->code_len()], orig[ldpc->code_len()], noisy[ldpc->code_len()];
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
			std::cerr << "	" << code[i+ldpc->data_len()];
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
		value_type tmp[mod->bits()];
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
	value_type precision = 1 / (sigma * sigma);
	for (int i = 0; i < SYMBOLS; ++i)
		mod->soft(code + i, symb[i], precision, SYMBOLS);

	for (int i = 0; i < ldpc->code_len(); ++i)
		noisy[i] = code[i];

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i) {
			std::cout << symb[i].real() << " " << symb[i].imag();
			for (int j = 0; j < mod->bits(); ++j)
				std::cout << " " << code[i + j * SYMBOLS];
			std::cout << std::endl;
		}
	}

	if (SHOW) {
		std::cerr << std::setprecision(4);
		std::cerr << "recv:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << code[i+ldpc->data_len()];
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
		awgn_errors += noisy[i] * orig[i] <= 0;
	int uncorrected_errors = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		uncorrected_errors += code[i] * orig[i] <= 0;
	int decoder_errors = 0;
	for (int i = 0; i < ldpc->code_len(); ++i)
		decoder_errors += code[i] * orig[i] <= 0 && orig[i] * noisy[i] > 0;

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
	std::cerr << decoder_errors << " errors caused by decoder." << std::endl;
	std::cerr << uncorrected_errors << " errors uncorrected." << std::endl;

	delete ldpc;

	return 0;
}

