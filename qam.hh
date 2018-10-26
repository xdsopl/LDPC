/*
Quadrature amplitude modulation

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef QAM_HH
#define QAM_HH

#include "modulation.hh"

template <int NUM, typename TYPE>
struct QuadratureAmplitudeModulation;

template <typename TYPE>
struct QuadratureAmplitudeModulation<16, TYPE> : public Modulation<TYPE>
{
	static const int NUM = 16;
	static const int BITS = 4;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;

	static constexpr value_type FAC = 1.0540925533894596;
	static constexpr value_type RCP = 3 * FAC;
	static constexpr value_type AMP = 1 / RCP;
	static constexpr value_type DIST = 2 * AMP;

	static constexpr value_type amp(int i)
	{
		return AMP * i;
	}

	int bits()
	{
		return BITS;
	}

	void hard(value_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < amp(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < amp(0) ? value_type(-1) : value_type(1);
		b[2*stride] = abs(c.real()) < amp(2) ? value_type(-1) : value_type(1);
		b[3*stride] = abs(c.imag()) < amp(2) ? value_type(-1) : value_type(1);
	}

	void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = DIST * precision * c.real();
		b[1*stride] = DIST * precision * c.imag();
		b[2*stride] = DIST * precision * (abs(c.real())-amp(2));
		b[3*stride] = DIST * precision * (abs(c.imag())-amp(2));
	}

	complex_type map(value_type *b, int stride = 1)
	{
		return AMP * complex_type(
			b[0*stride]*(b[2*stride]+value_type(2)),
			b[1*stride]*(b[3*stride]+value_type(2))
		);
	}
};

template <typename TYPE>
struct QuadratureAmplitudeModulation<64, TYPE> : public Modulation<TYPE>
{
	static const int NUM = 64;
	static const int BITS = 6;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;

	static constexpr value_type FAC = 0.9258200997725516;
	static constexpr value_type RCP = 7 * FAC;
	static constexpr value_type AMP = 1 / RCP;
	static constexpr value_type DIST = 2 * AMP;

	static constexpr value_type amp(int i)
	{
		return AMP * i;
	}

	int bits()
	{
		return BITS;
	}

	void hard(value_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < amp(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < amp(0) ? value_type(-1) : value_type(1);
		b[2*stride] = abs(c.real()) < amp(4) ? value_type(-1) : value_type(1);
		b[3*stride] = abs(c.imag()) < amp(4) ? value_type(-1) : value_type(1);
		b[4*stride] = abs(abs(c.real())-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
		b[5*stride] = abs(abs(c.imag())-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
	}

	void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = DIST * precision * c.real();
		b[1*stride] = DIST * precision * c.imag();
		b[2*stride] = DIST * precision * (abs(c.real())-amp(4));
		b[3*stride] = DIST * precision * (abs(c.imag())-amp(4));
		b[4*stride] = DIST * precision * (abs(abs(c.real())-amp(4))-amp(2));
		b[5*stride] = DIST * precision * (abs(abs(c.imag())-amp(4))-amp(2));
	}

	complex_type map(value_type *b, int stride = 1)
	{
		return AMP * complex_type(
			b[0*stride]*(b[2*stride]*(b[4*stride]+value_type(2))+value_type(4)),
			b[1*stride]*(b[3*stride]*(b[5*stride]+value_type(2))+value_type(4))
		);
	}
};

template <typename TYPE>
struct QuadratureAmplitudeModulation<256, TYPE> : public Modulation<TYPE>
{
	static const int NUM = 256;
	static const int BITS = 8;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;

	static constexpr value_type FAC = 0.8692269873603529;
	static constexpr value_type RCP = 15 * FAC;
	static constexpr value_type AMP = 1 / RCP;
	static constexpr value_type DIST = 2 * AMP;

	static constexpr value_type amp(int i)
	{
		return AMP * i;
	}

	int bits()
	{
		return BITS;
	}

	void hard(value_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < amp(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < amp(0) ? value_type(-1) : value_type(1);
		b[2*stride] = abs(c.real()) < amp(8) ? value_type(-1) : value_type(1);
		b[3*stride] = abs(c.imag()) < amp(8) ? value_type(-1) : value_type(1);
		b[4*stride] = abs(abs(c.real())-amp(8)) < amp(4) ? value_type(-1) : value_type(1);
		b[5*stride] = abs(abs(c.imag())-amp(8)) < amp(4) ? value_type(-1) : value_type(1);
		b[6*stride] = abs(abs(abs(c.real())-amp(8))-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
		b[7*stride] = abs(abs(abs(c.imag())-amp(8))-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
	}

	void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = DIST * precision * c.real();
		b[1*stride] = DIST * precision * c.imag();
		b[2*stride] = DIST * precision * (abs(c.real())-amp(8));
		b[3*stride] = DIST * precision * (abs(c.imag())-amp(8));
		b[4*stride] = DIST * precision * (abs(abs(c.real())-amp(8))-amp(4));
		b[5*stride] = DIST * precision * (abs(abs(c.imag())-amp(8))-amp(4));
		b[6*stride] = DIST * precision * (abs(abs(abs(c.real())-amp(8))-amp(4))-amp(2));
		b[7*stride] = DIST * precision * (abs(abs(abs(c.imag())-amp(8))-amp(4))-amp(2));
	}

	complex_type map(value_type *b, int stride = 1)
	{
		return AMP * complex_type(
			b[0*stride]*(b[2*stride]*(b[4*stride]*(b[6*stride]+value_type(2))+value_type(4))+value_type(8)),
			b[1*stride]*(b[3*stride]*(b[5*stride]*(b[7*stride]+value_type(2))+value_type(4))+value_type(8))
		);
	}
};

template <typename TYPE>
struct QuadratureAmplitudeModulation<1024, TYPE> : public Modulation<TYPE>
{
	static const int NUM = 1024;
	static const int BITS = 10;
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;

	static constexpr value_type FAC = 0.8424235391742344;
	static constexpr value_type RCP = 31 * FAC;
	static constexpr value_type AMP = 1 / RCP;
	static constexpr value_type DIST = 2 * AMP;

	static constexpr value_type amp(int i)
	{
		return AMP * i;
	}

	int bits()
	{
		return BITS;
	}

	void hard(value_type *b, complex_type c, int stride = 1)
	{
		b[0*stride] = c.real() < amp(0) ? value_type(-1) : value_type(1);
		b[1*stride] = c.imag() < amp(0) ? value_type(-1) : value_type(1);
		b[2*stride] = abs(c.real()) < amp(16) ? value_type(-1) : value_type(1);
		b[3*stride] = abs(c.imag()) < amp(16) ? value_type(-1) : value_type(1);
		b[4*stride] = abs(abs(c.real())-amp(16)) < amp(8) ? value_type(-1) : value_type(1);
		b[5*stride] = abs(abs(c.imag())-amp(16)) < amp(8) ? value_type(-1) : value_type(1);
		b[6*stride] = abs(abs(abs(c.real())-amp(16))-amp(8)) < amp(4) ? value_type(-1) : value_type(1);
		b[7*stride] = abs(abs(abs(c.imag())-amp(16))-amp(8)) < amp(4) ? value_type(-1) : value_type(1);
		b[8*stride] = abs(abs(abs(abs(c.real())-amp(16))-amp(8))-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
		b[9*stride] = abs(abs(abs(abs(c.imag())-amp(16))-amp(8))-amp(4)) < amp(2) ? value_type(-1) : value_type(1);
	}

	void soft(value_type *b, complex_type c, value_type precision, int stride = 1)
	{
		b[0*stride] = DIST * precision * c.real();
		b[1*stride] = DIST * precision * c.imag();
		b[2*stride] = DIST * precision * (abs(c.real())-amp(16));
		b[3*stride] = DIST * precision * (abs(c.imag())-amp(16));
		b[4*stride] = DIST * precision * (abs(abs(c.real())-amp(16))-amp(8));
		b[5*stride] = DIST * precision * (abs(abs(c.imag())-amp(16))-amp(8));
		b[6*stride] = DIST * precision * (abs(abs(abs(c.real())-amp(16))-amp(8))-amp(4));
		b[7*stride] = DIST * precision * (abs(abs(abs(c.imag())-amp(16))-amp(8))-amp(4));
		b[8*stride] = DIST * precision * (abs(abs(abs(abs(c.real())-amp(16))-amp(8))-amp(4))-amp(2));
		b[9*stride] = DIST * precision * (abs(abs(abs(abs(c.imag())-amp(16))-amp(8))-amp(4))-amp(2));
	}

	complex_type map(value_type *b, int stride = 1)
	{
		return AMP * complex_type(
			b[0*stride]*(b[2*stride]*(b[4*stride]*(b[6*stride]*(b[8*stride]+value_type(2))+value_type(4))+value_type(8))+value_type(16)),
			b[1*stride]*(b[3*stride]*(b[5*stride]*(b[7*stride]*(b[9*stride]+value_type(2))+value_type(4))+value_type(8))+value_type(16))
		);
	}
};

#endif

