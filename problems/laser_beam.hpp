#pragma once
#include <number_theory.hpp>

namespace laser_beam
{
	template<size_t N>
	int64_t calc_f(int64_t x, std::unordered_map<int64_t, int>& cache, int64_t cache2[N])
	{
		if (x < N)
		{
			if (cache2[(size_t)x])
				return cache2[(size_t)x];
		}
		else if (cache.count(x))
			return cache[x];

		int64_t sum = 0;
		int sqrt_x = (int)sqrt(x);
		int g0 = std::min(sqrt_x, int(x / N));

		for (int g = 2; g <= g0; ++g)
		{
			auto x1 = x / g;
			sum += calc_f<N>(x1, cache, cache2);
		}

		for (int g = std::max(g0 + 1, 2); g <= sqrt_x; ++g)
		{
			size_t x1 = size_t(x / g);
			sum += cache2[x1] ? cache2[x1] : calc_f<N>(x1, cache, cache2);
		}

		auto max_k = x / (std::max(sqrt_x, 1) + 1);
		auto xdk = x;
		for (int k = 1; k <= max_k; ++k)
		{
			auto next_s = x / (k + 1);
			auto range = xdk - next_s;
			xdk = next_s;
			auto f = cache2[k] ? cache2[k] : calc_f<N>(k, cache, cache2);
			sum += range * f;
		}

		sum = algorithm_lib::number_theory_t::mod(sum);

		auto g = algorithm_lib::number_theory_t::power_mod<int64_t>(x * 2 + 1, 3) - 1;
		auto r = algorithm_lib::number_theory_t::mod(g - sum);
		if (x < N)
			cache2[(size_t)x] = (int)r;
		else
			cache[x] = (int)r;
		return r;
	}

	template<size_t N>
	int64_t solve(int64_t n, int64_t m, int64_t d, std::unordered_map<int64_t, int>& cache, int64_t cache2[N])
	{
		auto r = (int)(n % d);
		auto s = (int)((m + d - 1) / d * d);

		int64_t sum = 0;
		int sqrt_n = (int)sqrt(n);

		for (auto sections = s; sections <= sqrt_n; sections += (int)d)
		{
			auto b0 = std::max(n / (sections + 1), (int64_t)r);
			auto b1 = n / sections;
			auto f0 = calc_f<N>(b0, cache, cache2);
			auto f1 = calc_f<N>(b1, cache, cache2);
			sum = algorithm_lib::number_theory_t::mod(sum + f1 - f0);
		}

		for (auto b = r + 1; b <= n / (sqrt_n + 1); ++b)
		{
			auto sections = n / b;
			if (sections < m)
				break;
			if (sections % d != 0)
				continue;
			auto f0 = calc_f<N>(b - 1, cache, cache2);
			auto f1 = calc_f<N>(b, cache, cache2);
			sum = algorithm_lib::number_theory_t::mod(sum + f1 - f0);
		}

		return sum;
	}

	void solve()
	{
		int t;
		std::cin >> t;
		std::unordered_map<int64_t, int> cache;
		const int N = 1000000;
		static int64_t cache2[N] = { 0 };

		while (t--)
		{
			int64_t n, m, d;
			std::cin >> n >> m >> d;
			std::cout << solve<N>(n, m, d, cache, cache2) << std::endl;
		}
	}
}