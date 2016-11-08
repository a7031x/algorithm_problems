//https://www.hackerrank.com/challenges/number-of-subsets
#include <iostream>
#include <mod.hpp>

namespace number_of_subsets
{
	inline int64_t solve(int64_t n)
	{
		int64_t m = 1000000007;
		auto r = algorithm_lib::power_mod(2, n, m - 1);
		r = algorithm_lib::mod(r - n, m - 1);
		r = algorithm_lib::power_mod(2, r, m);
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t n;
			std::cin >> n;
			std::cout << solve(n) << std::endl;
		}
	}
}