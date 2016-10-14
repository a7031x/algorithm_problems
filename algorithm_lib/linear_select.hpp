#include <vector>
#include <algorithm>

namespace algorithm_lib
{
	template<typename FwIt>
	inline auto linear_select(FwIt first, FwIt last, int k) -> decltype(*first)
	{
		auto n = std::distance(first, last);
		if (75 >= n)
		{
			std::sort(first, last);
			std::advance(first, k);
			return *first;
		}

		for (int k = 0; k < n / 5; ++k) {
			std::sort(first + k * 5, first + k * 5 + 5);
			std::swap(*(first + k), *(first + k * 5 + 2));
		}

		auto pivot = linear_select(first, first + n / 5, n / 10);
		auto mid = std::partition(first, last, [pivot](decltype(*first) x) {return x <= pivot; });
		if (mid - first == k)
			return *mid;
		if (mid - first > k) {
			return linear_select(first, mid, k);
		}
		else
			return linear_select(mid + 1, last, k - (mid - first) - 1);
	}
}