//https://www.hackerrank.com/challenges/cheese-and-random-toppings
//https://en.wikipedia.org/wiki/Lucas%27s_theorem
//https://www.hackerrank.com/external_redirect?to=http://en.wikipedia.org/wiki/Chinese_remainder_theorem#Existence_and_uniqueness
#include <array>
#include <iostream>
#include <binomial_coefficient.hpp>

namespace cheese_and_random_toppings
{
	inline std::vector<int> calc_factors(int m)
	{
		std::array<int, 50> max_factor;
		std::vector<int> prime;
		for (size_t k = 0; k < max_factor.size(); ++k)
			max_factor[k] = k;
		for (size_t i = 2; i < max_factor.size(); ++i)
		{
			if (max_factor[i] != i)
				continue;
			for (size_t j = i * 2; j < max_factor.size(); j += i)
				max_factor[j] = i;
			prime.push_back(i);
		}

		std::vector<int> r;
		for (auto p : prime)
		{
			if (m % p == 0)
				r.push_back(p);
		}
		return r;
	}

	inline std::vector<int> calc_coefficient(int n, int p)
	{
		std::vector<int> r;
		while (n)
		{
			r.push_back(n % p);
			n /= p;
		}
		return r;
	}

	inline int calc_reminder(const algorithm_lib::binomial_coefficient_t& bct, int n, int r, int p)
	{
		auto ns = calc_coefficient(n, p);
		auto rs = calc_coefficient(r, p);
		while (rs.size() < ns.size())
			rs.push_back(0);

		int64_t res = 1;
		for (size_t k = 0; k < ns.size(); ++k)
			res = res * bct.cnr(ns[k], rs[k]) % p;

		return (int)res;
	}

	inline int solve(int n, int r, int m)
	{
		auto factors = calc_factors(m);
		algorithm_lib::binomial_coefficient_t bct(50, m);
		int res = 0;
		int base = 1;
		for (auto p : factors)
		{
			auto remainder = calc_reminder(bct, n, r, p);
			while (res % p != remainder)
				res += base;
			base *= p;
		}
		return res;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int n, r, m;
			std::cin >> n >> r >> m;
			std::cout << solve(n, r, m) << std::endl;
		}
	}
}