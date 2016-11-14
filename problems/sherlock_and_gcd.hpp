//https://www.hackerrank.com/challenges/sherlock-and-gcd
#include <iostream>
#include <vector>
#include <number_theory.hpp>

namespace sherlock_and_gcd
{
	inline bool solve(const std::vector<int>& ar)
	{
		int gcd = 0;
		for (auto v : ar)
		{
			gcd = algorithm_lib::number_theory_t::gcd(v, gcd);
			if (1 == gcd)
				return true;
		}
		return false;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int n;
			std::cin >> n;
			std::vector<int> ar(n);
			for (int k = 0; k < n; ++k)
				std::cin >> ar[k];
			std::cout << (solve(ar) ? "YES" : "NO") << std::endl;
		}
	}
}