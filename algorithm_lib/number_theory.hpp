#pragma once
#include <vector>

namespace algorithm_lib
{
	class number_theory_t
	{
	public:
		template<typename T>
		static T gcd(T a, T b)
		{
			while (b)
			{
				auto r = a % b;
				a = b;
				b = r;
			}
			return a;
		}

		static std::vector<int> mobius(int n)
		{
			std::vector<int> r(n + 1, -1);
			r[1] = 1;
			for (int i = 2; i <= n; ++i)
				for (int j = i * 2; j <= n; j += i)
					r[j] -= r[i];
			return r;
		}
	};
}