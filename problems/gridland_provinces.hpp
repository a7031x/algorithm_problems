
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <set>

namespace gridland_provinces
{
	inline bool valid_move(size_t row, size_t column, const std::vector<char> grid[2])
	{
		if (0 == grid[row][column] || grid[row][column - 1] && grid[row][column + 1])
			return false;
		else
			return true;
	}

	inline void solve(
		size_t start_row, size_t start_column,
		std::vector<char> grid[2],
		std::vector<char>& text,
		std::set<std::string>& hash,
		size_t index)
	{
		char c = grid[start_row][start_column];
		grid[start_row][start_column] = 0;
		text[index++] = c;
		if (grid[0].size() * 2 - 4 > index)
		{
			if (valid_move(1 - start_row, start_column, grid))
				solve(1 - start_row, start_column, grid, text, hash, index);
			if (grid[start_row][start_column - 1])
				solve(start_row, start_column - 1, grid, text, hash, index);
			if (grid[start_row][start_column + 1])
				solve(start_row, start_column + 1, grid, text, hash, index);
		}
		else
		{
			hash.insert(text.data());
		}
		grid[start_row][start_column] = c;
		text[--index] = 0;
	}

	inline size_t solve(const std::string s[2])
	{
		std::set<std::string> rset;
		std::string sr[2] = { std::string(s[0].crbegin(), s[0].crend()), std::string(s[1].crbegin(), s[1].crend()) };
		std::string curl[2];
		size_t length = s[0].size();
		for (size_t k = 0; k < length; ++k)
		{
			curl[0] += s[k % 2][k];
			curl[0] += s[1 - k % 2][k];
			curl[1] += s[1 - k % 2][k];
			curl[1] += s[k % 2][k];
		}

		for (size_t c0 = 0; c0 < length; ++c0)
		{
			for (size_t r0 = 0; r0 < 2; ++r0)
			{
				std::string left, right;
				left = std::string(s[r0].crbegin() + (length - 1 - c0), s[r0].crend());
				left += s[1 - r0].substr(0, c0 + 1);

				right = s[r0].substr(c0, length - c0);
				right += std::string(s[1 - r0].crbegin(), s[1 - r0].crbegin() + (length - c0));

				if (left.size() == length * 2)
					rset.insert(left);

				if (right.size() == length * 2)
					rset.insert(right);

				auto curl_ref = curl[c0 % 2 ? (1 - r0) : r0];
				std::string left_curl;
				for (size_t c1 = c0 + 1; c1 < length; ++c1)
				{
					size_t r1 = (c1 - c0) % 2 ? (1 - r0) : r0;
					std::string forward = s[r1].substr(c1, length - c1);
					std::string backward(sr[1 - r1].cbegin(), sr[1 - r1].cbegin() + (length - c1));
					rset.insert(left + left_curl + forward + backward);
					left_curl.push_back(curl_ref[c1 * 2]);
					left_curl.push_back(curl_ref[c1 * 2 + 1]);
				}

				curl_ref = curl[c0 % 2 ? r0 : (1 - r0)];
				std::string right_curl(
						curl_ref.crbegin() + (length - c0) * 2,
						curl_ref.crbegin() + length * 2);

				for (size_t c1 = 0; c1 < c0; ++c1)
				{
					right_curl.resize(right_curl.size() - 2);
					size_t r1 = (c1 - c0) % 2 ? (1 - r0) : r0;
					std::string forward(sr[r1].cbegin() + (length - c1 - 1), sr[r1].cend());
					std::string backward = s[1 - r1].substr(0, c1 + 1);
					rset.insert(right + right_curl + forward + backward);
				}
			}
		}
		return rset.size();
	}

	inline int64_t solve(size_t columns, const std::string& s0, const std::string& s1)
	{
	/*	std::vector<char> grid[2];
		grid[0].push_back(0);
		grid[1].push_back(0);
		for (size_t k = 0; k < columns; ++k)
		{
			grid[0].push_back(s0[k]);
			grid[1].push_back(s1[k]);
		}
		grid[0].push_back(0);
		grid[1].push_back(0);

		std::set<std::string> hash;
		std::vector<char> text(columns * 2 + 1, 0);
		for (size_t k = 1; k <= columns; ++k)
		{
			solve(0, k, grid, text, hash, 0);
			solve(1, k, grid, text, hash, 0);
		}
		*/
		std::string s[2] = { s0, s1 };
		return solve(s);
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			size_t columns;
			std::cin >> columns;
			std::string s0, s1;
			std::cin >> s0 >> s1;
			std::cout << solve(columns, s0, s1) << std::endl;
		}
	}
}