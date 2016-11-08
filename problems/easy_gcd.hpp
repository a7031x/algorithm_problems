//https://www.hackerrank.com/challenges/easy-gcd-1
#include <iostream>
#include <number_theory.hpp>
#include <algorithm>
#include <math.h>

namespace easy_gcd
{
	inline void solve()
	{
		int n;
		int64_t k;
		std::cin >> n >> k;
		int64_t gcd = 0;
		for (int i = 0; i < n; ++i)
		{
			int64_t v;
			std::cin >> v;
			gcd = algorithm_lib::number_theory_t::gcd(v, gcd);
		}
		auto factors = algorithm_lib::number_theory_t::factors_of(gcd);
		int64_t optimal_factor = 1;
		int64_t min_remainder = k + 1;
		for (auto factor : factors)
		{
			if (1 == factor)
				continue;
			auto remainder = k % factor;
			if (remainder < min_remainder)
			{
				min_remainder = remainder;
				optimal_factor = factor;
			}
		}
		int64_t r = k / optimal_factor * optimal_factor;
		std::cout << r << std::endl;
	}
}
