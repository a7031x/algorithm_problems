//https://www.hackerrank.com/challenges/john-and-gcd-list
#include <iostream>
#include <number_theory.hpp>

namespace john_and_gcd_list
{
	inline std::vector<int> solve(const std::vector<int>& a)
	{
		std::vector<int> r;
		int last = 1;
		for (auto v : a)
		{
			r.push_back(v * last / algorithm_lib::number_theory_t::gcd(v, last));
			last = v;
		}
		r.push_back(a.back());
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int n;
			std::cin >> n;
			std::vector<int> a(n);
			for (int k = 0; k < n; ++k)
				std::cin >> a[k];
			for (auto v : solve(a))
				std::cout << v << " ";
			std::cout << std::endl;
		}
	}
}