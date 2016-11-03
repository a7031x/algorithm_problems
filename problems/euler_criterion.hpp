//https://www.hackerrank.com/challenges/eulers-criterion
//https://en.wikipedia.org/wiki/Euler%27s_criterion
#include <mod.hpp>
#include <iostream>

namespace euler_criterion
{
	inline bool solve(int a, int p)
	{
		if (2 == p)
			return 1 >= a;
		if (0 == a)
			return true;
		return algorithm_lib::power_mod(a, (p - 1) / 2, p) == 1;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int a, p;
			std::cin >> a >> p;
			std::cout << (solve(a, p) ? "YES" : "NO") << std::endl;
		}
	}
}