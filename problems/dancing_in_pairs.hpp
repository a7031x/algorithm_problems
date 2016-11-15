//https://www.hackerrank.com/challenges/dance-class
#include <math.h>
#include <iostream>

namespace dancing_in_pairs
{
	inline bool solve(int64_t n)
	{
		auto srn = (int64_t)sqrt(n);
		if ((srn + 1) * (srn + 1) == n)
			++srn;
		else if (srn * srn > n)
			--srn;
		return srn % 2 != 0;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t n;
			std::cin >> n;
			std::cout << (solve(n) ? "odd" : "even") << std::endl;
		}
	}
}
