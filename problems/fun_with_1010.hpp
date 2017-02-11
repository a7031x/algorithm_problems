#include <iostream>
#include <number_theory.hpp>

namespace fun_with_1010
{
	using namespace algorithm_lib;

	const int m2e63 = 2000003;

	inline int solve(const std::vector<int>& factors, int64_t n, int64_t k)
	{
		int64_t power_2n1 = number_theory_t::power_mod<int64_t>(2, n - 1, m2e63);
		int64_t power_2n = number_theory_t::mod(power_2n1 * 2, m2e63);
		int64_t cnr = number_theory_t::binomial_coeficient(factors, 2 * n, n, m2e63);
		int64_t n_over_2 = number_theory_t::modular_multiplicative_inverse<int64_t>(2, n, m2e63);
		int64_t p = number_theory_t::mod(cnr * n_over_2, m2e63);
		int64_t s1 = number_theory_t::mod(power_2n1 * (n + 2), m2e63);
		int64_t s2 = number_theory_t::mod(power_2n * s1 - p, m2e63);
		int64_t s3 = number_theory_t::mod(power_2n * s2 - power_2n1 * p, m2e63);
		int64_t s = number_theory_t::mod(s3 - 3 * s2 + 3 * s1 - n - 1, m2e63);
		int64_t remain = number_theory_t::mod((k - n) * number_theory_t::power_mod(power_2n - 1, (int64_t)3, m2e63), m2e63);
		return (int)number_theory_t::mod(s + remain, m2e63);
	}

	inline void solve()
	{
		auto factors = number_theory_t::n_factors(m2e63 - 1, m2e63);
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t m, k;
			std::cin >> m >> k;
			std::cout << solve(factors, m - 1, k) << std::endl;
		}
	}
}