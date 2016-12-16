//https://www.hackerrank.com/challenges/minion-of-the-year
#include <iostream>
#include <algorithm>
#include <number_theory.hpp>

namespace minion_of_the_year
{
	using namespace algorithm_lib;
	inline int64_t calc(int64_t a, int64_t b, int64_t x, int64_t y, int64_t oc, int64_t od)
	{
		if (x < 0 || y < 0)
			return std::numeric_limits<int64_t>::max();

		if (0 == x)
			x = oc;
		if (0 == y)
			y = od;

		return (int64_t)a * x + (int64_t)b * y;
	}

	int64_t test_by_step(int64_t a, int64_t b, int64_t c, int64_t d, int64_t m)
	{
		auto min_step_x = number_theory_t::gcd(c, m);
		auto min_step_y = number_theory_t::gcd(d, m);
		auto oc = m / min_step_x;
		auto od = m / min_step_y;
		if (0 == c)
			return b * od + a;
		else if (0 == d)
			return a * oc + b;
		auto x = number_theory_t::lcm(c, min_step_y) / c;
		auto step_y = number_theory_t::lcm(d, min_step_x) / d;
		auto y = number_theory_t::modular_multiplicative_inverse(d, c * x % m, m);
		auto lm = od - y;

		auto min_r = calc(a, b, x, y, oc, od);

		auto iy = y ? y : od;
		auto sx = number_theory_t::modular_multiplicative_inverse(c, d * step_y, m);
		while (true)
		{

			auto y = (lm + iy - 1) / iy * iy - lm;
			lm = iy;
			iy = y;
			if (0 == y)
				y = step_y;
			auto x = sx * (y / step_y) % oc;

			auto r = calc(a, b, x, y, oc, od);
			if (min_r <= r)
				break;
			min_r = r;
			if (1 == y)
				break;
		}

		return min_r;
	}

	int64_t solve(int64_t a, int64_t b, int64_t c, int64_t d, int64_t p,
		int64_t pr, const std::unordered_map<int64_t, size_t>& precalc)
	{
		c %= p;
		d %= p;

		if (0 == c && 0 == d)
			return a + b;

		int64_t length_c = number_theory_t::dlog(pr, p, c, precalc);
		int64_t length_d = number_theory_t::dlog(pr, p, d, precalc);

		if (-1 == length_c || -1 == length_d)
			return -1;

		return test_by_step(a, b, length_c, length_d, p - 1);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--) {
			int64_t p, q;
			std::cin >> p >> q;
			int64_t pr = number_theory_t::find_primitive_root(p);
			auto precalc = number_theory_t::dlog_precalc(pr, p, 200000ll);
			while (q--) {
				int64_t a, b, c, d;
				std::cin >> a >> b >> c >> d;
				auto r = solve(a, b, c, d, p, pr, precalc);
				if (-1 != r)
					std::cout << r << std::endl;
				else
					std::cout << "wala" << std::endl;
			}
		}
	}
}
