/*
AVX2 accelerated LDPC algorithms

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef AVX2_HH
#define AVX2_HH

#include <immintrin.h>
#include "exclusive_reduce.hh"

template <>
struct SelfCorrectedUpdate<__m256i>
{
	static __m256i update(__m256i a, __m256i b)
	{
		return _mm256_and_si256(b, _mm256_or_si256(
			_mm256_cmpeq_epi8(a, _mm256_setzero_si256()),
			_mm256_cmpeq_epi8(_mm256_and_si256(_mm256_xor_si256(a, b), _mm256_set1_epi8(-128)), _mm256_setzero_si256())));
	}
};

template <typename UPDATE>
struct MinSumAlgorithm<__m256i, UPDATE>
{
	static __m256i one()
	{
		return _mm256_set1_epi8(1);
	}
	static __m256i add(__m256i a, __m256i b)
	{
		return _mm256_adds_epi8(a, b);
	}
	static __m256i sign(__m256i a, __m256i b)
	{
		return _mm256_sign_epi8(a, b);
	}
	static void finalp(__m256i *links, int cnt)
	{
		__m256i mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = _mm256_abs_epi8(_mm256_max_epi8(links[i], _mm256_set1_epi8(-127)));
		CODE::exclusive_reduce(mags, mins, cnt, _mm256_min_epi8);

		__m256i signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, _mm256_xor_si256);
		for (int i = 0; i < cnt; ++i)
			signs[i] = _mm256_or_si256(signs[i], _mm256_set1_epi8(127));

		for (int i = 0; i < cnt; ++i)
			links[i] = _mm256_sign_epi8(mins[i], signs[i]);
	}
	static bool bad(__m256i v, int blocks)
	{
		__m256i tmp = _mm256_cmpgt_epi8(v, _mm256_setzero_si256());
		for (int i = 0; i < blocks; ++i)
			if (!reinterpret_cast<char *>(&tmp)[i])
				return true;
		return false;
	}
	static __m256i update(__m256i a, __m256i b)
	{
		return UPDATE::update(a, b);
	}
};

#endif
