//https://www.hackerrank.com/contests/master/challenges/gridland-provinces
#include <iostream>
#include <string>
#include <unordered_set>
#include <random>

namespace gridland_provinces
{
	const size_t max_column_size = 600;
	const size_t max_string_size = max_column_size * 2;

	inline void initial_hash_table(int64_t hash_table[max_string_size][128])
	{
		std::default_random_engine generator;
		std::uniform_int_distribution<int64_t> distribution;

		for (size_t k = 0; k < max_string_size; ++k)
			for (size_t j = 0; j < 128; ++j)
			{
				hash_table[k][j] = distribution(generator);
			}
	}

	inline int64_t string_hash(int64_t hash_table[][128], const std::string& text, size_t offset = 0)
	{
		int64_t sum = 0;
		size_t k = offset;
		for (auto c : text)
			sum ^= hash_table[k++][c];
		return sum;
	}

	class warmup_hash_t
	{
	public:
		warmup_hash_t(int64_t hash_table[max_string_size][128], const std::string s[2])
		{
			std::string sr[2] =
			{
				std::string(s[0].crbegin(), s[0].crend()),
				std::string(s[1].crbegin(), s[1].crend())
			};

			size_t length = s[0].size();

			for (size_t row = 0; row < 2; ++row)
			for (size_t column = 0; column < length; ++column)
			{
				std::string left = sr[row].substr(length - column - 1, column + 1) + s[1 - row].substr(0, column + 1);
				std::string right = s[row].substr(column, length - column) + sr[1 - row].substr(0, length - column);
				_cache[row][0][0][column] = string_hash(hash_table, left, 0);
				_cache[row][0][1][column] = string_hash(hash_table, left, length * 2 - left.size());
				_cache[row][1][0][column] = string_hash(hash_table, right, 0);
				_cache[row][1][1][column] = string_hash(hash_table, right, length * 2 - right.size());
			}
		}

		int64_t get_hash(size_t column, size_t row, bool from_end, bool right)
		{
			return _cache[row][right][from_end][column];
		}
	private:
		int64_t _cache[2][2][2][max_column_size];
	};

	inline size_t solve(int64_t hash_table[][128], const std::string s[2])
	{
		std::unordered_set<int64_t> rset;
		std::string sr[2] = { std::string(s[0].crbegin(), s[0].crend()), std::string(s[1].crbegin(), s[1].crend()) };
		std::string curl[2];
		warmup_hash_t cache(hash_table, s);

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
				auto left_hash = cache.get_hash(c0 , r0, false, false);
				auto right_hash = cache.get_hash(c0, r0, false, true);

				if (length - 1 == c0)
					rset.insert(left_hash);

				if (0 == c0)
					rset.insert(right_hash);

				auto curl_ref = curl[c0 % 2 ? (1 - r0) : r0];
				int64_t curl_hash = 0;
				size_t curl_index = (c0 + 1) * 2;

				for (size_t c1 = c0 + 1; c1 < length; ++c1)
				{
					size_t r1 = (c1 - c0) % 2 ? (1 - r0) : r0;
					auto hash = left_hash ^ curl_hash ^ cache.get_hash(c1, r1, true, true);
					rset.insert(hash);
					curl_hash ^= hash_table[curl_index++][curl_ref[c1 * 2]];
					curl_hash ^= hash_table[curl_index++][curl_ref[c1 * 2 + 1]];
				}

				curl_ref = curl[c0 % 2 ? r0 : (1 - r0)];

				curl_hash = string_hash(
					hash_table,
					std::string(
						curl_ref.crbegin() + (length - c0) * 2,
						curl_ref.crbegin() + length * 2),
					(length - c0) * 2);

				for (size_t c1 = 0; c1 < c0; ++c1)
				{
					curl_hash ^= hash_table[--curl_index][curl_ref[c1 * 2]];
					curl_hash ^= hash_table[--curl_index][curl_ref[c1 * 2 + 1]];
					size_t r1 = (c1 - c0) % 2 ? (1 - r0) : r0;
					auto hash = right_hash ^ curl_hash ^ cache.get_hash(c1, r1, true, false);
					rset.insert(hash);
				}
			}
		}
		return rset.size();
	}

	inline int64_t solve(int64_t hash_table[][128], const std::string& s0, const std::string& s1)
	{
		std::string s[2] = { s0, s1 };
		return solve(hash_table, s);
	}

	inline void solve()
	{
		static int64_t hash_table[max_string_size][128];
		initial_hash_table(hash_table);

		int t;
		std::cin >> t;
		while (t--)
		{
			size_t columns;
			std::cin >> columns;
			std::string s0, s1;
			std::cin >> s0 >> s1;
			std::cout << solve(hash_table, s0, s1) << std::endl;
		}
	}
}