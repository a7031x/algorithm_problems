//https://www.hackerrank.com/challenges/littlepandapower
#include <iostream>
#include <mod.hpp>

namespace little_panda_power
{
	inline int64_t solve(int64_t a, int64_t b, int64_t x)
	{
		if (b >= 0)
			return algorithm_lib::power_mod(a, b, x);
		int64_t m[2][2] = { 1, 0, 0, 1 };
		a = algorithm_lib::power_mod(a, -b, x);
		b = x;
		do {
			auto r = a % b;
			auto q = a / b;
			a = b;
			b = r;
			int64_t m0[2][2] = { 0, 1, 1, -q };
			algorithm_lib::matrix_multiply_inplace<2, 2, 2>(m0, m, m);
			if (1 == r)
				break;
		} while (true);
		return algorithm_lib::mod(m[1][0], x);
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