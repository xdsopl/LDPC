/*
LDPC SISO encoder and decoder

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef LDPC_HH
#define LDPC_HH

#include "exclusive_reduce.hh"

struct LDPCInterface
{
	virtual int code_len() = 0;
	virtual int data_len() = 0;
	virtual int links_total() = 0;
	virtual int links_max_cn() = 0;
	virtual int bit_deg() = 0;
	virtual int *acc_pos() = 0;
	virtual void first_bit() = 0;
	virtual void next_bit() = 0;
	virtual ~LDPCInterface() = default;
};

template <typename TABLE>
class LDPC : public LDPCInterface
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;

	int acc_pos_[TABLE::DEG_MAX];
	const int *row_ptr;
	int bit_deg_;
	int grp_num;
	int grp_len;
	int grp_cnt;
	int row_cnt;

	void next_group()
	{
		if (grp_cnt >= grp_len) {
			grp_len = TABLE::LEN[grp_num];
			bit_deg_ = TABLE::DEG[grp_num];
			grp_cnt = 0;
			++grp_num;
		}
		for (int i = 0; i < bit_deg_; ++i)
			acc_pos_[i] = row_ptr[i];
		row_ptr += bit_deg_;
		++grp_cnt;
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
	int links_total()
	{
		return TABLE::LINKS_TOTAL;
	}
	int links_max_cn()
	{
		return TABLE::LINKS_MAX_CN;
	}
	int bit_deg()
	{
		return bit_deg_;
	}
	int *acc_pos()
	{
		return acc_pos_;
	}
	void next_bit()
	{
		if (++row_cnt < M) {
			for (int i = 0; i < bit_deg_; ++i)
				acc_pos_[i] += q;
			for (int i = 0; i < bit_deg_; ++i)
				acc_pos_[i] %= R;
		} else {
			next_group();
			row_cnt = 0;
		}
	}
	void first_bit()
	{
		grp_num = 0;
		grp_len = 0;
		grp_cnt = 0;
		row_cnt = 0;
		row_ptr = TABLE::POS;
		next_group();
	}
};

template <typename TYPE, typename ALG>
class LDPCDecoder
{
	TYPE *bnl, *bnv, *cnl, *cnv;
	uint8_t *cnc;
	LDPCInterface *ldpc;
	ALG alg;
	int N, K, R, CNL;

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
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int bit_deg = ldpc->bit_deg();
			for (int n = 0; n < bit_deg; ++n)
				*bl++ = data[j];
			ldpc->next_bit();
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
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int *acc_pos = ldpc->acc_pos();
			int bit_deg = ldpc->bit_deg();
			for (int n = 0; n < bit_deg; ++n) {
				int i = acc_pos[n];
				cnv[i] = alg.sign(cnv[i], bnv[j+R]);
				cnl[CNL*i+cnc[i]++] = *bl++;
			}
			ldpc->next_bit();
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
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int *acc_pos = ldpc->acc_pos();
			int bit_deg = ldpc->bit_deg();
			TYPE inp[bit_deg];
			for (int n = 0; n < bit_deg; ++n) {
				int i = acc_pos[n];
				inp[n] = cnl[CNL*i+cnc[i]++];
			}
			TYPE out[bit_deg];
			CODE::exclusive_reduce(inp, out, bit_deg, alg.add);
			bnv[j+R] = alg.add(data[j], alg.add(out[0], inp[0]));
			for (int n = 0; n < bit_deg; ++n, ++bl)
				*bl = alg.update(*bl, alg.add(data[j], out[n]));
			ldpc->next_bit();
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
	LDPCDecoder(LDPCInterface *ldpc) : ldpc(ldpc)
	{
		N = ldpc->code_len();
		K = ldpc->data_len();
		R = N - K;
		CNL = ldpc->links_max_cn();
		bnl = new TYPE[ldpc->links_total()];
		bnv = new TYPE[N];
		cnl = new TYPE[R * CNL];
		cnv = new TYPE[R];
		cnc = new uint8_t[R];
	}
	int operator()(TYPE *data, TYPE *parity, int trials = 50, int blocks = 1)
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
	~LDPCDecoder()
	{
		delete[] bnl;
		delete[] bnv;
		delete[] cnl;
		delete[] cnv;
		delete[] cnc;
	}
};

template <typename TYPE>
class LDPCEncoder
{
	LDPCInterface *ldpc;
	int N, K, R;

	TYPE one()
	{
		return 1;
	}
	TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
public:
	LDPCEncoder(LDPCInterface *ldpc) : ldpc(ldpc)
	{
		N = ldpc->code_len();
		K = ldpc->data_len();
		R = N - K;
	}
	void operator()(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			parity[i] = one();
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int *acc_pos = ldpc->acc_pos();
			int bit_deg = ldpc->bit_deg();
			for (int n = 0; n < bit_deg; ++n) {
				int i = acc_pos[n];
				parity[i] = sign(parity[i], data[j]);
			}
			ldpc->next_bit();
		}
		for (int i = 1; i < R; ++i)
			parity[i] = sign(parity[i], parity[i-1]);
	}
};

#endif
