/*
SIMD-ified LDPC algorithms

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef ALGORITHMS_HH
#define ALGORITHMS_HH

#include "exclusive_reduce.hh"

template <typename TYPE>
struct NormalUpdate
{
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <typename TYPE>
struct SelfCorrectedUpdate
{
	static TYPE update(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(vand(vmask(b), vorr(vceqz(a), veor(vcgtz(a), vcltz(b)))));
	}
};

template <typename TYPE, typename UPDATE>
struct MinSumAlgorithm
{
	static TYPE one()
	{
		return vdup<TYPE>(1);
	}
	static TYPE min(TYPE a, TYPE b)
	{
		return vmin(a, b);
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return vsign(a, b);
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = vabs(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, min);

		TYPE signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, sign);

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(mins[i], signs[i]);
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return vadd(a, b);
	}
	static bool bad(TYPE v, int blocks)
	{
		auto tmp = vcgtz(v);
		for (int i = 0; i < blocks; ++i)
			if (!tmp.v[i])
				return true;
		return false;
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

template <int WIDTH, typename UPDATE>
struct MinSumAlgorithm<SIMD<int8_t, WIDTH>, UPDATE>
{
	typedef SIMD<int8_t, WIDTH> TYPE;
	static TYPE one()
	{
		return vdup<TYPE>(1);
	}
	static TYPE min(TYPE a, TYPE b)
	{
		return vmin(a, b);
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return vsign(a, b);
	}
	static TYPE eor(TYPE a, TYPE b)
	{
		return vreinterpret<TYPE>(veor(vmask(a), vmask(b)));
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = vqabs(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, min);

		TYPE signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, eor);
		for (int i = 0; i < cnt; ++i)
			signs[i] = vreinterpret<TYPE>(vorr(vmask(signs[i]), vmask(vdup<TYPE>(127))));

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(mins[i], signs[i]);
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return vqadd(a, b);
	}
	static bool bad(TYPE v, int blocks)
	{
		auto tmp = vcgtz(v);
		for (int i = 0; i < blocks; ++i)
			if (!tmp.v[i])
				return true;
		return false;
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

#endif
