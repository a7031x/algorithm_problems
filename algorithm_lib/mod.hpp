#pragma once
#include <stdint.h>

namespace algorithm_lib
{
	template<typename T = int64_t, int MOD = 1000000007>
	inline T mod(T v)
	{
		return v % MOD;
	}

	inline int64_t power_mod(int64_t a, int64_t p, int64_t m)
	{
		if (1 == p)
			return a % m;
		if (0 == p)
			return 1;

		int64_t r = power_mod(a, p / 2, m);
		r = r * r % m;
		if (p & 1)
			return r * a % m;
		else
			return r;
	}
}