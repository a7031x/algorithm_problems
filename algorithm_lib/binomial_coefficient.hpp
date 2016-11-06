#pragma once
#include <vector>

namespace algorithm_lib
{
	class binomial_coefficient_t
	{
		std::vector<std::vector<int64_t>> _cache;
	public:
		binomial_coefficient_t(size_t size, int64_t moduli = 1000000007)
		{
			_cache.resize(size + 1);
			_cache[0].push_back(1);
			for (size_t k = 1; k <= size; ++k)
			{
				auto& last_row = _cache[k - 1];
				auto& row = _cache[k];
				row.resize(k + 1);
				row[0] = row[k] = 1;
				for (size_t i = 1; i < k; ++i)
					row[i] = (last_row[i - 1] + last_row[i]) % moduli;
			}
		}

		int64_t cnr(size_t n, size_t r)const { return r <= n ? _cache[n][r] : 0; }
	};
}