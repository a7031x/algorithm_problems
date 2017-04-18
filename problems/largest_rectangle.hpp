//https://www.hackerrank.com/challenges/largest-rectangle
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>

namespace largest_rectangle
{
	inline int64_t solve(const std::vector<int>& histogram)
	{
		std::stack<int> s;
		int64_t max_area = 0;
		for (int position = 0; position < (int)histogram.size(); ++position)
		{
			auto h = s.empty() ? -1 : histogram[s.top()];
			if (h < histogram[position])
				s.push(position);
			else
			{
				s.pop();
				max_area = std::max((int64_t)h * (position - (s.empty() ? 0 : s.top() + 1)), max_area);
				--position;
			}
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
