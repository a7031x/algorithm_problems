//https://www.hackerrank.com/challenges/identify-smith-numbers
#include <iostream>
#include <numeric>
#include <number_theory.hpp>

namespace identify_smith_numbers
{
	inline int digit_sum(int n)
	{
		int r = 0;
		while (n)
		{
			r += n % 10;
			n /= 10;
		}
		return r;
	}

	inline bool solve(int n)
	{
		auto factors = algorithm_lib::number_theory_t::prime_factors(n);
		auto r0 = digit_sum(n);
		auto r1 = std::accumulate(factors.begin(), factors.end(), 0, [](int v, int n)
		{
			return v + digit_sum(n);
		});

		return r0 == r1;
	}

	inline void solve()
	{
		int n;
		std::cin >> n;
		std::cout << solve(n) << std::endl;
	}
}