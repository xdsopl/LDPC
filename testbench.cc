/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

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
#include "dvb_s2_tables.hh"
#include "dvb_s2x_tables.hh"
#include "dvb_t2_tables.hh"
#include "interleaver.hh"
#include "modulation.hh"

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

template <typename TYPE, typename UPDATE, typename ALGORITHM>
LDPCInterface<TYPE> *create_decoder(char *standard, char prefix, int number)
{
	if (!strcmp(standard, "S2")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_B1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_S2_TABLE_B2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_S2_TABLE_B3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_S2_TABLE_B4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_S2_TABLE_B5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_S2_TABLE_B6, TYPE, UPDATE, ALGORITHM>();
			case 7:
				return new LDPC<DVB_S2_TABLE_B7, TYPE, UPDATE, ALGORITHM>();
			case 8:
				return new LDPC<DVB_S2_TABLE_B8, TYPE, UPDATE, ALGORITHM>();
			case 9:
				return new LDPC<DVB_S2_TABLE_B9, TYPE, UPDATE, ALGORITHM>();
			case 10:
				return new LDPC<DVB_S2_TABLE_B10, TYPE, UPDATE, ALGORITHM>();
			case 11:
				return new LDPC<DVB_S2_TABLE_B11, TYPE, UPDATE, ALGORITHM>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_C1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_S2_TABLE_C2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_S2_TABLE_C3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_S2_TABLE_C4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_S2_TABLE_C5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_S2_TABLE_C6, TYPE, UPDATE, ALGORITHM>();
			case 7:
				return new LDPC<DVB_S2_TABLE_C7, TYPE, UPDATE, ALGORITHM>();
			case 8:
				return new LDPC<DVB_S2_TABLE_C8, TYPE, UPDATE, ALGORITHM>();
			case 9:
				return new LDPC<DVB_S2_TABLE_C9, TYPE, UPDATE, ALGORITHM>();
			case 10:
				return new LDPC<DVB_S2_TABLE_C10, TYPE, UPDATE, ALGORITHM>();
			}
		}
	}
	if (!strcmp(standard, "S2X")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_B1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_B2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_B3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_B4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_B5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_B6, TYPE, UPDATE, ALGORITHM>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_B7, TYPE, UPDATE, ALGORITHM>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_B8, TYPE, UPDATE, ALGORITHM>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_B9, TYPE, UPDATE, ALGORITHM>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_B10, TYPE, UPDATE, ALGORITHM>();
			case 11:
				return new LDPC<DVB_S2X_TABLE_B11, TYPE, UPDATE, ALGORITHM>();
			case 12:
				return new LDPC<DVB_S2X_TABLE_B12, TYPE, UPDATE, ALGORITHM>();
			case 13:
				return new LDPC<DVB_S2X_TABLE_B13, TYPE, UPDATE, ALGORITHM>();
			case 14:
				return new LDPC<DVB_S2X_TABLE_B14, TYPE, UPDATE, ALGORITHM>();
			case 15:
				return new LDPC<DVB_S2X_TABLE_B15, TYPE, UPDATE, ALGORITHM>();
			case 16:
				return new LDPC<DVB_S2X_TABLE_B16, TYPE, UPDATE, ALGORITHM>();
			case 17:
				return new LDPC<DVB_S2X_TABLE_B17, TYPE, UPDATE, ALGORITHM>();
			case 18:
				return new LDPC<DVB_S2X_TABLE_B18, TYPE, UPDATE, ALGORITHM>();
			case 19:
				return new LDPC<DVB_S2X_TABLE_B19, TYPE, UPDATE, ALGORITHM>();
			case 20:
				return new LDPC<DVB_S2X_TABLE_B20, TYPE, UPDATE, ALGORITHM>();
			case 21:
				return new LDPC<DVB_S2X_TABLE_B21, TYPE, UPDATE, ALGORITHM>();
			case 22:
				return new LDPC<DVB_S2X_TABLE_B22, TYPE, UPDATE, ALGORITHM>();
			case 23:
				return new LDPC<DVB_S2X_TABLE_B23, TYPE, UPDATE, ALGORITHM>();
			case 24:
				return new LDPC<DVB_S2X_TABLE_B24, TYPE, UPDATE, ALGORITHM>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_C1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_C2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_C3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_C4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_C5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_C6, TYPE, UPDATE, ALGORITHM>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_C7, TYPE, UPDATE, ALGORITHM>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_C8, TYPE, UPDATE, ALGORITHM>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_C9, TYPE, UPDATE, ALGORITHM>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_C10, TYPE, UPDATE, ALGORITHM>();
			}
		}
	}
	if (!strcmp(standard, "T2")) {
		if (prefix == 'A') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_A1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_T2_TABLE_A2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_T2_TABLE_A3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_T2_TABLE_A4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_T2_TABLE_A5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_T2_TABLE_A6, TYPE, UPDATE, ALGORITHM>();
			}
		}
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_B1, TYPE, UPDATE, ALGORITHM>();
			case 2:
				return new LDPC<DVB_T2_TABLE_B2, TYPE, UPDATE, ALGORITHM>();
			case 3:
				return new LDPC<DVB_T2_TABLE_B3, TYPE, UPDATE, ALGORITHM>();
			case 4:
				return new LDPC<DVB_T2_TABLE_B4, TYPE, UPDATE, ALGORITHM>();
			case 5:
				return new LDPC<DVB_T2_TABLE_B5, TYPE, UPDATE, ALGORITHM>();
			case 6:
				return new LDPC<DVB_T2_TABLE_B6, TYPE, UPDATE, ALGORITHM>();
			case 7:
				return new LDPC<DVB_T2_TABLE_B7, TYPE, UPDATE, ALGORITHM>();
			case 8:
				return new LDPC<DVB_T2_TABLE_B8, TYPE, UPDATE, ALGORITHM>();
			case 9:
				return new LDPC<DVB_T2_TABLE_B9, TYPE, UPDATE, ALGORITHM>();
			}
		}
	}
	return 0;
}

