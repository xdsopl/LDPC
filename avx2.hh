/*
Intel AVX2 acceleration

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef AVX2_HH
#define AVX2_HH

#include <immintrin.h>

template <>
union SIMD<float, 8>
{
	static const int SIZE = 8;
	typedef float value_type;
	__m256 m;
	float v[8];
};

template <>
union SIMD<double, 4>
{
	static const int SIZE = 4;
	typedef double value_type;
	__m256d m;
	double v[4];
};

template <>
union SIMD<int8_t, 32>
{
	static const int SIZE = 32;
	typedef int8_t value_type;
	__m256i m;
	int8_t v[32];
};

template <>
union SIMD<int16_t, 16>
{
	static const int SIZE = 16;
	typedef int16_t value_type;
	__m256i m;
	int16_t v[16];
};

template <>
union SIMD<int32_t, 8>
{
	static const int SIZE = 8;
	typedef int32_t value_type;
	__m256i m;
	int32_t v[8];
};

template <>
union SIMD<int64_t, 4>
{
	static const int SIZE = 4;
	typedef int64_t value_type;
	__m256i m;
	int64_t v[4];
};

template <>
union SIMD<uint8_t, 32>
{
	static const int SIZE = 32;
	typedef uint8_t value_type;
	__m256i m;
	uint8_t v[32];
};

template <>
union SIMD<uint16_t, 16>
{
	static const int SIZE = 16;
	typedef uint16_t value_type;
	__m256i m;
	uint16_t v[16];
};

template <>
union SIMD<uint32_t, 8>
{
	static const int SIZE = 8;
	typedef uint32_t value_type;
	__m256i m;
	uint32_t v[8];
};

template <>
union SIMD<uint64_t, 4>
{
	static const int SIZE = 4;
	typedef uint64_t value_type;
	__m256i m;
	uint64_t v[4];
};

template <>
inline SIMD<float, 8> vdup<SIMD<float, 8>>(float a)
{
	SIMD<float, 8> tmp;
	tmp.m = _mm256_set1_ps(a);
	return tmp;
}

template <>
inline SIMD<double, 4> vdup<SIMD<double, 4>>(double a)
{
	SIMD<double, 4> tmp;
	tmp.m = _mm256_set1_pd(a);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vdup<SIMD<int8_t, 32>>(int8_t a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_set1_epi8(a);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vdup<SIMD<int16_t, 16>>(int16_t a)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_set1_epi16(a);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vdup<SIMD<int32_t, 8>>(int32_t a)
{
	SIMD<int32_t, 8> tmp;
	tmp.m = _mm256_set1_epi32(a);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vdup<SIMD<int64_t, 4>>(int64_t a)
{
	SIMD<int64_t, 4> tmp;
	tmp.m = _mm256_set1_epi64x(a);
	return tmp;
}

template <>
inline SIMD<float, 8> vadd(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m = _mm256_add_ps(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<double, 4> vadd(SIMD<double, 4> a, SIMD<double, 4> b)
{
	SIMD<double, 4> tmp;
	tmp.m = _mm256_add_pd(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vadd(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_add_epi8(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vadd(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_add_epi16(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vadd(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m = _mm256_add_epi32(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vadd(SIMD<int64_t, 4> a, SIMD<int64_t, 4> b)
{
	SIMD<int64_t, 4> tmp;
	tmp.m = _mm256_add_epi64(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vqadd(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_adds_epi8(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vqadd(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_adds_epi16(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<float, 8> vabs(SIMD<float, 8> a)
{
	SIMD<float, 8> tmp;
	tmp.m = _mm256_andnot_ps(_mm256_set1_ps(-0.f), a.m);
	return tmp;
}

template <>
inline SIMD<double, 4> vabs(SIMD<double, 4> a)
{
	SIMD<double, 4> tmp;
	tmp.m = _mm256_andnot_pd(_mm256_set1_pd(-0.), a.m);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vqabs(SIMD<int8_t, 32> a)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_abs_epi8(_mm256_max_epi8(a.m, _mm256_set1_epi8(-INT8_MAX)));
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vqabs(SIMD<int16_t, 16> a)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_abs_epi16(_mm256_max_epi16(a.m, _mm256_set1_epi16(-INT16_MAX)));
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vqabs(SIMD<int32_t, 8> a)
{
	SIMD<int32_t, 8> tmp;
	tmp.m = _mm256_abs_epi32(_mm256_max_epi32(a.m, _mm256_set1_epi32(-INT32_MAX)));
	return tmp;
}

template <>
inline SIMD<float, 8> vsign(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m = _mm256_andnot_ps(
		_mm256_cmp_ps(b.m, _mm256_setzero_ps(), _CMP_EQ_OQ),
		_mm256_xor_ps(a.m, _mm256_and_ps(_mm256_set1_ps(-0.f), b.m)));
	return tmp;
}

template <>
inline SIMD<double, 4> vsign(SIMD<double, 4> a, SIMD<double, 4> b)
{
	SIMD<double, 4> tmp;
	tmp.m = _mm256_andnot_pd(
		_mm256_cmp_pd(b.m, _mm256_setzero_pd(), _CMP_EQ_OQ),
		_mm256_xor_pd(a.m, _mm256_and_pd(_mm256_set1_pd(-0.), b.m)));
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vsign(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_sign_epi8(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vsign(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_sign_epi16(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vsign(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m = _mm256_sign_epi32(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vorr(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_or_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vorr(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_or_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vorr(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_or_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vorr(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_or_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vand(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_and_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vand(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_and_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vand(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_and_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vand(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_and_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> veor(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_xor_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> veor(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_xor_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> veor(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_xor_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> veor(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_xor_si256(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vbic(SIMD<uint8_t, 32> a, SIMD<uint8_t, 32> b)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_andnot_si256(b.m, a.m);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vbic(SIMD<uint16_t, 16> a, SIMD<uint16_t, 16> b)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_andnot_si256(b.m, a.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vbic(SIMD<uint32_t, 8> a, SIMD<uint32_t, 8> b)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_andnot_si256(b.m, a.m);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vbic(SIMD<uint64_t, 4> a, SIMD<uint64_t, 4> b)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_andnot_si256(b.m, a.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceqz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmp_ps(a.m, _mm256_setzero_ps(), _CMP_EQ_OQ);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vceqz(SIMD<double, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmp_pd(a.m, _mm256_setzero_pd(), _CMP_EQ_OQ);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vceqz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_cmpeq_epi8(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vceqz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_cmpeq_epi16(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vceqz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmpeq_epi32(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vceqz(SIMD<int64_t, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmpeq_epi64(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcgtz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmp_ps(a.m, _mm256_setzero_ps(), _CMP_GT_OQ);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vcgtz(SIMD<double, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmp_pd(a.m, _mm256_setzero_pd(), _CMP_GT_OQ);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vcgtz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_cmpgt_epi8(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vcgtz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_cmpgt_epi16(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcgtz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmpgt_epi32(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vcgtz(SIMD<int64_t, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmpgt_epi64(a.m, _mm256_setzero_si256());
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcltz(SIMD<float, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmp_ps(a.m, _mm256_setzero_ps(), _CMP_LT_OQ);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vcltz(SIMD<double, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmp_pd(a.m, _mm256_setzero_pd(), _CMP_LT_OQ);
	return tmp;
}

template <>
inline SIMD<uint8_t, 32> vcltz(SIMD<int8_t, 32> a)
{
	SIMD<uint8_t, 32> tmp;
	tmp.m = _mm256_cmpgt_epi8(_mm256_setzero_si256(), a.m);
	return tmp;
}

template <>
inline SIMD<uint16_t, 16> vcltz(SIMD<int16_t, 16> a)
{
	SIMD<uint16_t, 16> tmp;
	tmp.m = _mm256_cmpgt_epi16(_mm256_setzero_si256(), a.m);
	return tmp;
}

template <>
inline SIMD<uint32_t, 8> vcltz(SIMD<int32_t, 8> a)
{
	SIMD<uint32_t, 8> tmp;
	tmp.m = _mm256_cmpgt_epi32(_mm256_setzero_si256(), a.m);
	return tmp;
}

template <>
inline SIMD<uint64_t, 4> vcltz(SIMD<int64_t, 4> a)
{
	SIMD<uint64_t, 4> tmp;
	tmp.m = _mm256_cmpgt_epi64(_mm256_setzero_si256(), a.m);
	return tmp;
}

template <>
inline SIMD<float, 8> vmin(SIMD<float, 8> a, SIMD<float, 8> b)
{
	SIMD<float, 8> tmp;
	tmp.m = _mm256_min_ps(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<double, 4> vmin(SIMD<double, 4> a, SIMD<double, 4> b)
{
	SIMD<double, 4> tmp;
	tmp.m = _mm256_min_pd(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int8_t, 32> vmin(SIMD<int8_t, 32> a, SIMD<int8_t, 32> b)
{
	SIMD<int8_t, 32> tmp;
	tmp.m = _mm256_min_epi8(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int16_t, 16> vmin(SIMD<int16_t, 16> a, SIMD<int16_t, 16> b)
{
	SIMD<int16_t, 16> tmp;
	tmp.m = _mm256_min_epi16(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int32_t, 8> vmin(SIMD<int32_t, 8> a, SIMD<int32_t, 8> b)
{
	SIMD<int32_t, 8> tmp;
	tmp.m = _mm256_min_epi32(a.m, b.m);
	return tmp;
}

template <>
inline SIMD<int64_t, 4> vmin(SIMD<int64_t, 4> a, SIMD<int64_t, 4> b)
{
	SIMD<int64_t, 4> tmp;
	tmp.m = _mm256_min_epi64(a.m, b.m);
	return tmp;
}

#endif
