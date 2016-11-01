//https://www.hackerrank.com/challenges/camelcase
#include <string>
#include <iostream>
#include <algorithm>

namespace camel_case
{
	inline void solve()
	{
		std::string s;
		std::cin >> s;
		std::cout << std::count_if(s.begin(), s.end(), [](char c) {return toupper(c) == c; }) + 1 << std::endl;
	}
}