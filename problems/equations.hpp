//https://www.hackerrank.com/challenges/equations
#include <unordered_map>
#include <iostream>
#include <number_theory.hpp>
#include <mod.hpp>

namespace equations
{
	inline int solve(int n)
	{
		const int m = 1000007;
		auto factors_within = algorithm_lib::number_theory_t::factors_within(n);
		std::unordered_map<int, int> factors;
		for (int k = 2; k <= n; ++k)
		{
			int v = k;
			while(1 != v)
			{
				++factors[factors_within[v]];
				v /= factors_within[v];
			}
		}
		int64_t r = 1;
		for (auto kv : factors)
		{
			r = algorithm_lib::mod(r * (kv.second * 2 + 1), m);
		}
		return (int)r;
	}

	inline void solve()
	{
		int n;
		std::cin >> n;
		std::cout << solve(n) << std::endl;
	}
}
