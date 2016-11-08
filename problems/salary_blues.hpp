//https://www.hackerrank.com/challenges/salary-blues
#include <iostream>
#include <vector>
#include <algorithm>
#include <number_theory.hpp>

namespace salary_blues
{
	inline std::vector<int64_t> solve(const std::vector<int64_t>& salaries, std::vector<int64_t>& bonus)
	{
		auto pivot = *std::min_element(salaries.begin(), salaries.end());
		int64_t gcd = 0;
		for (auto salary : salaries)
		{
			if (salary == pivot)
				continue;
			else
				salary -= pivot;
			gcd = algorithm_lib::number_theory_t::gcd(salary, gcd);
		}
		std::vector<int64_t> r;
		for (auto k : bonus)
		{
			r.push_back(algorithm_lib::number_theory_t::gcd(k + pivot, gcd));
		}
		return r;
	}

	inline void solve()
	{
		size_t n, q;
		std::cin >> n >> q;
		std::vector<int64_t> salaries(n);
		for (size_t k = 0; k < n; ++k)
		{
			std::cin >> salaries[k];
		}

		std::vector<int64_t> bonus;
		while (q--)
		{
			int64_t k;
			std::cin >> k;
			bonus.push_back(k);
		}
		for (auto v : solve(salaries, bonus))
			std::cout << v << std::endl;
	}
}