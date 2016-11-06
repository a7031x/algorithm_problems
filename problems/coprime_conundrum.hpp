//https://www.hackerrank.com/challenges/arthur-and-coprimes
//http://laurentmazare.github.io/2014/09/14/counting-coprime-pairs/
//http://blog.csdn.net/a1s4z5/article/details/52738331
//http://baike.baidu.com/link?url=4z6TSmOFuWdYMs2wQsSlcj3fOEYF7yX3_KMzQD7UEWDK5at9a4DbwJgmXASFa420u6Q-ZM1RCJmQo3cJAz7J5a

#include <list>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>

namespace coprime_conundrum
{
	inline int64_t solve_naive(int64_t n)
	{
		std::list<std::pair<int64_t, int64_t>> q;
		q.push_back({ 2, 1 });
		q.push_back({ 3, 1 });

		int64_t r = 0;
		while (q.size())
		{
			auto& p = q.front();
			if (p.second != 1)
				++r;
			std::pair<int64_t, int64_t> p1 = { p.first * 2 - p.second, p.first };
			std::pair<int64_t, int64_t> p2 = { p.first * 2 + p.second, p.first };
			std::pair<int64_t, int64_t> p3 = { p.second * 2 + p.first, p.second };
			if (p1.first * p1.second <= n)
				q.push_back(p1);
			if (p2.first * p2.second <= n)
				q.push_back(p2);
			if (p3.first * p3.second <= n)
				q.push_back(p3);
			q.pop_front();
		}
		return r;
	}

	inline std::vector<std::vector<int>> pairs(int n)
	{
		std::vector<std::vector<int>> r(n + 1);
		std::list<std::pair<int, int>> q;
		q.push_back({ 2, 1 });
		q.push_back({ 3, 1 });
		while (q.size())
		{
			auto& p = q.front();
			r[p.first].push_back(p.second);
			std::pair<int, int> p1 = { p.first * 2 - p.second, p.first };
			std::pair<int, int> p2 = { p.first * 2 + p.second, p.first };
			std::pair<int, int> p3 = { p.second * 2 + p.first, p.second };
			if (p1.first <= n)
				q.push_back(p1);
			if (p2.first <= n)
				q.push_back(p2);
			if (p3.first <= n)
				q.push_back(p3);
			q.pop_front();
		}
		for (auto& v : r)
			std::sort(v.begin(), v.end());
		return r;
	}

	inline std::vector<int> calc_max_factors(int n)
	{
		std::vector<int> r(n + 1);
		for (int k = 0; k <= n; ++k)
			r[k] = k;
		for (int k = 2; k <= n / 2; ++k)
		{
			if (k == r[k])
			{
				for (int kk = k * 2; kk <= n; kk += k)
					r[kk] = k;
			}
		}
		return r;
	}

	inline int64_t phi(const std::vector<int>& max_factors, int64_t n)
	{
		int k = (int)n;
		do {
			auto factor = max_factors[k];
			n -= n / factor;
			do {
				k /= factor;
			} while (max_factors[k] == factor);
		} while (1 != k);
		return n;
	}

	inline int64_t pairs(
		const std::vector<int>& max_factors,
		const std::vector<std::vector<int>>& pairs,
		int64_t start, int64_t limit)
	{
		if (1 == start)
			return limit - 1;
		int k = (int)start;
		auto limit_floor = limit / start * start;
		int64_t phi_limit = limit / start * start;
		int64_t phi_start = start;
		do {
			auto factor = max_factors[k];
			phi_limit -= phi_limit / factor;
			phi_start -= phi_start / factor;
			do {
				k /= factor;
			} while (max_factors[k] == factor);
		} while (1 != k);
		int64_t r = phi_limit - phi_start;
		if (limit_floor != limit)
		{
			auto& coprimes = pairs[(size_t)start];
			auto offset = std::distance(coprimes.begin(), std::upper_bound(coprimes.begin(), coprimes.end(), limit - limit_floor));
			r += offset;
		}
		return r;
	}

	inline int64_t solve_euler_totient(int64_t n)
	{
		int sqrt_n = (int)sqrt(n);
		auto max_factors = calc_max_factors(sqrt_n);
		auto pairs_cache = pairs(sqrt_n);
		int64_t r = 0;
		for (int k = 2; k <= sqrt_n; ++k)
			r += pairs(max_factors, pairs_cache, k, n / k);
		return r;
	}

	inline std::vector<int> calc_mobius(int n)
	{
		std::vector<int> r(n + 1, -1);
		r[1] = 1;
		for (int i = 2; i <= n; ++i)
			for (int j = i * 2; j <= n; j += i)
				r[j] -= r[i];
		return r;
	}

	inline std::vector<std::vector<int>> calc_factors(const std::vector<int>& mu)
	{
		int n = (int)mu.size() - 1;
		std::vector<std::vector<int>> r(n + 1);
		for (int i = 1; i <= n; ++i)
		{
			if (mu[i])
				for (int j = i; j <= n; j += i)
					r[j].push_back(i);
		}
		return r;
	}

	inline int64_t calc_pairs(const std::vector<int>& mu, const std::vector<std::vector<int>>& factors, int k, int64_t limit)
	{
		int64_t r = 0;
		for (auto factor : factors[k])
		{
			r += mu[factor] * (limit - k) / factor;
		}
		return r;
	}

	inline int64_t solve_mobius(int64_t n)
	{
		int sqrt_n = (int)sqrt(n);
		auto mu = calc_mobius(sqrt_n);
		auto factors = calc_factors(mu);
		int64_t r = 0;
		for (int k = 2; k <= sqrt_n; ++k)
		{
			r += calc_pairs(mu, factors, k, n / k);
		}
		return r;
	}

	inline void solve()
	{
		int64_t n;
		std::cin >> n;
		std::cout << solve_mobius(n) << std::endl;
	}
}