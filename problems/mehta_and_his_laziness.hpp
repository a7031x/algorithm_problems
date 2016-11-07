//https://www.hackerrank.com/challenges/mehta-and-his-laziness
#include <iostream>
#include <math.h>
#include <number_theory.hpp>

namespace mehta_and_his_laziness
{
	inline std::vector<std::pair<int, int>> solve(int n)
	{
		std::vector<std::pair<int, int>> r(n + 1, { 0, 0 });
		for (int i = 1; i <= n; ++i)
		{
			int sr = (int)sqrt(i);
			bool even_sqrt = false;
			if ((sr & 1) == 0 && sr * sr == i)
				even_sqrt = true;
			for (int j = i * 2; j <= n; j += i)
			{
				++r[j].second;
				if (even_sqrt)
					++r[j].first;
			}
		}

		for (auto& e : r)
		{
			if (0 == e.first)
				continue;
			auto factor = algorithm_lib::number_theory_t::gcd(e.second, e.first);
			e.first /= factor;
			e.second /= factor;
		}
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		auto r = solve(1000000);
		while (t--)
		{
			int n;
			std::cin >> n;
			if (r[n].first)
				std::cout << r[n].first << "/" << r[n].second << std::endl;
			else
				std::cout << 0 << std::endl;
		}
	}
}