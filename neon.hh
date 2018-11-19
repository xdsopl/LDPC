/*
NEON accelerated LDPC algorithms

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef NEON_HH
#define NEON_HH

#include <arm_neon.h>
#include "exclusive_reduce.hh"

template <>
struct SelfCorrectedUpdate<int8x16_t>
{
	static int8x16_t update(int8x16_t a, int8x16_t b)
	{
		return vandq_s8(b, (int8x16_t)vorrq_u8(
			vceqq_s8(a, vdupq_n_s8(0)),
			veorq_u8(
				vcgtq_s8(a, vdupq_n_s8(0)),
				vcgtq_s8(vdupq_n_s8(0), b))));
	}
};

template <typename UPDATE>
struct MinSumAlgorithm<int8x16_t, UPDATE>
{
	static const int SIMD_WIDTH = 16;
	static int8x16_t one()
	{
		return vdupq_n_s8(1);
	}
	static int8x16_t add(int8x16_t a, int8x16_t b)
	{
		return vqaddq_s8(a, b);
	}
	static int8x16_t sign(int8x16_t a, int8x16_t b)
	{
		return vorrq_s8(
			vandq_s8((int8x16_t)vcgtq_s8(vdupq_n_s8(0), b), vnegq_s8(a)),
			vandq_s8((int8x16_t)vcgtq_s8(b, vdupq_n_s8(0)), a));
	}
	static void finalp(int8x16_t *links, int cnt)
	{
		int8x16_t mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = vqabsq_s8(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, vminq_s8);

		int8x16_t signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, veorq_s8);
		for (int i = 0; i < cnt; ++i)
			signs[i] = vorrq_s8(signs[i], vdupq_n_s8(127));

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(mins[i], signs[i]);
	}
	static bool bad(int8x16_t v, int blocks)
	{
		uint8x16_t tmp = vcgtq_s8(v, vdupq_n_s8(0));
		for (int i = 0; i < blocks; ++i)
			if (!reinterpret_cast<uint8_t *>(&tmp)[i])
				return true;
		return false;
	}
	static int8x16_t update(int8x16_t a, int8x16_t b)
	{
		return UPDATE::update(a, b);
	}
};

#endif
