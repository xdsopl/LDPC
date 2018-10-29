/*
Phase-shift keying

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef PSK_HH
#define PSK_HH

#include "modulation.hh"

template <int NUM, typename TYPE, typename CODE>
struct PhaseShiftKeying;

template <typename TYPE, typename CODE>
struct PhaseShiftKeying<2, TYPE, CODE> : public Modulation<TYPE, CODE>
{
	static const int NUM = 2;
	static const int BITS = 1;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	typedef CODE code_type;

	static constexpr value_type DIST = 2;

	static code_type quantize(value_type precision, value_type value)
	{
		value *= DIST * precision;
		if (std::is_integral<code_type>::value)
			value = std::nearbyint(value);
		if (std::is_same<code_type, int8_t>::value)
			value = std::min<value_type>(std::max<value_type>(value, -128), 127);
		return value;
	}

	int bits()
	{
		return BITS;
	}

	void hard(code_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
	}

	void soft(code_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = quantize(precision, c.real());
	}

	complex_type map(code_type *b, int stride = 1)
	{
		return complex_type(b[0*stride], 0);
	}
};

template <typename TYPE, typename CODE>
struct PhaseShiftKeying<4, TYPE, CODE> : public Modulation<TYPE, CODE>
{
	static const int NUM = 4;
	static const int BITS = 2;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	typedef CODE code_type;

	// 1/sqrt(2)
	static constexpr value_type rcp_sqrt_2 = 0.70710678118654752440;
	static constexpr value_type DIST = 2 * rcp_sqrt_2;

	static code_type quantize(value_type precision, value_type value)
	{
		value *= DIST * precision;
		if (std::is_integral<code_type>::value)
			value = std::nearbyint(value);
		if (std::is_same<code_type, int8_t>::value)
			value = std::min<value_type>(std::max<value_type>(value, -128), 127);
		return value;
	}

	int bits()
	{
		return BITS;
	}

	void hard(code_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < value_type(0) ? value_type(-1) : value_type(1);
	}

	void soft(code_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = quantize(precision, c.real());
		b[1*stride] = quantize(precision, c.imag());
	}

	complex_type map(code_type *b, int stride = 1)
	{
		return rcp_sqrt_2 * complex_type(b[0*stride], b[1*stride]);
	}
};

template <typename TYPE, typename CODE>
struct PhaseShiftKeying<8, TYPE, CODE> : public Modulation<TYPE, CODE>
{
	static const int NUM = 8;
	static const int BITS = 3;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	typedef CODE code_type;

	// c(a(1)/2)
	static constexpr value_type cos_pi_8 = 0.92387953251128675613;
	// s(a(1)/2)
	static constexpr value_type sin_pi_8 = 0.38268343236508977173;
	// 1/sqrt(2)
	static constexpr value_type rcp_sqrt_2 = 0.70710678118654752440;

	static constexpr value_type DIST = 2 * sin_pi_8;

	static code_type quantize(value_type precision, value_type value)
	{
		value *= DIST * precision;
		if (std::is_integral<code_type>::value)
			value = std::nearbyint(value);
		if (std::is_same<code_type, int8_t>::value)
			value = std::min<value_type>(std::max<value_type>(value, -128), 127);
		return value;
	}

	int bits()
	{
		return BITS;
	}

	void hard(code_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < value_type(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < value_type(0) ? value_type(-1) : value_type(1);
		b[2*stride] = abs(c.real()) < abs(c.imag()) ? value_type(-1) : value_type(1);
	}

	void soft(code_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = quantize(precision, c.real());
		b[1*stride] = quantize(precision, c.imag());
		b[2*stride] = quantize(precision, rcp_sqrt_2 * (abs(c.real()) - abs(c.imag())));
	}

	complex_type map(code_type *b, int stride = 1)
	{
		value_type real = cos_pi_8;
		value_type imag = sin_pi_8;
		if (b[2*stride] < value_type(0))
			std::swap(real, imag);
		return complex_type(real * b[0*stride], imag * b[1*stride]);
	}
};

#endif

