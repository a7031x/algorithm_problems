//https://www.hackerrank.com/contests/master/challenges/new-year-chaos
#include <binary_indexed_tree.hpp>
#include <console.hpp>

namespace new_year_chaos
{
	inline void solve()
	{
		console::tn<int>([](const std::vector<int>& arr)
		{
			algorithm_lib::binary_indexed_tree_t<int, 100000> tree;
			int sum = 0;
			for (auto a : arr)
			{
				int bribe_number = a - 1 - tree.read(a);
				if (bribe_number > 2)
				{
					std::cout << "Too chaotic" << std::endl;
					return;
				}
				tree.update(a, 1);

				sum += bribe_number;
			}
			std::cout << sum << std::endl;
		});
	}
}