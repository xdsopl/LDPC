/*
Bit interleaver

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef INTERLEAVER_HH
#define INTERLEAVER_HH

template <typename TYPE>
struct Interleaver
{
	virtual void fwd(TYPE *) = 0;
	virtual void bwd(TYPE *) = 0;
	virtual ~Interleaver() = default;
};

template <typename TYPE>
struct ITL0 : public Interleaver<TYPE>
{
	void fwd(TYPE *){}
	void bwd(TYPE *){}
};

template <typename TYPE, int N, int E0, int E1, int E2>
struct ITL3 : public Interleaver<TYPE>
{
	static const int COLS = 3;
	static const int ROWS = N / COLS;
	TYPE tmp[N];
	void fwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[COLS*row+E0] = io[ROWS*0+row];
			tmp[COLS*row+E1] = io[ROWS*1+row];
			tmp[COLS*row+E2] = io[ROWS*2+row];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
	void bwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[ROWS*0+row] = io[COLS*row+E0];
			tmp[ROWS*1+row] = io[COLS*row+E1];
			tmp[ROWS*2+row] = io[COLS*row+E2];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
};

template <typename TYPE, int N, int E0, int E1, int E2, int E3, int E4, int E5, int E6, int E7>
struct ITL8 : public Interleaver<TYPE>
{
	static const int COLS = 8;
	static const int ROWS = N / COLS;
	TYPE tmp[N];
	void fwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[COLS*row+E0] = io[ROWS*0+row];
			tmp[COLS*row+E1] = io[ROWS*1+row];
			tmp[COLS*row+E2] = io[ROWS*2+row];
			tmp[COLS*row+E3] = io[ROWS*3+row];
			tmp[COLS*row+E4] = io[ROWS*4+row];
			tmp[COLS*row+E5] = io[ROWS*5+row];
			tmp[COLS*row+E6] = io[ROWS*6+row];
			tmp[COLS*row+E7] = io[ROWS*7+row];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
	void bwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[ROWS*0+row] = io[COLS*row+E0];
			tmp[ROWS*1+row] = io[COLS*row+E1];
			tmp[ROWS*2+row] = io[COLS*row+E2];
			tmp[ROWS*3+row] = io[COLS*row+E3];
			tmp[ROWS*4+row] = io[COLS*row+E4];
			tmp[ROWS*5+row] = io[COLS*row+E5];
			tmp[ROWS*6+row] = io[COLS*row+E6];
			tmp[ROWS*7+row] = io[COLS*row+E7];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
};

template <typename TYPE, int N, int E0, int E1, int E2, int E3, int E4, int E5, int E6, int E7, int E8, int E9, int E10, int E11>
struct ITL12 : public Interleaver<TYPE>
{
	static const int COLS = 12;
	static const int ROWS = N / COLS;
	TYPE tmp[N];
	void fwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[COLS*row+E0] = io[ROWS*0+row];
			tmp[COLS*row+E1] = io[ROWS*1+row];
			tmp[COLS*row+E2] = io[ROWS*2+row];
			tmp[COLS*row+E3] = io[ROWS*3+row];
			tmp[COLS*row+E4] = io[ROWS*4+row];
			tmp[COLS*row+E5] = io[ROWS*5+row];
			tmp[COLS*row+E6] = io[ROWS*6+row];
			tmp[COLS*row+E7] = io[ROWS*7+row];
			tmp[COLS*row+E8] = io[ROWS*8+row];
			tmp[COLS*row+E9] = io[ROWS*9+row];
			tmp[COLS*row+E10] = io[ROWS*10+row];
			tmp[COLS*row+E11] = io[ROWS*11+row];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
	void bwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[ROWS*0+row] = io[COLS*row+E0];
			tmp[ROWS*1+row] = io[COLS*row+E1];
			tmp[ROWS*2+row] = io[COLS*row+E2];
			tmp[ROWS*3+row] = io[COLS*row+E3];
			tmp[ROWS*4+row] = io[COLS*row+E4];
			tmp[ROWS*5+row] = io[COLS*row+E5];
			tmp[ROWS*6+row] = io[COLS*row+E6];
			tmp[ROWS*7+row] = io[COLS*row+E7];
			tmp[ROWS*8+row] = io[COLS*row+E8];
			tmp[ROWS*9+row] = io[COLS*row+E9];
			tmp[ROWS*10+row] = io[COLS*row+E10];
			tmp[ROWS*11+row] = io[COLS*row+E11];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
};

template <typename TYPE, int N, int E0, int E1, int E2, int E3, int E4, int E5, int E6, int E7, int E8, int E9, int E10, int E11, int E12, int E13, int E14, int E15>
struct ITL16 : public Interleaver<TYPE>
{
	static const int COLS = 16;
	static const int ROWS = N / COLS;
	TYPE tmp[N];
	void fwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[COLS*row+E0] = io[ROWS*0+row];
			tmp[COLS*row+E1] = io[ROWS*1+row];
			tmp[COLS*row+E2] = io[ROWS*2+row];
			tmp[COLS*row+E3] = io[ROWS*3+row];
			tmp[COLS*row+E4] = io[ROWS*4+row];
			tmp[COLS*row+E5] = io[ROWS*5+row];
			tmp[COLS*row+E6] = io[ROWS*6+row];
			tmp[COLS*row+E7] = io[ROWS*7+row];
			tmp[COLS*row+E8] = io[ROWS*8+row];
			tmp[COLS*row+E9] = io[ROWS*9+row];
			tmp[COLS*row+E10] = io[ROWS*10+row];
			tmp[COLS*row+E11] = io[ROWS*11+row];
			tmp[COLS*row+E12] = io[ROWS*12+row];
			tmp[COLS*row+E13] = io[ROWS*13+row];
			tmp[COLS*row+E14] = io[ROWS*14+row];
			tmp[COLS*row+E15] = io[ROWS*15+row];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
	void bwd(TYPE *io)
	{
		for (int row = 0; row < ROWS; ++row) {
			tmp[ROWS*0+row] = io[COLS*row+E0];
			tmp[ROWS*1+row] = io[COLS*row+E1];
			tmp[ROWS*2+row] = io[COLS*row+E2];
			tmp[ROWS*3+row] = io[COLS*row+E3];
			tmp[ROWS*4+row] = io[COLS*row+E4];
			tmp[ROWS*5+row] = io[COLS*row+E5];
			tmp[ROWS*6+row] = io[COLS*row+E6];
			tmp[ROWS*7+row] = io[COLS*row+E7];
			tmp[ROWS*8+row] = io[COLS*row+E8];
			tmp[ROWS*9+row] = io[COLS*row+E9];
			tmp[ROWS*10+row] = io[COLS*row+E10];
			tmp[ROWS*11+row] = io[COLS*row+E11];
			tmp[ROWS*12+row] = io[COLS*row+E12];
			tmp[ROWS*13+row] = io[COLS*row+E13];
			tmp[ROWS*14+row] = io[COLS*row+E14];
			tmp[ROWS*15+row] = io[COLS*row+E15];
		}
		for (int n = 0; n < N; ++n)
			io[n] = tmp[n];
	}
};

#endif

