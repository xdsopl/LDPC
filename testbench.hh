/*
LDPC testbench

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#include <cstdint>
#include <complex>

#ifdef __AVX2__
#include <immintrin.h>
typedef __m256 float_simd_type;
typedef __m256i int_simd_type;
#endif

#ifdef __ARM_NEON__
#include <arm_neon.h>
typedef int8x16_t int_simd_type;
#endif

typedef float value_type;
typedef std::complex<value_type> complex_type;

#if 1
typedef int8_t code_type;
typedef code_type simd_type;
//typedef int_simd_type simd_type;
const int FACTOR = 2;
#else
typedef float code_type;
typedef code_type simd_type;
//typedef float_simd_type simd_type;
const int FACTOR = 1;
#endif

