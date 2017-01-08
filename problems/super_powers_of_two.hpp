#include <iostream>
#include <number_theory.hpp>

namespace super_powers_of_two
{
	int solve(int a, int b)
	{
		int t = 0;
		while (b % 2 == 0)
		{
			b /= 2;
			++t;
		}

		if (a < 31 && (1 << a) <= t)
			return (1 << (1 << a)) % (b * (1 << t));

		if (1 == b)
			return 0;

		auto primes = algorithm_lib::number_theory_t::distinct_prime_factors(b);
		int m0 = 1;
		int m1 = b;
		for (auto p : primes)
		{
			m0 *= (p - 1);
			m1 /= p;
		}
		m0 *= m1;
		m0 = algorithm_lib::number_theory_t::power_mod(2, a, m0);
		m1 = algorithm_lib::number_theory_t::power_mod(2, m0 - t, b);
		return m1 * (1 << t);
	}

	void solve()
	{
		int a, b;
		std::cin >> a >> b;
		std::cout << solve(a, b) << std::endl;
	}
}
