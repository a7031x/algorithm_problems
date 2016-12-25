//https://www.hackerrank.com/challenges/ichigo-and-cubes
#include <iostream>
#include <number_theory.hpp>

namespace ichigo_and_cubes
{
	inline int64_t uncut_cubes(int64_t a, int64_t b)
	{
		int r = 1;
		--a;
		--b;
		auto x = (a / algorithm_lib::m1e9n7) & ~1;
		auto y = (b / algorithm_lib::m1e9n7) & ~1;
		a -= x * algorithm_lib::m1e9n7;
		b -= y * algorithm_lib::m1e9n7;
		return algorithm_lib::number_theory_t::mod(a * b / 2);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t p, q, r;
			std::cin >> p >> q >> r;
			
			auto s0 = uncut_cubes(p, q) + uncut_cubes(p, r) + uncut_cubes(q, r);
			auto s1 = algorithm_lib::number_theory_t::mod(p + q + r - 2);
			std::cout << algorithm_lib::number_theory_t::mod(s0 + s1) << std::endl;
		}
	}
}