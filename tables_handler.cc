/*
LDPC tables handler

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <cstring>
#include <cstdint>
#include "ldpc.hh"
#include "dvb_s2_tables.hh"
#include "dvb_s2x_tables.hh"
#include "dvb_t2_tables.hh"

LDPCInterface *create_ldpc(char *standard, char prefix, int number)
{
	if (!strcmp(standard, "S2")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_B1>();
			case 2:
				return new LDPC<DVB_S2_TABLE_B2>();
			case 3:
				return new LDPC<DVB_S2_TABLE_B3>();
			case 4:
				return new LDPC<DVB_S2_TABLE_B4>();
			case 5:
				return new LDPC<DVB_S2_TABLE_B5>();
			case 6:
				return new LDPC<DVB_S2_TABLE_B6>();
			case 7:
				return new LDPC<DVB_S2_TABLE_B7>();
			case 8:
				return new LDPC<DVB_S2_TABLE_B8>();
			case 9:
				return new LDPC<DVB_S2_TABLE_B9>();
			case 10:
				return new LDPC<DVB_S2_TABLE_B10>();
			case 11:
				return new LDPC<DVB_S2_TABLE_B11>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2_TABLE_C1>();
			case 2:
				return new LDPC<DVB_S2_TABLE_C2>();
			case 3:
				return new LDPC<DVB_S2_TABLE_C3>();
			case 4:
				return new LDPC<DVB_S2_TABLE_C4>();
			case 5:
				return new LDPC<DVB_S2_TABLE_C5>();
			case 6:
				return new LDPC<DVB_S2_TABLE_C6>();
			case 7:
				return new LDPC<DVB_S2_TABLE_C7>();
			case 8:
				return new LDPC<DVB_S2_TABLE_C8>();
			case 9:
				return new LDPC<DVB_S2_TABLE_C9>();
			case 10:
				return new LDPC<DVB_S2_TABLE_C10>();
			}
		}
	}
	if (!strcmp(standard, "S2X")) {
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_B1>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_B2>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_B3>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_B4>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_B5>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_B6>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_B7>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_B8>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_B9>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_B10>();
			case 11:
				return new LDPC<DVB_S2X_TABLE_B11>();
			case 12:
				return new LDPC<DVB_S2X_TABLE_B12>();
			case 13:
				return new LDPC<DVB_S2X_TABLE_B13>();
			case 14:
				return new LDPC<DVB_S2X_TABLE_B14>();
			case 15:
				return new LDPC<DVB_S2X_TABLE_B15>();
			case 16:
				return new LDPC<DVB_S2X_TABLE_B16>();
			case 17:
				return new LDPC<DVB_S2X_TABLE_B17>();
			case 18:
				return new LDPC<DVB_S2X_TABLE_B18>();
			case 19:
				return new LDPC<DVB_S2X_TABLE_B19>();
			case 20:
				return new LDPC<DVB_S2X_TABLE_B20>();
			case 21:
				return new LDPC<DVB_S2X_TABLE_B21>();
			case 22:
				return new LDPC<DVB_S2X_TABLE_B22>();
			case 23:
				return new LDPC<DVB_S2X_TABLE_B23>();
			case 24:
				return new LDPC<DVB_S2X_TABLE_B24>();
			}
		}
		if (prefix == 'C') {
			switch (number) {
			case 1:
				return new LDPC<DVB_S2X_TABLE_C1>();
			case 2:
				return new LDPC<DVB_S2X_TABLE_C2>();
			case 3:
				return new LDPC<DVB_S2X_TABLE_C3>();
			case 4:
				return new LDPC<DVB_S2X_TABLE_C4>();
			case 5:
				return new LDPC<DVB_S2X_TABLE_C5>();
			case 6:
				return new LDPC<DVB_S2X_TABLE_C6>();
			case 7:
				return new LDPC<DVB_S2X_TABLE_C7>();
			case 8:
				return new LDPC<DVB_S2X_TABLE_C8>();
			case 9:
				return new LDPC<DVB_S2X_TABLE_C9>();
			case 10:
				return new LDPC<DVB_S2X_TABLE_C10>();
			}
		}
	}
	if (!strcmp(standard, "T2")) {
		if (prefix == 'A') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_A1>();
			case 2:
				return new LDPC<DVB_T2_TABLE_A2>();
			case 3:
				return new LDPC<DVB_T2_TABLE_A3>();
			case 4:
				return new LDPC<DVB_T2_TABLE_A4>();
			case 5:
				return new LDPC<DVB_T2_TABLE_A5>();
			case 6:
				return new LDPC<DVB_T2_TABLE_A6>();
			}
		}
		if (prefix == 'B') {
			switch (number) {
			case 1:
				return new LDPC<DVB_T2_TABLE_B1>();
			case 2:
				return new LDPC<DVB_T2_TABLE_B2>();
			case 3:
				return new LDPC<DVB_T2_TABLE_B3>();
			case 4:
				return new LDPC<DVB_T2_TABLE_B4>();
			case 5:
				return new LDPC<DVB_T2_TABLE_B5>();
			case 6:
				return new LDPC<DVB_T2_TABLE_B6>();
			case 7:
				return new LDPC<DVB_T2_TABLE_B7>();
			case 8:
				return new LDPC<DVB_T2_TABLE_B8>();
			case 9:
				return new LDPC<DVB_T2_TABLE_B9>();
			}
		}
	}
	return 0;
}

