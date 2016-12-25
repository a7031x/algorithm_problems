//https://www.hackerrank.com/challenges/power-of-large-numbers
#include <string>
#include <iostream>
#include <number_theory.hpp>

namespace power_of_large_numbers
{
	inline int64_t solve(const std::string& a, const std::string& b)
	{
		int64_t ia = 0, ib = 0;

		for (auto c : a)
		{
			c -= '0';
			ia = algorithm_lib::number_theory_t::mod(ia * 10 + c);
		}

		for (auto c : b)
		{
			c -= '0';
			ib = algorithm_lib::number_theory_t::mod(ib * 10 + c, algorithm_lib::m1e9n7 - 1);
		}
		return algorithm_lib::number_theory_t::power_mod(ia, ib);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			std::string a, b;
			std::cin >> a >> b;
			std::cout << solve(a, b) << std::endl;
		}
	}
}