template <typename TYPE>
Interleaver<TYPE> *create_interleaver(char *modulation, char *standard, char prefix, int number)
{
	if (!strcmp(standard, "S2")) {
		if (!strcmp(modulation, "8PSK")) {
			typedef MUX3<TYPE, 0, 1, 2> _012;
			typedef MUX3<TYPE, 2, 1, 0> _210;
			if (prefix == 'B') {
				switch (number) {
				case 5:
					return new BITL<TYPE, PITL0<TYPE, 64800>, _210>();
				default:
					return new BITL<TYPE, PITL0<TYPE, 64800>, _012>();
				}
			}
			if (prefix == 'C') {
				switch (number) {
				case 5:
					return new BITL<TYPE, PITL0<TYPE, 16200>, _210>();
				default:
					return new BITL<TYPE, PITL0<TYPE, 16200>, _012>();
				}
			}
		}
	}
	if (!strcmp(standard, "S2X")) {
		if (!strcmp(modulation, "8PSK")) {
			typedef MUX3<TYPE, 0, 1, 2> _012;
			typedef MUX3<TYPE, 1, 0, 2> _102;
			if (prefix == 'B') {
				switch (number) {
				case 7:
				case 8:
				case 9:
					return new BITL<TYPE, PITL0<TYPE, 64800>, _102>();
				default:
					return new BITL<TYPE, PITL0<TYPE, 64800>, _012>();
				}
			}
			if (prefix == 'C') {
				switch (number) {
				case 4:
				case 5:
				case 6:
				case 7:
					return new BITL<TYPE, PITL0<TYPE, 16200>, _102>();
				default:
					return new BITL<TYPE, PITL0<TYPE, 16200>, _012>();
				}
			}
		}
	}
	if (!strcmp(standard, "T2")) {
		if (!strcmp(modulation, "QPSK")) {
			if (prefix == 'B') {
				switch (number) {
				case 8:
					return new BITL<TYPE, PITL<TYPE, 16200, 30>, MUX0<TYPE>>();
				case 9:
					return new BITL<TYPE, PITL<TYPE, 16200, 27>, MUX0<TYPE>>();
				}
			}
		}
		if (!strcmp(modulation, "QAM16")) {
			typedef MUX8<TYPE, 7, 1, 4, 2, 5, 3, 6, 0> _71425360;
			if (prefix == 'A') {
				typedef MUX8<TYPE, 0, 5, 1, 2, 4, 7, 3, 6> _05124736;
				typedef CT8<TYPE, 0, 0, 2, 4, 4, 5, 7, 7> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 90, CT>, _71425360>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 72, CT>, _05124736>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 60, CT>, _71425360>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 45, CT>, _71425360>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 36, CT>, _71425360>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 30, CT>, _71425360>();
				}
			}
			if (prefix == 'B') {
				typedef MUX8<TYPE, 6, 0, 3, 4, 5, 2, 1, 7> _60345217;
				typedef MUX8<TYPE, 7, 5, 4, 0, 3, 1, 2, 6> _75403126;
				typedef CT8<TYPE, 0, 0, 0, 1, 7, 20, 20, 21> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 36, CT>, _71425360>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 25, CT>, _71425360>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 18, CT>, _71425360>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 15, CT>, _71425360>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 12, CT>, _71425360>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 10, CT>, _71425360>();
				case 7:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 8, CT>, _71425360>();
				case 8:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 30, CT>, _60345217>();
				case 9:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 27, CT>, _75403126>();
				}
			}
		}
		if (!strcmp(modulation, "QAM64")) {
			typedef MUX12<TYPE, 11, 7, 3, 10, 6, 2, 9, 5, 1, 8, 4, 0> _11731062951840;
			if (prefix == 'A') {
				typedef MUX12<TYPE, 2, 7, 6, 9, 0, 3, 1, 8, 4, 11, 5, 10> _27690318411510;
				typedef CT12<TYPE, 0, 0, 2, 2, 3, 4, 4, 5, 5, 7, 8, 9> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 90, CT>, _11731062951840>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 72, CT>, _27690318411510>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 60, CT>, _11731062951840>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 45, CT>, _11731062951840>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 36, CT>, _11731062951840>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 30, CT>, _11731062951840>();
				}
			}
			if (prefix == 'B') {
				typedef MUX12<TYPE, 4, 2, 0, 5, 6, 1, 3, 7, 8, 9, 10, 11> _42056137891011;
				typedef MUX12<TYPE, 4, 0, 1, 6, 2, 3, 5, 8, 7, 10, 9, 11> _40162358710911;
				typedef CT12<TYPE, 0, 0, 0, 2, 2, 2, 3, 3, 3, 6, 7, 7> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 36, CT>, _11731062951840>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 25, CT>, _11731062951840>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 18, CT>, _11731062951840>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 15, CT>, _11731062951840>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 12, CT>, _11731062951840>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 10, CT>, _11731062951840>();
				case 7:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 8, CT>, _11731062951840>();
				case 8:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 30, CT>, _42056137891011>();
				case 9:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 27, CT>, _40162358710911>();
				}
			}
		}
		if (!strcmp(modulation, "QAM256")) {
			if (prefix == 'A') {
				typedef MUX16<TYPE, 15, 1, 13, 3, 8, 11, 9, 5, 10, 6, 4, 7, 12, 2, 14, 0> _1511338119510647122140;
				typedef MUX16<TYPE, 2, 11, 3, 4, 0, 9, 1, 8, 10, 13, 7, 14, 6, 15, 5, 12> _2113409181013714615512;
				typedef MUX16<TYPE, 7, 2, 9, 0, 4, 6, 13, 3, 14, 10, 15, 5, 8, 12, 11, 1> _7290461331410155812111;
				typedef CT16<TYPE, 0, 2, 2, 2, 2, 3, 7, 15, 16, 20, 22, 22, 27, 27, 28, 32> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 90, CT>, _1511338119510647122140>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 72, CT>, _2113409181013714615512>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 60, CT>, _7290461331410155812111>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 45, CT>, _1511338119510647122140>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 36, CT>, _1511338119510647122140>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 64800, 30, CT>, _1511338119510647122140>();
				}
			}
			if (prefix == 'B') {
				typedef MUX8<TYPE, 7, 3, 1, 5, 2, 6, 4, 0> _73152640;
				typedef MUX8<TYPE, 4, 0, 1, 2, 5, 3, 6, 7> _40125367;
				typedef MUX8<TYPE, 4, 0, 5, 1, 2, 3, 6, 7> _40512367;
				typedef CT8<TYPE, 0, 0, 0, 1, 7, 20, 20, 21> CT;
				switch (number) {
				case 1:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 36, CT>, _73152640>();
				case 2:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 25, CT>, _73152640>();
				case 3:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 18, CT>, _73152640>();
				case 4:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 15, CT>, _73152640>();
				case 5:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 12, CT>, _73152640>();
				case 6:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 10, CT>, _73152640>();
				case 7:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 8, CT>, _73152640>();
				case 8:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 30, CT>, _40125367>();
				case 9:
					return new BITL<TYPE, PCTITL<TYPE, 16200, 27, CT>, _40512367>();
				}
			}
		}
	}
	std::cerr << "using noop interleaver." << std::endl;
	return new ITL0<TYPE>();
}

