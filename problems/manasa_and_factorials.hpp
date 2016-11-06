//https://www.hackerrank.com/challenges/manasa-and-factorials
#include <math.h>
#include <iostream>

namespace manasa_and_factorials
{
	inline int64_t solve(int64_t x)
	{
		int64_t n = 4 * x + 1;
		while (true)
		{
			int64_t m = (int64_t)floor(log(n) / log(5));
			int64_t n1 = 0;
			int64_t y = n;
			for (int64_t k = 0; k < m; ++k)
			{
				n1 += y / 5;
				y /= 5;
			}
			if (n1 >= x)
				break;
			else
				n = (n + 5) / 5 * 5;
		}
		return n;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t v;
			std::cin >> v;
			std::cout << solve(v) << std::endl;
		}
	}
}