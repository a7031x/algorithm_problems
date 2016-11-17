//https://www.hackerrank.com/challenges/strange-numbers
#include <iostream>

namespace strange_numbers
{
	inline int64_t solve(int64_t start, int64_t end, int64_t root)
	{
		int64_t digit_limit = 100;
		int64_t digit_number = 2;
		int64_t r = 0;
		int64_t max_root = 1000000000000000000LL / 17;
		while(root <= end)
		{
			if (root >= start)
				++r;

			if (root > max_root)
				break;

			while (root * digit_number >= digit_limit)
			{
				digit_number++;
				digit_limit *= 10;
			}
			if (root * (digit_number + 1) >= digit_limit)
				r += solve(start, end, root * (digit_number + 1));
			root *= digit_number;
		}

		return r;
	}

	inline int64_t solve(int64_t start, int64_t end)
	{
		int64_t r = 0;
		for (auto root : { 5, 6, 7, 8, 9 })
			r += solve(start, end, root);
		if (start <= 4)
			r += 5 - start;
		return r;
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int64_t l, r;
			std::cin >> l >> r;
			std::cout << solve(l, r) << std::endl;
		}
	}
}