//https://www.hackerrank.com/challenges/gcd-sequence
#include <iostream>
#include <number_theory.hpp>

namespace gcd_sequence
{
	inline std::vector<int> f_sum(int n, int k)
	{
		std::vector<int> r(n);
		r[0] = 1;
		int64_t t = 1;
		for (int x = 1; x < n; ++x)
		{
			t = algorithm_lib::number_theory_t::mod(t * (k + x - 1));
			t = algorithm_lib::number_theory_t::modular_multiplicative_inverse<int>(x, (int)t, algorithm_lib::m1e9n7);
			r[x] = (int)algorithm_lib::number_theory_t::mod(r[x - 1] + t);
		}
		return r;
	}

	inline int solve(int n, int k)
	{
		auto mu = algorithm_lib::number_theory_t::mobius(n);
		auto factors = algorithm_lib::number_theory_t::batch_factors(mu);
		auto fs = f_sum(n, k - 1);
		auto r = 0;
		for (int k = 1; k <= n; ++k)
		{
			int f = fs[n - k];
			for (auto factor : factors[k])
			{
				if (1 == factor)
					continue;
				f = algorithm_lib::number_theory_t::mod(f + mu[factor] * fs[(n - k) / factor]);
			}
			r = algorithm_lib::number_theory_t::mod(r + f);
		}
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int n, k;
			std::cin >> n >> k;
			std::cout << solve(n, k) << std::endl;
		}
	}
}
