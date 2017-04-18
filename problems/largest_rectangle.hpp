//https://www.hackerrank.com/challenges/largest-rectangle
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

namespace largest_rectangle
{
	inline int64_t solve(const std::vector<int>& histogram)
	{
		std::map<int, int> h_positions;
		int64_t max_area = 0;
		int position = 0;
		for (auto h : histogram)
		{
			auto it = h_positions.lower_bound(h);
			if (h_positions.end() == it)
			{
				h_positions[h] = position;
			}
			else
			{
				max_area = std::max(max_area, (int64_t)h * (position - it->second + 1));

				h_positions[h] = it->second;

				for (auto last = h_positions.rbegin(); last->first > h; last = h_positions.rbegin())
				{
					max_area = std::max(max_area, (int64_t)last->first * (position - last->second));
					h_positions.erase(std::prev(h_positions.end()));
				}
			}
			++position;
		}

		return max_area;
	}

	inline void solve()
	{
		int n;
		std::cin >> n;
		std::vector<int> histogram(n);
		for (int k = 0; k < n; ++k)
			std::cin >> histogram[k];

		histogram.push_back(0);
		std::cout << solve(histogram) << std::endl;
	}
}
