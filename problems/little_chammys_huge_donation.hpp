//https://www.hackerrank.com/challenges/little-chammys-huge-donation
#include <iostream>
#include <math.h>

namespace little_chammys_huge_donation
{
	inline int64_t solve(int64_t v)
	{
		int64_t r = (int64_t)pow(3 * v, 1.0 / 3);
		while (r * (r + 1) * (2 * r + 1) > 6 * v)
			--r;
		return r;
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