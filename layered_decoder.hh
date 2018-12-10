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
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			TYPE buf[deg*M];
			for (int c = 0; c < cnt; ++c) {
				int shift = pos[CNL*i+c] % M;
				int offset = pos[CNL*i+c] - shift;
				for (int j = 0; j < shift; ++j)
					buf[M*c+j+(M-shift)] = data[offset+j];
				for (int j = 0; j < M - shift; ++j)
					buf[M*c+j] = data[offset+shift+j];
			}
			if (i) {
				for (int j = 0; j < M; ++j)
					buf[M*cnt+j] = parity[M*(i-1)+j];
			} else {
				buf[M*cnt] = alg.one();
				for (int j = 1; j < M; ++j)
					buf[M*cnt+j] = parity[j+(q-1)*M-1];
			}
			for (int j = 0; j < M; ++j)
				buf[M*(cnt+1)+j] = parity[M*i+j];
			for (int j = 0; j < M; ++j) {
				TYPE cnv = alg.sign(alg.one(), buf[j]);
				for (int d = 1; d < deg; ++d)
					cnv = alg.sign(cnv, buf[M*d+j]);
				if (alg.bad(cnv, blocks))
					return true;
			}
		}
		return false;
	}
	void update(TYPE *data, TYPE *parity)
	{
		TYPE *bl = bnl;
		for (int i = 0; i < q; ++i) {
			int cnt = cnc[i];
			int deg = cnt + 2;
			TYPE buf[deg*M];
			for (int c = 0; c < cnt; ++c) {
				int shift = pos[CNL*i+c] % M;
				int offset = pos[CNL*i+c] - shift;
				for (int j = 0; j < shift; ++j)
					buf[M*c+j+(M-shift)] = data[offset+j];
				for (int j = 0; j < M - shift; ++j)
					buf[M*c+j] = data[offset+shift+j];
			}
			if (i) {
				for (int j = 0; j < M; ++j)
					buf[M*cnt+j] = parity[M*(i-1)+j];
			} else {
				buf[M*cnt] = alg.one();
				for (int j = 1; j < M; ++j)
					buf[M*cnt+j] = parity[j+(q-1)*M-1];
			}
			for (int j = 0; j < M; ++j)
				buf[M*(cnt+1)+j] = parity[M*i+j];
			for (int j = 0; j < M; ++j) {
				TYPE inp[deg], out[deg];
				for (int d = 0; d < deg; ++d)
					inp[d] = out[d] = alg.sub(buf[M*d+j], bl[d]);
				alg.finalp(out, deg);
				for (int d = 0; d < deg; ++d)
					buf[M*d+j] = alg.add(inp[d], out[d]);
				for (int d = 0; d < deg; ++d)
					alg.update(bl++, out[d]);
			}
			for (int c = 0; c < cnt; ++c) {
				int shift = pos[CNL*i+c] % M;
				int offset = pos[CNL*i+c] - shift;
				for (int j = 0; j < shift; ++j)
					data[offset+j] = buf[M*c+j+(M-shift)];
				for (int j = 0; j < M - shift; ++j)
					data[offset+shift+j] = buf[M*c+j];
			}
			if (i) {
				for (int j = 0; j < M; ++j)
					parity[M*(i-1)+j] = buf[M*cnt+j];
			} else {
				for (int j = 1; j < M; ++j)
					parity[j+(q-1)*M-1] = buf[M*cnt+j];
			}
			for (int j = 0; j < M; ++j)
				parity[M*i+j] = buf[M*(cnt+1)+j];
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
		pos = new uint16_t[q * CNL];
		cnc = new uint8_t[q];
		for (int i = 0; i < q; ++i)
			cnc[i] = 0;
		ldpc->first_bit();
		for (int j = 0; j < K; ++j) {
			int *acc_pos = ldpc->acc_pos();
			int bit_deg = ldpc->bit_deg();
			for (int n = 0; n < bit_deg; ++n) {
				int i = acc_pos[n];
				if (i < q)
					pos[CNL*i+cnc[i]++] = j;
			}
			ldpc->next_bit();
		}
		LT = ldpc->links_total() + 1;
		delete ldpc;
		bnl = reinterpret_cast<TYPE *>(aligned_alloc(sizeof(TYPE), sizeof(TYPE) * LT));
		pty = reinterpret_cast<TYPE *>(aligned_alloc(sizeof(TYPE), sizeof(TYPE) * R));
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
