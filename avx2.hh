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
			_mm256_xor_si256(
				_mm256_cmpgt_epi8(a, _mm256_setzero_si256()),
				_mm256_cmpgt_epi8(_mm256_setzero_si256(), b))));
	}
};

template <>
struct SelfCorrectedUpdate<__m256>
{
	static __m256 update(__m256 a, __m256 b)
	{
		return _mm256_and_ps(b, _mm256_or_ps(
			_mm256_cmp_ps(a, _mm256_setzero_ps(), _CMP_EQ_OQ),
			_mm256_xor_ps(
				_mm256_cmp_ps(a, _mm256_setzero_ps(), _CMP_GT_OQ),
				_mm256_cmp_ps(_mm256_setzero_ps(), b, _CMP_GT_OQ))));
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

template <typename UPDATE>
struct MinSumAlgorithm<__m256, UPDATE>
{
	static __m256 one()
	{
		return _mm256_set1_ps(1.f);
	}
	static __m256 add(__m256 a, __m256 b)
	{
		return _mm256_add_ps(a, b);
	}
	static __m256 sign(__m256 a, __m256 b)
	{
		return _mm256_andnot_ps(
			_mm256_cmp_ps(b, _mm256_setzero_ps(), _CMP_EQ_OQ),
			_mm256_xor_ps(a, _mm256_and_ps(_mm256_set1_ps(-0.f), b)));
	}
	static void finalp(__m256 *links, int cnt)
	{
		__m256 mask = _mm256_set1_ps(-0.f);
		__m256 mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = _mm256_andnot_ps(mask, links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, _mm256_min_ps);

		__m256 signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, _mm256_xor_ps);
		for (int i = 0; i < cnt; ++i)
			signs[i] = _mm256_and_ps(mask, signs[i]);

		for (int i = 0; i < cnt; ++i)
			links[i] = _mm256_or_ps(signs[i], mins[i]);
	}
	static bool bad(__m256 v, int blocks)
	{
		__m256 tmp = _mm256_cmp_ps(v, _mm256_setzero_ps(), _CMP_GT_OQ);
		for (int i = 0; i < blocks; ++i)
			if (!reinterpret_cast<int *>(&tmp)[i])
				return true;
		return false;
	}
	static __m256 update(__m256 a, __m256 b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename UPDATE, int FACTOR>
struct MinSumCAlgorithm<__m256i, UPDATE, FACTOR>
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
	static __m256i correction_factor(__m256i a, __m256i b)
	{
		__m256i factor2 = _mm256_set1_epi8(FACTOR * 2);
		__m256i pc = _mm256_set1_epi8(FACTOR / 2);
		__m256i nc = _mm256_set1_epi8(-FACTOR / 2);
		__m256i zero = _mm256_setzero_si256();
		__m256i apb = _mm256_abs_epi8(_mm256_adds_epi8(a, b));
		__m256i apb2 = _mm256_adds_epu8(apb, apb);
		__m256i amb = _mm256_abs_epi8(_mm256_subs_epi8(a, b));
		__m256i amb2 = _mm256_adds_epu8(amb, amb);
		pc = _mm256_andnot_si256(_mm256_and_si256(
			_mm256_cmpeq_epi8(zero, _mm256_subs_epu8(factor2, apb)),
			_mm256_cmpeq_epi8(zero, _mm256_subs_epu8(amb, apb2))),
			pc);
		nc = _mm256_andnot_si256(_mm256_and_si256(
			_mm256_cmpeq_epi8(zero, _mm256_subs_epu8(factor2, amb)),
			_mm256_cmpeq_epi8(zero, _mm256_subs_epu8(apb, amb2))),
			nc);
		return _mm256_or_si256(pc, nc);
	}
	static __m256i min(__m256i a, __m256i b)
	{
		__m256i m = _mm256_min_epu8(_mm256_abs_epi8(a), _mm256_abs_epi8(b));
		m = _mm256_min_epu8(m, _mm256_set1_epi8(127));
		__m256i x = _mm256_sign_epi8(_mm256_sign_epi8(m, a), b);
		x = _mm256_adds_epi8(x, correction_factor(a, b));
		return x;
	}
	static void finalp(__m256i *links, int cnt)
	{
		__m256i tmp[cnt];
		CODE::exclusive_reduce(links, tmp, cnt, min);
		for (int i = 0; i < cnt; ++i)
			links[i] = tmp[i];
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
