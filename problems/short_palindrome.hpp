#include <iostream>
#include <vector>
#include <string>
#include <input_output.hpp>

namespace short_palindrome
{
	inline int64_t solve(const std::string& text)
	{
		std::vector<std::vector<size_t>> position(26);
		std::vector<std::vector<size_t>> cache(text.size(), std::vector<size_t>(26, 0));

		for (size_t k = 0; k < text.size(); ++k)
		{
			char c = text[k] - 'a';
			if (k > 0)
				cache[k] = cache[k - 1];
			++cache[k][c];
			position[c].push_back(k);
		}

		int64_t sum = 0;
		for (char c = 0; c < 26; ++c)
		{
			if (2 > position[c].size())
				continue;

			for (char d = 0; d < 26; ++d)
			{
				int64_t F = 0;
				int64_t G = 0;
				for (size_t k = 1; k < position[c].size(); ++k)
				{
					if (d != c)
					{
						size_t right = position[c][k];
						size_t left = position[c][k - 1];
						int64_t x = cache[right - 1][d] - cache[left][d];
						int64_t f = x * (x - 1) / 2;
						F = console::mod(F + f * k + x * G);
						G = console::mod(G + x * k);
					}
					else
					{
						F += (k - 1) * (k - 2) / 2;
					}
					sum = console::mod(sum + F);
				}
			}
		}
		return sum;
	}
}