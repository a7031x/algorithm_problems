//https://www.hackerrank.com/challenges/ants
#include <vector>
#include <algorithm>
#include <iostream>

namespace ants
{
	inline int distance(const std::vector<int>& ar, size_t a, size_t b)
	{
		a %= ar.size();
		b %= ar.size();
		return abs(ar[a] - ar[b]) % 1000;
	}

	inline int64_t solve(std::vector<int>& ar)
	{
		std::sort(ar.begin(), ar.end());
		int64_t r = 0;
		size_t offset = 0;
		for (; offset < ar.size(); ++offset)
		{
			if (distance(ar, offset, offset + 1) > 1)
				break;
		}

		for (size_t k = 0; k < ar.size() - 1; ++k)
		{
			size_t pos = k + offset + 1;
			if (distance(ar, pos, pos + 1) == 1)
			{
				++r;
				++k;
			}
		}

		int64_t rounds = 1000000000 / 5000;
		r += rounds * (ar.size() / 2) * ((ar.size() + 1) / 2);
		return r * 2;
	}

	inline void solve()
	{
		int n;
		std::cin >> n;
		std::vector<int> ar(n);
		for (int k = 0; k < n; ++k)
			std::cin >> ar[k];
		std::cout << solve(ar) << std::endl;
	}
}