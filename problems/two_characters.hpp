//https://www.hackerrank.com/challenges/two-characters
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>

namespace two_characters
{
	inline void update(int& s, int c)
	{
		if (-1 == s)
			return;

		if (s != c)
			s = c;
		else
			s = -1;
	}

	inline void solve()
	{
		std::string s;
		int count;
		std::cin >> count >> s;
		int distincts[26][26];
		std::memset(distincts, -2, sizeof(distincts));

		for (auto c : s)
		{
			c -= 'a';
			for (int k = 0; k < 26; ++k)
			{
				update(distincts[k][c], c);
				update(distincts[c][k], c);
			}
		}

		std::ptrdiff_t r = 0;
		for (int x = 0; x < 26; ++x)
		{
			for (int y = 0; y < 26; ++y)
			{
				auto count = std::count(s.begin(), s.end(), x + 'a') * 2;
				if (-1 != distincts[x][y])
				{
					int carry = (distincts[x][y] == x);
					r = std::max(r, count - carry);
					if(0 == carry)
						break;
				}
			}
		}
		std::cout << (r < 2 ? 0 : r) << std::endl;
	}
}

/*
int main()
{
	console::redirect_string("1 a");
	two_characters::solve();
	console::redirect_string("2 ab");
	two_characters::solve();
	console::redirect_string("10 beabeefeab");
	two_characters::solve();
	return 0;
}
*/