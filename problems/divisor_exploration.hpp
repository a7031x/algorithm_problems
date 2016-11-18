//https://www.hackerrank.com/challenges/divisor-exploration
#include <iostream>
#include <segment_tree.hpp>
#include <mod.hpp>

namespace divisor_exploration
{
	inline void solve()
	{
		int t;
		std::cin >> t;

		algorithm_lib::segment_tree_t<int64_t, algorithm_lib::mult_op<int64_t, algorithm_lib::m1e9n7>> st(1000001);
		for (int64_t k = 0; k < 1000001; ++k)
		{
			int64_t m = algorithm_lib::mod((3 + k) * (2 + k) / 2);
			st.update((size_t)k, m);
		}

		while (t--)
		{
			int m, a;
			std::cin >> m >> a;
			std::cout << st.aggregate(a, a + m) << std::endl;
		}
	}
}