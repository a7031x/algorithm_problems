//https://www.hackerrank.com/contests/master/challenges/richie-rich
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>

namespace richie_rich
{
	inline void solve()
	{
		int n, l;
		std::cin >> n >> l;
		std::string s;
		std::cin >> s;

		int c = 0;
		for (int k = 0; k < s.size() / 2; ++k)
		{
			auto s0 = s[k];
			auto s1 = s[s.size() - 1 - k];
			if (s0 != s1 && ++c > l)
			{
				std::cout << -1 << std::endl;
				return;
			}
		}

		for (int k = 0; k < s.size() / 2 && l; ++k)
		{
			auto& s0 = s[k];
			auto& s1 = s[s.size() - 1 - k];
			if (s0 != s1)
			{
				auto m = std::max(s0, s1);
				if ('9' != m && l > c)
				{
					l -= 2;
					--c;
					s0 = s1 = '9';
				}
				else
				{
					--l;
					--c;
					s0 = s1 = m;
				}
			}
			else
			{
				if (l - c >= 2 && s0 != '9')
				{
					l -= 2;
					s0 = s1 = '9';
				}
			}
		}

		if (l)
			s[s.size() / 2] = '9';

		std::cout << s << std::endl;
	}
}
