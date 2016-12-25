//https://www.hackerrank.com/challenges/akhil-and-gf?h_r=next-challenge&h_v=zen
#include <unordered_map>
#include <iostream>
#include <number_theory.hpp>

namespace akhil_and_girlfriend
{
	inline int64_t power_mod(int64_t n, int m, std::unordered_map<int64_t, int64_t>& cache)
	{
		auto it = cache.find(n);
		if (cache.end() != it)
			return it->second;
		return cache[n] = algorithm_lib::number_theory_t::power_mod<int64_t>(10, n, m);
	}

	inline int64_t solve(int64_t n, int m, int c,
		std::unordered_map<int64_t, int64_t>& result_cache,
		std::unordered_map<int64_t, int64_t>& power_cache)
	{
		if (1 == n)
			return c % m;
		auto it = result_cache.find(n);
		if (result_cache.end() != it)
			return it->second;

		return result_cache[n] = algorithm_lib::number_theory_t::mod(
			solve(n - n / 2, m, c, result_cache, power_cache) * power_mod(n / 2, m, power_cache) +
			solve(n / 2, m, c, result_cache, power_cache), m);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t n;
			int m;
			std::cin >> n >> m;
			std::unordered_map<int64_t, int64_t> result_cache, power_cache;
			std::cout << solve(n, m, 1, result_cache, power_cache) << std::endl;
		}
	}
}