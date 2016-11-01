//https://www.hackerrank.com/contests/master/challenges/short-palindrome
#include <iostream>
#include <vector>
#include <string>
#include <console.hpp>

namespace short_palindrome
{
	inline int64_t solve(const std::string& text)
	{
		int64_t closed_pairs[26] = { 0 };
		int64_t third_input[26][26] = { 0 };
		int64_t second_input[26][26] = { 0 };
		int64_t first_input[26] = { 0 };

		int64_t sum = 0;

		for (auto c : text)
		{
			c -= 'a';
			for (size_t k = 0; k < 26; ++k)
				sum = console::mod(sum + third_input[c][k]);
			for (size_t k = 0; k < 26; ++k)
			{
				third_input[k][c] = console::mod(third_input[k][c] + second_input[k][c]);
				second_input[k][c] = console::mod(second_input[k][c] + first_input[k]);
			}
			++first_input[c];
		}
		return sum;
	}

	inline void solve()
	{
		std::string s;
		std::cin >> s;
		std::cout << solve(s) << std::endl;
	}
}