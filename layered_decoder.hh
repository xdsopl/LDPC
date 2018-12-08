/*
LDPC SISO layered decoder

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef LAYERED_DECODER_HH
#define LAYERED_DECODER_HH

#include <stdlib.h>
#include "ldpc.hh"

template <typename TYPE, typename ALG>
class LDPCDecoder
{
	TYPE *bnl, *pty;
	uint16_t *pos;
	uint8_t *cnc;
	ALG alg;
	int M, N, K, R, q, CNL, LT;
	bool initialized;

	void reset()
	{
		for (int i = 0; i < LT; ++i)
			bnl[i] = alg.zero();
	}
	bool bad(TYPE *data, TYPE *parity, int blocks)
	{
		{
			int cnt = cnc[0];
			TYPE cnv = alg.sign(alg.one(), parity[0]);
			for (int c = 0; c < cnt; ++c)
				cnv = alg.sign(cnv, data[pos[c]]);
			if (alg.bad(cnv, blocks))
				return true;
		}
		for (int j = 1; j < M; ++j) {
			int cnt = cnc[0];
			TYPE cnv = alg.sign(alg.sign(alg.one(), parity[j+(q-1)*M-1]), parity[j]);
			for (int c = 0; c < cnt; ++c)
				cnv = alg.sign(cnv, data[pos[CNL*j+c]]);
			if (alg.bad(cnv, blocks))
				return true;
		}
		for (int i = 1; i < q; ++i) {
			for (int j = 0; j < M; ++j) {
				int cnt = cnc[i];
				TYPE cnv = alg.sign(alg.sign(alg.one(), parity[M*(i-1)+j]), parity[M*i+j]);
				for (int c = 0; c < cnt; ++c)
					cnv = alg.sign(cnv, data[pos[CNL*(M*i+j)+c]]);
				if (alg.bad(cnv, blocks))
					return true;
			}
		}
		return false;
	}
	void update(TYPE *data, TYPE *parity)
	{
		TYPE *bl = bnl;
		{
			int cnt = cnc[0];
			int deg = cnt + 1;
			TYPE inp[deg], out[deg];
			for (int c = 0; c < cnt; ++c)
				inp[c] = out[c] = alg.sub(data[pos[c]], bl[c]);
			inp[cnt] = out[cnt] = alg.sub(parity[0], bl[cnt]);
			alg.finalp(out, deg);
			for (int c = 0; c < cnt; ++c)
				data[pos[c]] = alg.add(inp[c], out[c]);
			parity[0] = alg.add(inp[cnt], out[cnt]);
			for (int d = 0; d < deg; ++d)
				alg.update(bl++, out[d]);
		}
		for (int j = 1; j < M; ++j) {
			int cnt = cnc[0];
			int deg = cnt + 2;
			TYPE inp[deg], out[deg];
			for (int c = 0; c < cnt; ++c)
				inp[c] = out[c] = alg.sub(data[pos[CNL*j+c]], bl[c]);
			inp[cnt] = out[cnt] = alg.sub(parity[j+(q-1)*M-1], bl[cnt]);
			inp[cnt+1] = out[cnt+1] = alg.sub(parity[j], bl[cnt+1]);
			alg.finalp(out, deg);
			for (int c = 0; c < cnt; ++c)
				data[pos[CNL*j+c]] = alg.add(inp[c], out[c]);
			parity[j+(q-1)*M-1] = alg.add(inp[cnt], out[cnt]);
			parity[j] = alg.add(inp[cnt+1], out[cnt+1]);
			for (int d = 0; d < deg; ++d)
				alg.update(bl++, out[d]);
		}
		for (int i = 1; i < q; ++i) {
			for (int j = 0; j < M; ++j) {
				int cnt = cnc[i];
				int deg = cnt + 2;
				TYPE inp[deg], out[deg];
				for (int c = 0; c < cnt; ++c)
					inp[c] = out[c] = alg.sub(data[pos[CNL*(M*i+j)+c]], bl[c]);
				inp[cnt] = out[cnt] = alg.sub(parity[M*(i-1)+j], bl[cnt]);
				inp[cnt+1] = out[cnt+1] = alg.sub(parity[M*i+j], bl[cnt+1]);
				alg.finalp(out, deg);
				for (int c = 0; c < cnt; ++c)
					data[pos[CNL*(M*i+j)+c]] = alg.add(inp[c], out[c]);
				parity[M*(i-1)+j] = alg.add(inp[cnt], out[cnt]);
				parity[M*i+j] = alg.add(inp[cnt+1], out[cnt+1]);
				for (int d = 0; d < deg; ++d)
					alg.update(bl++, out[d]);
			}
		}
	}
public:
	LDPCDecoder() : initialized(false)
	{
	}
	void init(LDPCInterface *it)
	{
		if (initialized) {
			free(bnl);
			free(pty);
			delete[] cnc;
			delete[] pos;
		}
		initialized = true;
		LDPCInterface *ldpc = it->clone();
		N = ldpc->code_len();
		K = ldpc->data_len();
		M = ldpc->group_len();
		R = N - K;
		q = R / M;
		CNL = ldpc->links_max_cn() - 2;
		pos = new uint16_t[R * CNL];
		cnc = new uint8_t[R];
		for (int i = 0; i < R; ++i)
			cnc[i] = 0;
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int *acc_pos = ldpc->acc_pos();
			int bit_deg = ldpc->bit_deg();
			for (int n = 0; n < bit_deg; ++n) {
				int i = acc_pos[n];
				pos[CNL*i+cnc[i]++] = j;
			}
			ldpc->next_bit();
		}
		LT = ldpc->links_total();
		delete ldpc;
		bnl = reinterpret_cast<TYPE *>(aligned_alloc(sizeof(TYPE), sizeof(TYPE) * LT));
		pty = reinterpret_cast<TYPE *>(aligned_alloc(sizeof(TYPE), sizeof(TYPE) * R));
		uint16_t *tmp = new uint16_t[R * CNL];
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				for (int c = 0; c < CNL; ++c)
					tmp[CNL*(M*i+j)+c] = pos[CNL*(q*j+i)+c];
		delete[] pos;
		pos = tmp;
	}
	int operator()(TYPE *data, TYPE *parity, int trials = 25, int blocks = 1)
	{
		reset();
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				pty[M*i+j] = parity[q*j+i];
		while (bad(data, pty, blocks) && --trials >= 0)
			update(data, pty);
		for (int i = 0; i < q; ++i)
			for (int j = 0; j < M; ++j)
				parity[q*j+i] = pty[M*i+j];
		return trials;
	}
	~LDPCDecoder()
	{
		if (initialized) {
			free(bnl);
			free(pty);
			delete[] cnc;
			delete[] pos;
		}
	}
};

#endif
