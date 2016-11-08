//https://www.hackerrank.com/challenges/help-mike
#include <iostream>

namespace help_mike
{
	int64_t solve(int64_t n, int64_t k)
	{
		int64_t r = 0;
		int64_t n_align = n / k;
		int64_t n_remainder = n % k;
		for (int64_t i = 0; i <= k / 2; ++i)
		{
			auto c0 = n_align + (i && i <= n_remainder);
			if (i * 2 % k == 0)
			{
				r += c0 * (c0 - 1) / 2;
			}
			else
			{
				auto c1 = n_align + (k - i <= n_remainder);
				r += c0 * c1;
			}
		}
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t n, k;
			std::cin >> n >> k;
			std::cout << solve(n, k) << std::endl;
		}
	}
}
