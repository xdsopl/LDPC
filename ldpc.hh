/*
LDPC SISO encoder and decoder

Copyright 2018 Ahmet Inan <xdsopl@gmail.com>
*/

#ifndef LDPC_HH
#define LDPC_HH

#include "exclusive_reduce.hh"

template <typename TYPE>
struct MinSumAlgorithm
{
	static TYPE min(TYPE a, TYPE b)
	{
		return std::min(a, b);
	}
	static TYPE mul(TYPE a, TYPE b)
	{
		return a * b;
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE blmags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			blmags[i] = std::abs(links[i]);
		CODE::exclusive_reduce(blmags, mins, cnt, min);

		TYPE blsigns[cnt], signs[cnt];
		for (int i = 0; i < cnt; ++i)
			blsigns[i] = links[i] < TYPE(0) ? TYPE(-1) : TYPE(1);
		CODE::exclusive_reduce(blsigns, signs, cnt, mul);

		for (int i = 0; i < cnt; ++i)
			links[i] = signs[i] * mins[i];
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <typename TYPE>
struct SelfCorrectedMinSumAlgorithm
{
	static TYPE min(TYPE a, TYPE b)
	{
		return std::min(a, b);
	}
	static TYPE mul(TYPE a, TYPE b)
	{
		return a * b;
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE blmags[cnt], mins[cnt];
		for (int i = 0; i < cnt; ++i)
			blmags[i] = std::abs(links[i]);
		CODE::exclusive_reduce(blmags, mins, cnt, min);

		TYPE blsigns[cnt], signs[cnt];
		for (int i = 0; i < cnt; ++i)
			blsigns[i] = links[i] < TYPE(0) ? TYPE(-1) : TYPE(1);
		CODE::exclusive_reduce(blsigns, signs, cnt, mul);

		for (int i = 0; i < cnt; ++i)
			links[i] = signs[i] * mins[i];
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static TYPE update(TYPE a, TYPE b)
	{
		return (a == TYPE(0) || (a < TYPE(0)) == (b < TYPE(0))) ? b : TYPE(0);
	}
};

template <typename TYPE>
struct MinSumCAlgorithm
{
	static TYPE correction_factor(TYPE a, TYPE b)
	{
		if (1) {
			TYPE c = 0.5;
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
	static TYPE min(TYPE a, TYPE b)
	{
		TYPE m = std::min(std::abs(a), std::abs(b));
		TYPE x = (a < TYPE(0)) != (b < TYPE(0)) ? -m : m;
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
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <>
struct MinSumCAlgorithm<int8_t>
{
	static int8_t add(int8_t a, int8_t b)
	{
		int x = int(a) + int(b);
		x = std::min<int>(std::max<int>(x, -128), 127);
		return x;
	}
	static int8_t sub(int8_t a, int8_t b)
	{
		int x = int(a) - int(b);
		x = std::min<int>(std::max<int>(x, -128), 127);
		return x;
	}
	static int8_t correction_factor(int8_t a, int8_t b)
	{
		int factor = 2;
		int c = factor / 2;
		int apb = std::abs<int>(add(a, b));
		int amb = std::abs<int>(sub(a, b));
		if (apb / 2 < factor && amb / 2 > apb)
			return c;
		if (amb / 2 < factor && apb / 2 > amb)
			return -c;
		return 0;
	}
	static int8_t min(int8_t a, int8_t b)
	{
		int m = std::min(std::abs<int>(a), std::abs<int>(b));
		int x = (a ^ b) < 0 ? -m : m;
		x = std::min<int>(std::max<int>(x, -128), 127);
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
	static int8_t update(int8_t, int8_t v)
	{
		return v;
	}
};

template <typename TYPE>
struct LogDomainSPA
{
	static TYPE phi(TYPE x)
	{
		x = std::min(std::max(x, TYPE(0.000001)), TYPE(14.5));
		return std::log(std::exp(x)+TYPE(1)) - std::log(std::exp(x)-TYPE(1));
	}
	static TYPE mul(TYPE a, TYPE b)
	{
		return a * b;
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static void finalp(TYPE *links, int cnt)
	{
		TYPE blmags[cnt], sums[cnt];
		for (int i = 0; i < cnt; ++i)
			blmags[i] = phi(std::abs(links[i]));
		CODE::exclusive_reduce(blmags, sums, cnt, add);

		TYPE blsigns[cnt], signs[cnt];
		for (int i = 0; i < cnt; ++i)
			blsigns[i] = links[i] < TYPE(0) ? TYPE(-1) : TYPE(1);
		CODE::exclusive_reduce(blsigns, signs, cnt, mul);

		for (int i = 0; i < cnt; ++i)
			links[i] = signs[i] * phi(sums[i]);
	}
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <typename TYPE, int LAMBDA>
struct LambdaMinAlgorithm
{
	static TYPE phi(TYPE x)
	{
		x = std::min(std::max(x, TYPE(0.000001)), TYPE(14.5));
		return std::log(std::exp(x)+TYPE(1)) - std::log(std::exp(x)-TYPE(1));
	}
	static TYPE mul(TYPE a, TYPE b)
	{
		return a * b;
	}
	static TYPE add(TYPE a, TYPE b)
	{
		return a + b;
	}
	static void finalp(TYPE *links, int cnt)
	{
		typedef std::pair<TYPE, int> Pair;
		Pair blmags[cnt];
		for (int i = 0; i < cnt; ++i)
			blmags[i] = Pair(std::abs(links[i]), i);
		std::nth_element(blmags, blmags+LAMBDA, blmags+cnt, [](Pair a, Pair b){ return a.first < b.first; });

		TYPE sums[cnt];
		for (int i = 0; i < cnt; ++i) {
			int j = 0;
			if (i == blmags[0].second)
				++j;
			sums[i] = phi(blmags[j].first);
			for (int l = 1; l < LAMBDA; ++l) {
				++j;
				if (i == blmags[j].second)
					++j;
				sums[i] += phi(blmags[j].first);
			}
		}

		TYPE blsigns[cnt], signs[cnt];
		for (int i = 0; i < cnt; ++i)
			blsigns[i] = links[i] < TYPE(0) ? TYPE(-1) : TYPE(1);
		CODE::exclusive_reduce(blsigns, signs, cnt, mul);

		for (int i = 0; i < cnt; ++i)
			links[i] = signs[i] * phi(sums[i]);
	}
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <typename TYPE>
struct SumProductAlgorithm
{
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
	static TYPE update(TYPE, TYPE v)
	{
		return v;
	}
};

template <typename TYPE>
struct LDPCInterface
{
	virtual int code_len() = 0;
	virtual int data_len() = 0;
	virtual void encode(TYPE *, TYPE *) = 0;
	virtual int decode(TYPE *, TYPE *, int) = 0;
	virtual void examine() = 0;
	virtual ~LDPCInterface() = default;
};

template <typename TABLE, typename TYPE>
class LDPC : public LDPCInterface<TYPE>
{
	static const int M = TABLE::M;
	static const int N = TABLE::N;
	static const int K = TABLE::K;
	static const int R = N-K;
	static const int q = R/M;
	static const int CNL = TABLE::LINKS_MAX_CN;
	int acc_pos[TABLE::DEG_MAX];
	const int *row_ptr;
	int bit_deg;
	int grp_cnt;
	int grp_num;
	int grp_len;
	TYPE bnl[TABLE::LINKS_TOTAL];
	TYPE bnv[N];
	TYPE cnl[R * CNL];
	int8_t cnv[R];
	uint8_t cnc[R];
	//MinSumAlgorithm<TYPE> alg;
	SelfCorrectedMinSumAlgorithm<TYPE> alg;
	//MinSumCAlgorithm<TYPE> alg;
	//LogDomainSPA<TYPE> alg;
	//LambdaMinAlgorithm<TYPE, 3> alg;
	//SumProductAlgorithm<TYPE> alg;

	int signum(TYPE v)
	{
		return (v > TYPE(0)) - (v < TYPE(0));
	}
	void next_group()
	{
		if (grp_cnt >= grp_len) {
			grp_len = TABLE::LEN[grp_num];
			bit_deg = TABLE::DEG[grp_num];
			grp_cnt = 0;
			++grp_num;
		}
		for (int i = 0; i < bit_deg; ++i)
			acc_pos[i] = row_ptr[i];
		row_ptr += bit_deg;
		++grp_cnt;
	}
	void next_bit()
	{
		for (int i = 0; i < bit_deg; ++i)
			acc_pos[i] += q;
		for (int i = 0; i < bit_deg; ++i)
			acc_pos[i] %= R;
	}
	void first_group()
	{
		grp_num = 0;
		grp_len = 0;
		grp_cnt = 0;
		row_ptr = TABLE::POS;
		next_group();
	}
	void bit_node_init(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			bnv[i] = parity[i];
		for (int i = 0; i < K; ++i)
			bnv[i+R] = data[i];

		TYPE *bl = bnl;
		for (int i = 0; i < R-1; ++i) {
			*bl++ = parity[i];
			*bl++ = parity[i];
		}
		*bl++ = parity[R-1];
		first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < bit_deg; ++n)
					*bl++ = data[j+m];
				next_bit();
			}
			next_group();
		}
	}
	void check_node_update()
	{
		cnv[0] = signum(bnv[0]);
		for (int i = 1; i < R; ++i)
			cnv[i] = signum(bnv[i-1]) * signum(bnv[i]);

		TYPE *bl = bnl;
		cnl[0] = *bl++;
		cnc[0] = 1;
		for (int i = 1; i < R; ++i) {
			cnl[CNL*i] = *bl++;
			cnl[CNL*i+1] = *bl++;
			cnc[i] = 2;
		}
		first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < bit_deg; ++n) {
					int i = acc_pos[n];
					cnv[i] *= signum(bnv[j+m+R]);
					cnl[CNL*i+cnc[i]++] = *bl++;
				}
				next_bit();
			}
			next_group();
		}
		for (int i = 0; i < R; ++i)
			alg.finalp(cnl+CNL*i, cnc[i]);
		if (0) {
			TYPE min = 0, max = 0;
			for (int i = 0; i < R * CNL; ++i) {
				min = std::min(min, cnl[i]);
				max = std::max(max, cnl[i]);
			}
			std::cerr << "cnl: min = " << min << " max = " << max << std::endl;
		}
	}
	void bit_node_update(TYPE *data, TYPE *parity)
	{
		bnv[0] = alg.add(parity[0], alg.add(cnl[0], cnl[CNL]));
		for (int i = 1; i < R-1; ++i)
			bnv[i] = alg.add(parity[i], alg.add(cnl[CNL*i+1], cnl[CNL*(i+1)]));
		bnv[R-1] = alg.add(parity[R-1], cnl[CNL*(R-1)+1]);

		TYPE *bl = bnl;
		cnc[0] = 1;
		for (int i = 1; i < R; ++i)
			cnc[i] = 2;
		*bl = alg.update(*bl, alg.add(parity[0], cnl[CNL])); ++bl;
		*bl = alg.update(*bl, alg.add(parity[0], cnl[0])); ++bl;
		for (int i = 1; i < R-1; ++i) {
			*bl = alg.update(*bl, alg.add(parity[i], cnl[CNL*(i+1)])); ++bl;
			*bl = alg.update(*bl, alg.add(parity[i], cnl[CNL*i+1])); ++bl;
		}
		*bl = alg.update(*bl, parity[R-1]); ++bl;
		first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				TYPE inp[bit_deg];
				for (int n = 0; n < bit_deg; ++n) {
					int i = acc_pos[n];
					inp[n] = cnl[CNL*i+cnc[i]++];
				}
				TYPE out[bit_deg];
				CODE::exclusive_reduce(inp, out, bit_deg, alg.add);
				bnv[j+m+R] = alg.add(data[j+m], alg.add(out[0], inp[0]));
				for (int n = 0; n < bit_deg; ++n, ++bl)
					*bl = alg.update(*bl, alg.add(data[j+m], out[n]));
				next_bit();
			}
			next_group();
		}
		if (0) {
			TYPE min = 0, max = 0;
			for (int i = 0; i < TABLE::LINKS_TOTAL; ++i) {
				min = std::min(min, bnv[i]);
				max = std::max(max, bnv[i]);
			}
			std::cerr << "bnl: min = " << min << " max = " << max << std::endl;
		}
		if (0) {
			TYPE min = 0, max = 0;
			for (int i = 0; i < N; ++i) {
				min = std::min(min, bnv[i]);
				max = std::max(max, bnv[i]);
			}
			std::cerr << "bnv: min = " << min << " max = " << max << std::endl;
		}
		if (0) {
			static int count;
			std::cout << count++;
			for (int i = 0; i < N; ++i)
				std::cout << " " << std::min<TYPE>(std::max<TYPE>(bnv[i], -1000000), 1000000);
			std::cout << std::endl;
		}
	}
	bool hard_decision()
	{
		for (int i = 0; i < R; ++i)
			if (cnv[i] <= 0)
				return true;
		return false;
	}
	void update_user(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			parity[i] = bnv[i];
		for (int i = 0; i < K; ++i)
			data[i] = bnv[i+R];
	}
public:
	int code_len()
	{
		return N;
	}
	int data_len()
	{
		return K;
	}
	int decode(TYPE *data, TYPE *parity, int trials = 50)
	{
		bit_node_init(data, parity);
		check_node_update();
		if (!hard_decision())
			return trials;
		--trials;
		bit_node_update(data, parity);
		check_node_update();
		while (hard_decision() && --trials >= 0) {
			bit_node_update(data, parity);
			check_node_update();
		}
		update_user(data, parity);
		return trials;
	}
	void encode(TYPE *data, TYPE *parity)
	{
		for (int i = 0; i < R; ++i)
			parity[i] = 1;
		first_group();
		for (int j = 0; j < K; j += M) {
			for (int m = 0; m < M; ++m) {
				for (int n = 0; n < bit_deg; ++n) {
					int i = acc_pos[n];
					parity[i] *= data[j+m];
				}
				next_bit();
			}
			next_group();
		}
		for (int i = 1; i < R; ++i)
			parity[i] *= parity[i-1];
	}
	void examine()
	{
		int bit_node_link_count[N];
		for (int i = 0; i < N; ++i)
			bit_node_link_count[i] = 0;
		int check_node_link_count[R];
		for (int i = 0; i < R; ++i)
			check_node_link_count[i] = 0;
		first_group();
		for (int j = 0; j < K; j += M) {
			if (0) {
				for (int n = 0; n < bit_deg; ++n)
					std::cerr << " " << acc_pos[n];
				std::cerr << std::endl;
			}
			for (int m = 0; m < M; ++m) {
				bit_node_link_count[j+m] += bit_deg;
				for (int n = 0; n < bit_deg; ++n) {
					int i = acc_pos[n];
					++check_node_link_count[i];
				}
				for (int n = 0; n < bit_deg; ++n)
					for (int k = 0; k < bit_deg; ++k)
						assert(n == k || acc_pos[n] != acc_pos[k]);
				next_bit();
			}
			next_group();
		}
		for (int i = K; i < N-1; ++i)
			bit_node_link_count[i] = 2;
		bit_node_link_count[N-1] = 1;
		int bit_node_max_links = 0;
		for (int i = 0; i < N; ++i)
			bit_node_max_links = std::max(bit_node_max_links, bit_node_link_count[i]);
		int bit_node_hist[bit_node_max_links+1];
		for (int i = 0; i <= bit_node_max_links; ++i)
			bit_node_hist[i] = 0;
		for (int i = 0; i < N; ++i)
			++bit_node_hist[bit_node_link_count[i]];
		int bit_node_link_count_total = 0;
		for (int i = 0; i <= bit_node_max_links; ++i)
			bit_node_link_count_total += i * bit_node_hist[i];
		assert(bit_node_link_count_total <= TABLE::LINKS_TOTAL);
		std::cerr << "total number of bit node links: " << bit_node_link_count_total << " with: (number of bit nodes):(having number of links)";
		for (int i = 0; i <= bit_node_max_links; ++i)
			if (bit_node_hist[i])
				std::cerr << " " << bit_node_hist[i] << ":" << i;
		std::cerr << std::endl;

		++check_node_link_count[0];
		for (int i = 1; i < R; ++i)
			check_node_link_count[i] += 2;
		int check_node_max_links = 0;
		for (int i = 0; i < R; ++i)
			check_node_max_links = std::max(check_node_max_links, check_node_link_count[i]);
		assert(check_node_max_links <= CNL);
		int check_node_hist[check_node_max_links+1];
		for (int i = 0; i <= check_node_max_links; ++i)
			check_node_hist[i] = 0;
		for (int i = 0; i < R; ++i)
			++check_node_hist[check_node_link_count[i]];
		int check_node_link_count_total = 0;
		for (int i = 0; i <= check_node_max_links; ++i)
			check_node_link_count_total += i * check_node_hist[i];
		assert(check_node_link_count_total <= CNL * R);
		std::cerr << "total number of check node links: " << check_node_link_count_total << " with: (number of check nodes):(having number of links)";
		for (int i = 0; i <= check_node_max_links; ++i)
			if (check_node_hist[i])
				std::cerr << " " << check_node_hist[i] << ":" << i;
		std::cerr << std::endl;
	}
};

#endif
