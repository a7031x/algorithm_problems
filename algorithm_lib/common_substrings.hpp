#include <vector>
#include <string>
#include <number_theory.hpp>

namespace algorithm_lib
{
	template<int64_t _Mod = 1000000007>
	class common_substring_t
	{
	private:
		mutable std::vector<std::vector<int64_t>> _cache;
		std::string _a;
		std::string _b;

	public:
		int64_t count_with_cache(size_t start_a, size_t start_b)const
		{
			if (_a.size() == start_a || _b.size() == start_b)
				return 0;

			auto& cache = _cache[start_a][start_b];
			if (-1 != cache)
				return cache;
			if (_a[start_a] == _b[start_b])
			{
				cache = 1 + count_with_cache(start_a + 1, start_b) + count_with_cache(start_a, start_b + 1);
			}
			else
			{
				cache = count_with_cache(start_a + 1, start_b)
					+ count_with_cache(start_a, start_b + 1) - count_with_cache(start_a + 1, start_b + 1);
			}
			cache = algorithm_lib::number_theory_t::mod<int64_t>(cache, _Mod);
			return cache;
		}

		int64_t count(const std::string& a, const std::string& b)
		{
			_cache.clear();
			_cache.resize(a.size(), std::vector<int64_t>(b.size(), -1));
			_a = a;
			_b = b;
			return count_with_cache(0, 0);
		}
	};
}