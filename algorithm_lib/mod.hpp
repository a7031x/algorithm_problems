#pragma once
#include <stdint.h>
#include <memory.h>

namespace algorithm_lib
{
	template<typename T = int64_t, int64_t MOD = 1000000007>
	inline T mod(T v)
	{
		return v % MOD;
	}

	template<typename T = int64_t>
	inline T mod(T v, T m)
	{
		if (v >= 0)
			return v % m;
		else
			return (v % m + m) % m;
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

	template<size_t A, size_t B, size_t C, int64_t M = 1000000007>
	inline void matrix_multiply(int64_t a[A][B], int64_t b[B][C], int64_t c[A][C])
	{
		for(size_t k0 = 0; k0 < A; ++k0)
			for (size_t k2 = 0; k2 < C; ++k2)
			{
				int64_t sum = 0;
				for (size_t k1 = 0; k1 < B; ++k1)
					sum = mod(sum + a[k0][k1] * b[k1][k2]);
				c[k0][k2] = sum;
			}
	}

	template<size_t A, size_t B, size_t C, int64_t M = 1000000007>
	inline void matrix_multiply_inplace(int64_t a[A][B], int64_t b[B][C], int64_t c[A][C])
	{
		int64_t c0[A][C];
		matrix_multiply<A, B, C, false>(a, b, c0);
		memcpy(c, c0, sizeof(c0));
	}

	template<size_t N, int64_t M = 1000000007>
	inline void matrix_power_mod(int64_t m[N][N], int64_t p)
	{
		if (0 == p)
		{
			for (size_t r = 0; r < N; ++r)
				for (size_t c = 0; c < N; ++c)
					m[r][c] = r == c;
			return;
		}
		if (1 == p)
			return;
		if (p & 1)
		{
			int64_t m1[N][N];
			memcpy(m1, m, sizeof(m1));
			matrix_power_mod<N>(m, p / 2);
			matrix_multiply_inplace<N, N, N>(m, m, m);
			matrix_multiply_inplace<N, N, N>(m, m1, m);
		}
		else
		{
			matrix_power_mod<N>(m, p / 2);
			matrix_multiply_inplace<N, N, N>(m, m, m);
		}
	}
}
