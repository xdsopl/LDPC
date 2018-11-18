/*
Generic LDPC algorithms

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef GENERIC_HH
#define GENERIC_HH

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
		return (a == TYPE(0) || (a < TYPE(0)) == (b < TYPE(0))) ? b : TYPE(0);
	}
};

template <>
struct SelfCorrectedUpdate<int8_t>
{
	static int8_t update(int8_t a, int8_t b)
	{
		return (a == 0 || ((a ^ b) & 128) == 0) ? b : 0;
	}
};

template <typename TYPE, typename UPDATE>
struct MinSumAlgorithm
{
	static TYPE one()
	{
		return 1;
	}
	static TYPE min(TYPE a, TYPE b)
	{
		return std::min(a, b);
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = std::abs(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, min);

		TYPE signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, sign);

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(mins[i], signs[i]);
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static bool bad(TYPE v, int)
	{
		return v <= TYPE(0);
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename UPDATE>
struct MinSumAlgorithm<float, UPDATE>
{
	static float one()
	{
		return 1.f;
	}
	static float min(float a, float b)
	{
		return std::min(a, b);
	}
	static int xor_(int a, int b)
	{
		return a ^ b;
	}
	static void finalp(float *links, int cnt)
	{
		int mask = 0x80000000;
		float mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = std::abs(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, min);

		int signs[cnt];
		CODE::exclusive_reduce(reinterpret_cast<int *>(links), signs, cnt, xor_);
		for (int i = 0; i < cnt; ++i)
			signs[i] &= mask;

		for (int i = 0; i < cnt; ++i)
			reinterpret_cast<int *>(links)[i] = signs[i] | reinterpret_cast<int *>(mins)[i];
	}
	static float sign(float a, float b)
	{
		return b < 0.f ? -a : b > 0.f ? a : 0.f;
	}
	static float add(float a, float b)
	{
		return a + b;
	}
	static bool bad(float v, int)
	{
		return v <= 0.f;
	}
	static float update(float a, float b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename UPDATE>
struct MinSumAlgorithm<int8_t, UPDATE>
{
	static int8_t one()
	{
		return 1;
	}
	static int8_t add(int8_t a, int8_t b)
	{
		int x = int(a) + int(b);
		x = std::min<int>(std::max<int>(x, -128), 127);
		return x;
	}
	static int8_t min(int8_t a, int8_t b)
	{
		return std::min(a, b);
	}
	static int8_t xor_(int8_t a, int8_t b)
	{
		return a ^ b;
	}
	static int8_t sqabs(int8_t a)
	{
		return std::abs(std::max<int8_t>(a, -127));
	}
	static int8_t sign(int8_t a, int8_t b)
	{
		return b < 0 ? -a : b > 0 ? a : 0;
	}
	static void finalp(int8_t *links, int cnt)
	{
		int8_t mags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = sqabs(links[i]);
		CODE::exclusive_reduce(mags, mins, cnt, min);

		int8_t signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, xor_);
		for (int i = 0; i < cnt; ++i)
			signs[i] |= 127;

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(mins[i], signs[i]);
	}
	static bool bad(int8_t v, int)
	{
		return v <= 0;
	}
	static int8_t update(int8_t a, int8_t b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename TYPE, typename UPDATE, int FACTOR>
struct MinSumCAlgorithm
{
	static TYPE one()
	{
		return 1;
	}
	static TYPE correction_factor(TYPE a, TYPE b)
	{
		if (1) {
			TYPE c = TYPE(FACTOR) / TYPE(2);
			TYPE apb = std::abs(a + b);
			TYPE amb = std::abs(a - b);
			if (apb < TYPE(2) && amb > TYPE(2) * apb)
				return c;
			if (amb < TYPE(2) && apb > TYPE(2) * amb)
				return -c;
			return 0;
		}
		return std::log(TYPE(1)+std::exp(-std::abs(a+b))) - std::log(TYPE(1)+std::exp(-std::abs(a-b)));
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
	static TYPE min(TYPE a, TYPE b)
	{
		TYPE m = std::min(std::abs(a), std::abs(b));
		TYPE x = sign(sign(m, a), b);
		x += correction_factor(a, b);
		return x;
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE tmp[cnt];
		CODE::exclusive_reduce(links, tmp, cnt, min);
		for (int i = 0; i < cnt; ++i)
			links[i] = tmp[i];
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static bool bad(TYPE v, int)
	{
		return v <= TYPE(0);
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename UPDATE, int FACTOR>
struct MinSumCAlgorithm<float, UPDATE, FACTOR>
{
	static float one()
	{
		return 1.f;
	}
	static float correction_factor(float a, float b)
	{
		float c = 0.5f;
		float apb = std::abs(a + b);
		float amb = std::abs(a - b);
		if (apb < 2.f && amb > 2.f * apb)
			return c;
		if (amb < 2.f && apb > 2.f * amb)
			return -c;
		return 0;
	}
	static float min(float a, float b)
	{
		int mask = 0x80000000;
		float m = std::min(std::abs(a), std::abs(b));
		int tmp = (mask & (*reinterpret_cast<int *>(&a) ^ *reinterpret_cast<int *>(&b))) | *reinterpret_cast<int *>(&m);
		float x = *reinterpret_cast<float *>(&tmp);
		x += correction_factor(a, b);
		return x;
	}
	static void finalp(float *links, int cnt)
	{
		float tmp[cnt];
		CODE::exclusive_reduce(links, tmp, cnt, min);
		for (int i = 0; i < cnt; ++i)
			links[i] = tmp[i];
	}
	static float sign(float a, float b)
	{
		return b < 0.f ? -a : b > 0.f ? a : 0.f;
	}
	static float add(float a, float b)
	{
		return a + b;
	}
	static bool bad(float v, int)
	{
		return v <= 0.f;
	}
	static float update(float a, float b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename UPDATE, int FACTOR>
struct MinSumCAlgorithm<int8_t, UPDATE, FACTOR>
{
	static int8_t one()
	{
		return 1;
	}
	static int8_t add(int8_t a, int8_t b)
	{
		int x = int(a) + int(b);
		x = std::min<int>(std::max<int>(x, -128), 127);
		return x;
	}
	static uint8_t addu(uint8_t a, uint8_t b)
	{
		int x = int(a) + int(b);
		x = std::min<int>(x, 255);
		return x;
	}
	static int8_t sub(int8_t a, int8_t b)
	{
		int x = int(a) - int(b);
		x = std::min<int>(std::max<int>(x, -128), 127);
		return x;
	}
	static uint8_t subu(uint8_t a, uint8_t b)
	{
		int x = int(a) - int(b);
		x = std::max<int>(x, 0);
		return x;
	}
	static uint8_t abs(int8_t a)
	{
		return std::abs<int>(a);
	}
	static int8_t sqabs(int8_t a)
	{
		return std::abs(std::max<int8_t>(a, -127));
	}
	static int8_t sign(int8_t a, int8_t b)
	{
		return b < 0 ? -a : b > 0 ? a : 0;
	}
	static int8_t correction_factor(int8_t a, int8_t b)
	{
		uint8_t factor2 = FACTOR * 2;
		uint8_t c = FACTOR / 2;
		uint8_t apb = abs(add(a, b));
		uint8_t apb2 = addu(apb, apb);
		uint8_t amb = abs(sub(a, b));
		uint8_t amb2 = addu(amb, amb);
		if (subu(factor2, apb) && subu(amb, apb2))
			return c;
		if (subu(factor2, amb) && subu(apb, amb2))
			return -c;
		return 0;
	}
	static int8_t min(int8_t a, int8_t b)
	{
		int8_t m = std::min(sqabs(a), sqabs(b));
		int8_t x = sign(sign(m, a), b);
		x = add(x, correction_factor(a, b));
		return x;
	}
	static void finalp(int8_t *links, int cnt)
	{
		int8_t tmp[cnt];
		CODE::exclusive_reduce(links, tmp, cnt, min);
		for (int i = 0; i < cnt; ++i)
			links[i] = tmp[i];
	}
	static bool bad(int8_t v, int)
	{
		return v <= 0;
	}
	static int8_t update(int8_t a, int8_t b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename TYPE, typename UPDATE>
struct LogDomainSPA
{
	static TYPE one()
	{
		return 1;
	}
	static TYPE phi(TYPE x)
	{
		x = std::min(std::max(x, TYPE(0.000001)), TYPE(14.5));
		return std::log(std::exp(x)+TYPE(1)) - std::log(std::exp(x)-TYPE(1));
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE mags[cnt], sums[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = phi(std::abs(links[i]));
		CODE::exclusive_reduce(mags, sums, cnt, add);

		TYPE signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, sign);

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(phi(sums[i]), signs[i]);
	}
	static bool bad(TYPE v, int)
	{
		return v <= TYPE(0);
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename TYPE, typename UPDATE, int LAMBDA>
struct LambdaMinAlgorithm
{
	static TYPE one()
	{
		return 1;
	}
	static TYPE phi(TYPE x)
	{
		x = std::min(std::max(x, TYPE(0.000001)), TYPE(14.5));
		return std::log(std::exp(x)+TYPE(1)) - std::log(std::exp(x)-TYPE(1));
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
	static void finalp(TYPE *links, int cnt)
	{
		typedef std::pair<TYPE, int> Pair;
		Pair mags[cnt];
		for (int i = 0; i < cnt; ++i)
			mags[i] = Pair(std::abs(links[i]), i);
		std::nth_element(mags, mags+LAMBDA, mags+cnt, [](Pair a, Pair b){ return a.first < b.first; });

		TYPE sums[cnt];
		for (int i = 0; i < cnt; ++i) {
			int j = 0;
			if (i == mags[0].second)
				++j;
			sums[i] = phi(mags[j].first);
			for (int l = 1; l < LAMBDA; ++l) {
				++j;
				if (i == mags[j].second)
					++j;
				sums[i] += phi(mags[j].first);
			}
		}

		TYPE signs[cnt];
		CODE::exclusive_reduce(links, signs, cnt, sign);

		for (int i = 0; i < cnt; ++i)
			links[i] = sign(phi(sums[i]), signs[i]);
	}
	static bool bad(TYPE v, int)
	{
		return v <= TYPE(0);
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

template <typename TYPE, typename UPDATE>
struct SumProductAlgorithm
{
	static TYPE one()
	{
		return 1;
	}
	static TYPE prep(TYPE x)
	{
		return std::tanh(TYPE(0.5) * x);
	}
	static TYPE postp(TYPE x)
	{
		return TYPE(2) * std::atanh(x);
	}
	static TYPE mul(TYPE a, TYPE b)
	{
		return a * b;
	}
	static TYPE sign(TYPE a, TYPE b)
	{
		return b < TYPE(0) ? -a : b > TYPE(0) ? a : TYPE(0);
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE in[cnt], out[cnt];
		for (int i = 0; i < cnt; ++i)
			in[i] = prep(links[i]);
		CODE::exclusive_reduce(in, out, cnt, mul);
		for (int i = 0; i < cnt; ++i)
			links[i] = postp(out[i]);
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static bool bad(TYPE v, int)
	{
		return v <= TYPE(0);
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return UPDATE::update(a, b);
	}
};

#endif