int main(int argc, char **argv)
{
	if (argc != 6)
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

	//typedef NormalUpdate<code_type> update_type;
	typedef SelfCorrectedUpdate<code_type> update_type;

	typedef MinSumAlgorithm<code_type> algorithm_type;
	//typedef MinSumCAlgorithm<code_type, factor> algorithm_type;
	//typedef LogDomainSPA<code_type> algorithm_type;
	//typedef LambdaMinAlgorithm<code_type, 3> algorithm_type;
	//typedef SumProductAlgorithm<code_type> algorithm_type;

	LDPCInterface<code_type> *ldpc = create_decoder<code_type, update_type, algorithm_type>(argv[2], argv[3][0], atoi(argv[3]+1));
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

	Interleaver<code_type> *itl = create_interleaver<code_type>(argv[4], argv[2], argv[3][0], atoi(argv[3]+1));
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
	code_type *code = new code_type[BLOCKS * CODE_LEN];
	code_type *orig = new code_type[BLOCKS * CODE_LEN];
	code_type *noisy = new code_type[BLOCKS * CODE_LEN];
	complex_type *symb = new complex_type[BLOCKS * SYMBOLS];
	const int SHOW = 0;

	//ldpc->examine();

	for (int j = 0; j < BLOCKS; ++j)
		for (int i = 0; i < DATA_LEN; ++i)
			code[j * CODE_LEN + i] = 1 - 2 * data();

	for (int i = 0; i < BLOCKS; ++i)
		ldpc->encode(code + i * CODE_LEN, code + i * CODE_LEN + DATA_LEN);

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		orig[i] = code[i];

	std::cerr << std::showpos;
	std::cerr << std::setprecision(3);

	if (SHOW) {
		std::cerr << "send:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << +code[i+DATA_LEN];
		std::cerr << std::endl;
	}

	for (int i = 0; i < BLOCKS; ++i)
		itl->fwd(code + i * CODE_LEN);

	for (int j = 0; j < BLOCKS; ++j)
		mod->mapN(symb + j * SYMBOLS, code + j * CODE_LEN);

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i)
			std::cout << symb[i].real() << " " << symb[i].imag() << std::endl;
	}

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

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i)
			std::cout << symb[i].real() << " " << symb[i].imag() << std::endl;
	}

	// $LLR=log(\frac{p(x=+1|y)}{p(x=-1|y)})$
	// $p(x|\mu,\sigma)=\frac{1}{\sqrt{2\pi}\sigma}}e^{-\frac{(x-\mu)^2}{2\sigma^2}}$
	value_type precision = factor / (sigma * sigma);
	for (int j = 0; j < BLOCKS; ++j)
		mod->softN(code + j * CODE_LEN, symb + j * SYMBOLS, precision);

	for (int i = 0; i < BLOCKS; ++i)
		itl->bwd(code + i * CODE_LEN);

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		noisy[i] = code[i];

	if (0) {
		for (int i = 0; i < SYMBOLS; ++i) {
			std::cout << symb[i].real() << " " << symb[i].imag();
			for (int j = 0; j < MOD_BITS; ++j)
				std::cout << " " << +code[i + j * SYMBOLS];
			std::cout << std::endl;
		}
	}

	if (SHOW) {
		std::cerr << std::setprecision(4);
		std::cerr << "recv:";
		for (int i = 0; i < SHOW; ++i)
			std::cerr << "	" << +code[i+DATA_LEN];
		std::cerr << std::endl;
		std::cerr << std::setprecision(3);
	}

	for (int i = 0; i < BLOCKS * CODE_LEN; ++i)
		assert(!std::isnan(code[i]));

	int iterations = 0;
	auto start = std::chrono::system_clock::now();
	for (int j = 0; j < BLOCKS; ++j) {
		int trials = 50;
		int count = ldpc->decode(code + j * CODE_LEN, code + j * CODE_LEN + DATA_LEN, trials);
		if (count < 0) {
			iterations += trials;
			std::cerr << "decoder failed at converging to a code word!" << std::endl;
		} else {
			iterations += trials - count;
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

	delete[] code;
	delete[] orig;
	delete[] noisy;
	delete[] symb;

	return 0;
}

