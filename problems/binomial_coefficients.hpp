//https://www.hackerrank.com/challenges/binomial-coefficients
#include <string>
#include <iostream>
#include <big_number.hpp>

namespace binomial_cofficients
{
	inline std::string solve(const std::string& n, int p)
	{
		std::vector<int> mi;
		std::vector<int> ni;
		for (auto c : n)
			ni.push_back(c - '0');

		mi = algorithm_lib::change_system(ni.begin(), ni.end(), 10, p);
		std::vector<int> ri(1, 1);
		for (auto v : mi)
			algorithm_lib::multiply(ri, 10, v + 1, 0);
		std::reverse(ri.begin(), ri.end());
		ni = algorithm_lib::add(ni, std::vector<int>(), 10, 1);
		auto r = algorithm_lib::substract(ni, ri, 10);
		std::string rs;
		for (auto v : r)
			rs += '0' + v;
		return rs;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			std::string n;
			int p;
			std::cin >> n >> p;
			std::cout << solve(n, p) << std::endl;
		}
	}
}
