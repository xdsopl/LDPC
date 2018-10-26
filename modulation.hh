/*
Modulation interface

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef MODULATION_HH
#define MODULATION_HH

template <typename TYPE>
struct Modulation
{
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;

	virtual int bits() = 0;
	virtual void hard(value_type *, complex_type, int = 1) = 0;
	virtual void soft(value_type *, complex_type, value_type, int = 1) = 0;
	virtual complex_type map(value_type *, int = 1) = 0;
	virtual ~Modulation() = default;
};

#endif

