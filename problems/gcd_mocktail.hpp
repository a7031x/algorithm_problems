#include <iostream>
#include <binomial_coefficient.hpp>
#include <number_theory.hpp>

namespace gcd_mocktail
{
	const int moduli = 30000001;
	std::vector<int> power_l_coef(const algorithm_lib::binomial_coefficient_t& cnr, int l)
	{
		std::vector<int> r(l + 1);
		r[l] = algorithm_lib::number_theory_t::modular_multiplicative_inverse((int)cnr.cnr(l + 1, 1), moduli);

		for (int c = 2; c <= l + 1; ++c)
		{
			int64_t s = 0;
			for (int d = c; d > 1; --d)
			{
				auto delta = algorithm_lib::number_theory_t::mod((int64_t)r[l + d - c] * cnr.cnr(l + 1 + d - c, d), moduli);
				auto sign = (d & 1) ? -1 : 1;
				s = algorithm_lib::number_theory_t::mod(s + sign * delta, moduli);
			}
			r[l - c + 1] = algorithm_lib::number_theory_t::modular_multiplicative_inverse((int)cnr.cnr(l + 2 - c, 1), (int)s, moduli);
		}

		return r;
	}

	int sum(int n, const std::vector<int>& coef)
	{
		int64_t r = 0;
		int l = (int)coef.size() - 1;
		int64_t x = 1;

		for (auto cf : coef)
		{
			x = algorithm_lib::number_theory_t::mod(x * n, moduli);
			r += cf * x;
		}
		r = algorithm_lib::number_theory_t::mod(r, moduli);

		return (int)r;
	}

	int calc_f(int n, int d, std::unordered_map<int, int>& cache)
	{
		if (cache.count(n))
			return cache[n];

		if (1 == n)
		{
			cache[1] = 1;
			return 1;
		}

		int sqrt_n = (int)sqrt(n);
		int xd = algorithm_lib::number_theory_t::power_mod(n, d, moduli);
		int s0 = 0;
		for (int g = 2; g <= sqrt_n; ++g)
		{
			s0 = algorithm_lib::number_theory_t::mod(s0 + calc_f(n / g, d, cache), moduli);
		}
		int s1 = 0;
		for (int k = 1; k <= n / (sqrt_n + 1); ++k)
		{
			int64_t delta = calc_f(k, d, cache);
			auto range = n / k - n / (k + 1);
			delta = algorithm_lib::number_theory_t::mod(delta * range, moduli);
			s1 = (int)algorithm_lib::number_theory_t::mod(s1 + delta, moduli);
		}
		auto r = algorithm_lib::number_theory_t::mod(xd - s0 - s1, moduli);
		cache[n] = r;
		return r;
	}

	int calc_h(int n, int d, int l,
		const algorithm_lib::binomial_coefficient_t& cnr,
		std::unordered_map<int, int>& f_cache)
	{
		int sqrt_n = (int)sqrt(n);
		auto coef = power_l_coef(cnr, l);

		int64_t h0 = 0;
		for (int g = 1; g <= sqrt_n; ++g)
		{
			auto f = calc_f(n / g, d, f_cache);
			int64_t gl = algorithm_lib::number_theory_t::power_mod(g, l, moduli);
			h0 = algorithm_lib::number_theory_t::mod(h0 + f * gl, moduli);
		}
		int64_t h1 = 0;
		auto next_gl = sum(n, coef);

		for (int k = 1; k <= n / (sqrt_n + 1); ++k)
		{
			auto f = calc_f(k, d, f_cache);
			auto gl = next_gl;
			next_gl = sum(n / (k + 1), coef);
			int64_t range = algorithm_lib::number_theory_t::mod(gl - next_gl, moduli);
			h1 = algorithm_lib::number_theory_t::mod(h1 + f * range, moduli);
		}
		auto r = algorithm_lib::number_theory_t::mod(h0 + h1, moduli);
		return (int)r;
	}

	void solve(int n, int d, const std::vector<int>& ls,
		const algorithm_lib::binomial_coefficient_t& cnr)
	{
		std::unordered_map<int, int> f_cache;
		for (auto l : ls)
		{
			std::cout << calc_h(n, d, l, cnr, f_cache) << std::endl;
		}
	}

	void solve()
	{
		algorithm_lib::binomial_coefficient_t cnr(101, moduli);
		int t;
		std::cin >> t;
		while (t--)
		{
			int n, d, q;
			std::cin >> n >> d >> q;
			std::vector<int> ls;
			while (q--)
			{
				int l;
				std::cin >> l;
				ls.push_back(l);
			}
			solve(n, d, ls, cnr);
		}
	}
}