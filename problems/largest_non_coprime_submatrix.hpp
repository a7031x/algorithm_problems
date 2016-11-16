//https://www.hackerrank.com/challenges/largest-coprime-submatrix
#include <iostream>
#include <algorithm>
#include <matrix.hpp>
#include <number_theory.hpp>

namespace largest_non_coprime_submatrix
{
	typedef algorithm_lib::matrix_t<int> matrix_t;

	inline int solve(const matrix_t& m, size_t row, size_t column)
	{
		int r = 0;
		std::vector<int> gcd_row(m.columns());
		for (size_t i = row; i < m.rows(); ++i)
		{
			int last_gcd = 0;
			for (size_t j = column; j < m.columns(); ++j)
			{
				gcd_row[j] = algorithm_lib::number_theory_t::gcd(m[i][j], gcd_row[j], last_gcd);
				if (1 == gcd_row[j])
					break;
				r = std::max(r, int(i - row + 1) * int(j - column + 1));
				last_gcd = gcd_row[j];
			}
		}
		return r;
	}

	inline void solve()
	{
		size_t n, m;
		std::cin >> n >> m;
		matrix_t mat(n, m);

		for (size_t r = 0; r < n; ++r)
			for (size_t c = 0; c < m; ++c)
				std::cin >> mat[r][c];
		int r = 0;
		for(size_t i = 0; i < n; ++i)
			for (size_t j = 0; j < m; ++j)
			{
				if ((n - i) * (m - j) <= (size_t)r)
					break;
				r = std::max(solve(mat, i, j), r);
			}
		std::cout << r << std::endl;
	}
}
