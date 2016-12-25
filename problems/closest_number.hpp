//https://www.hackerrank.com/challenges/closest-number
#include <number_theory.hpp>
#include <limits>
#include <iostream>

namespace closest_number
{
	inline int64_t closer_number(int64_t a, int64_t b, int64_t x)
	{
		return abs(a - x) <= abs(b - x) ? a : b;
	}

	inline int64_t solve(int64_t a, int64_t b, int64_t x)
	{
		if (1 == a || 0 == b)
		{
			return closer_number(0, x, 1);
		}
		if (b < 0)
		{
			return 0;
		}
		int64_t ab = algorithm_lib::number_theory_t::power_mod(a, b, std::numeric_limits<int64_t>::max());
		return closer_number(ab / x * x, ab / x * x + x, ab);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t a, b, x;
			std::cin >> a >> b >> x;
			std::cout << solve(a, b, x) << std::endl;
		}
	}
}