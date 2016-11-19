//https://www.hackerrank.com/challenges/lucy-and-flowers
#include <iostream>
#include <binomial_coefficient.hpp>

namespace lucy_and_flowers
{
	inline void solve()
	{
		const int64_t moduli = 1000000009;
		algorithm_lib::binomial_coefficient_t bc(5000, moduli);
		std::vector<int64_t> patterns(5001);
		patterns[0] = patterns[1] = 1;
		for (size_t k = 2; k < patterns.size(); ++k)
		{
			int64_t s = 0;
			for (size_t i = 0; i < k / 2; ++i)
			{
				s += patterns[i] * patterns[k - 1 - i];
				s %= moduli;
			}
			s *= 2;
			if (k & 1)
				s += patterns[k / 2] * patterns[k / 2];
			s %= moduli;
			patterns[k] = s;
		}
		int t;
		std::cin >> t;
		while (t--)
		{
			int n;
			std::cin >> n;
			int64_t r = 0;
			for (int k = 1; k <= n; ++k)
			{
				r += bc.cnr(n, k) * patterns[k];
				r %= moduli;
			}
			std::cout << r << std::endl;
		}
	}
}