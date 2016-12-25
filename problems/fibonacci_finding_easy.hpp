//https://www.hackerrank.com/challenges/fibonacci-finding-easy
#include <iostream>
#include <number_theory.hpp>

namespace fibonacci_finding_easy
{
	inline int64_t solve(int64_t a, int64_t b, int64_t n)
	{
		int64_t m[2][2] = { 1, 1, 1, 0 };
		algorithm_lib::number_theory_t::matrix_power_mod<2>(m, n - 1);
		return algorithm_lib::number_theory_t::mod(m[0][0] * b + m[0][1] * a);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t a, b, n;
			std::cin >> a >> b >> n;
			std::cout << solve(a, b, n) << std::endl;
		}
	}
}