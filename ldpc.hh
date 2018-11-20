/*
LDPC SISO encoder and decoder

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef LDPC_HH
#define LDPC_HH

#include "exclusive_reduce.hh"

template <typename TYPE>
struct LDPCInterface
{
	virtual int code_len() = 0;
	virtual int data_len() = 0;
	virtual void encode(TYPE *, TYPE *) = 0;
	virtual int decode(TYPE *, TYPE *, int = 50, int = 1) = 0;
	virtual ~LDPCInterface() = default;
};

template <typename TABLE>
struct AccumulatorPositions
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	int pos[TABLE::DEG_MAX];
	const int *row_ptr;
	int bit_deg;
	int grp_num;
	int grp_len;
	int grp_cnt;

	void next_group()
	{
		if (grp_cnt >= grp_len) {
			grp_len = TABLE::LEN[grp_num];
			bit_deg = TABLE::DEG[grp_num];
			grp_cnt = 0;
			++grp_num;
		}
		for (int i = 0; i < bit_deg; ++i)
			pos[i] = row_ptr[i];
		row_ptr += bit_deg;
		++grp_cnt;
	}
	void next_bit()
	{
		for (int i = 0; i < bit_deg; ++i)
			pos[i] += q;
		for (int i = 0; i < bit_deg; ++i)
			pos[i] %= R;
	}
	void first_group()
	{
		grp_num = 0;
		grp_len = 0;
		grp_cnt = 0;
		row_ptr = TABLE::POS;
		next_group();
	}
};

template <typename TABLE, typename TYPE, typename ALG>
class LDPC : public LDPCInterface<TYPE>
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int CNL = TABLE::LINKS_MAX_CN;

	TYPE bnl[TABLE::LINKS_TOTAL];
	TYPE bnv[N];
	TYPE cnl[R * CNL];
	TYPE cnv[R];
	uint8_t cnc[R];
	ALG alg;
	AccumulatorPositions<TABLE> acc;

	void bit_node_init(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			bnv[i] = parity[i];
		for (int i = 0; i < K; ++i)
			bnv[i+R] = data[i];

		TYPE *bl = bnl;
		for (int i = 0; i < R-1; ++i) {
			*bl++ = parity[i];
			*bl++ = parity[i];
		}
		*bl++ = parity[R-1];
		acc.first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < acc.bit_deg; ++n)
					*bl++ = data[j+m];
				acc.next_bit();
			}
			acc.next_group();
		}
	}
	void check_node_update()
	{
		cnv[0] = alg.sign(alg.one(), bnv[0]);
		for (int i = 1; i < R; ++i)
			cnv[i] = alg.sign(alg.sign(alg.one(), bnv[i-1]), bnv[i]);

		TYPE *bl = bnl;
		cnl[0] = *bl++;
		cnc[0] = 1;
		for (int i = 1; i < R; ++i) {
			cnl[CNL*i] = *bl++;
			cnl[CNL*i+1] = *bl++;
			cnc[i] = 2;
		}
		acc.first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < acc.bit_deg; ++n) {
					int i = acc.pos[n];
					cnv[i] = alg.sign(cnv[i], bnv[j+m+R]);
					cnl[CNL*i+cnc[i]++] = *bl++;
				}
				acc.next_bit();
			}
			acc.next_group();
		}
		for (int i = 0; i < R; ++i)
			alg.finalp(cnl+CNL*i, cnc[i]);
	}
	void bit_node_update(TYPE *data, TYPE *parity)
	{
		bnv[0] = alg.add(parity[0], alg.add(cnl[0], cnl[CNL]));
		for (int i = 1; i < R-1; ++i)
			bnv[i] = alg.add(parity[i], alg.add(cnl[CNL*i+1], cnl[CNL*(i+1)]));
		bnv[R-1] = alg.add(parity[R-1], cnl[CNL*(R-1)+1]);

		TYPE *bl = bnl;
		cnc[0] = 1;
		for (int i = 1; i < R; ++i)
			cnc[i] = 2;
		*bl = alg.update(*bl, alg.add(parity[0], cnl[CNL])); ++bl;
		*bl = alg.update(*bl, alg.add(parity[0], cnl[0])); ++bl;
		for (int i = 1; i < R-1; ++i) {
			*bl = alg.update(*bl, alg.add(parity[i], cnl[CNL*(i+1)])); ++bl;
			*bl = alg.update(*bl, alg.add(parity[i], cnl[CNL*i+1])); ++bl;
		}
		*bl = alg.update(*bl, parity[R-1]); ++bl;
		acc.first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				TYPE inp[acc.bit_deg];
				for (int n = 0; n < acc.bit_deg; ++n) {
					int i = acc.pos[n];
					inp[n] = cnl[CNL*i+cnc[i]++];
				}
				TYPE out[acc.bit_deg];
				CODE::exclusive_reduce(inp, out, acc.bit_deg, alg.add);
				bnv[j+m+R] = alg.add(data[j+m], alg.add(out[0], inp[0]));
				for (int n = 0; n < acc.bit_deg; ++n, ++bl)
					*bl = alg.update(*bl, alg.add(data[j+m], out[n]));
				acc.next_bit();
			}
			acc.next_group();
		}
	}
	bool hard_decision(int blocks)
	{
		for (int i = 0; i < R; ++i)
			if (alg.bad(cnv[i], blocks))
				return true;
		return false;
	}
	void update_user(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			parity[i] = bnv[i];
		for (int i = 0; i < K; ++i)
			data[i] = bnv[i+R];
	}
public:
	int code_len()
	{
		return N;
	}
	int data_len()
	{
		return K;
	}
	int decode(TYPE *data, TYPE *parity, int trials = 50, int blocks = 1)
	{
		bit_node_init(data, parity);
		check_node_update();
		if (!hard_decision(blocks))
			return trials;
		--trials;
		bit_node_update(data, parity);
		check_node_update();
		while (hard_decision(blocks) && --trials >= 0) {
			bit_node_update(data, parity);
			check_node_update();
		}
		update_user(data, parity);
		return trials;
	}
	void encode(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			parity[i] = alg.one();
		acc.first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < acc.bit_deg; ++n) {
					int i = acc.pos[n];
					parity[i] = alg.sign(parity[i], data[j+m]);
				}
				acc.next_bit();
			}
			acc.next_group();
		}
		for (int i = 1; i < R; ++i)
			parity[i] = alg.sign(parity[i], parity[i-1]);
	}
};

#endif
