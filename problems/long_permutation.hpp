//https://www.hackerrank.com/challenges/long-permutation
#include <vector>
#include <iostream>
#include <iomanip>

namespace long_permutation
{
	inline int64_t solve_naive(const std::vector<int>& transform, int64_t index)
	{
		std::vector<int> ar((size_t)index + transform.size() + 1);
		for (size_t k = 0; k < ar.size(); ++k)
			ar[k] = k;

		std::vector<int> buf(transform.size());
		for (int64_t offset = 0; offset < index; ++offset)
		{
			for (size_t k = 0; k < transform.size(); ++k)
				buf[k] = ar[(size_t)offset + transform[k]];
			for (size_t k = 0; k < transform.size(); ++k)
				ar[(size_t)offset + k + 1] = buf[k];

			for(size_t k = 1; k < ar.size(); ++k)
				std::cout << std::setfill('0') << std::setw(2) << ar[k] << ' ';
			std::cout << std::endl;
		}
		return ar[(size_t)index];
	}

	inline int64_t solve(const std::vector<int>& transform, int64_t index)
	{
		int64_t x = 0;
		for (int64_t offset = 0; offset < transform.size(); ++offset)
		{
			x = transform[(size_t)x];
			if (x >= transform.size())
				return index - offset + transform.size() - 1;
			if (offset + 1 == index)
				return x;
		}
		return 0;
	}

	inline void solve()
	{
		size_t n;
		int64_t index;
		std::cin >> n >> index;
		std::vector<int> transform(n);
		for (size_t k = 0; k < n; ++k)
			std::cin >> transform[k];
		std::cout << solve(transform, index) << std::endl;
	}
}