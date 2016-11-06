//https://www.hackerrank.com/challenges/manasa-loves-maths
#include <string>
#include <vector>
#include <set>
#include <iostream>

namespace manasa_loves_maths
{
	std::vector<int> calculate_digits(const std::string& s)
	{
		std::vector<int> sum(10, 0);
		for (auto c : s)
			++sum[c - '0'];
		if (s.size() < 3)
			sum[0] += 3 - s.size();
		return sum;
	}

	inline std::set<std::vector<int>> calculate_octs()
	{
		std::set<std::vector<int>> octs;
		for (int k = 0; k < 1000; ++k)
		{
			if (0 == k % 8)
				octs.insert(calculate_digits(std::to_string(k)));
		}
		return octs;
	}

	inline bool solve(const std::string& s, const std::set<std::vector<int>>& octs)
	{
		if (s.size() <= 2)
		{
			return std::stoi(s) % 8 == 0 || std::stoi(std::string(s.crbegin(), s.crend()), nullptr, 10) % 8 == 0;
		}

		auto sum = calculate_digits(s);
		for (auto& oct : octs)
		{
			bool flag = true;
			for (size_t k = 0; k < 10; ++k)
				if (oct[k] > sum[k])
				{
					flag = false;
					break;
				}
			if (flag)
				return true;
		}

		return false;
	}

	inline void solve()
	{
		auto octs = calculate_octs();
		int t;
		std::cin >> t;
		while (t--)
		{
			std::string s;
			std::cin >> s;
			std::cout << (solve(s, octs) ? "YES" : "NO") << std::endl;
		}
	}
}
