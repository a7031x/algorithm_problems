//https://www.hackerrank.com/challenges/easy-math
#include <iostream>
#include <algorithm>
#include <number_theory.hpp>

namespace easy_math
{
	inline int solve(int x)
	{
		auto factors = algorithm_lib::number_theory_t::prime_factors(x);
		int twos = 0;
		int fives = 0;
		int y = 1;
		for (auto factor : factors)
		{
			switch (factor)
			{
			case 2: ++twos; break;
			case 5: ++fives; break;
			default:
				y *= factor;
				break;
			}
		}
		int a = 0;
		int m = 0;
		do {
			m = (m * 10 + 1) % y;
			++a;
		} while (m);

		int b = std::max(fives, twos - 2);
		return 2 * a + b;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int n;
			std::cin >> n;
			std::cout << solve(n) << std::endl;
		}
	}
}