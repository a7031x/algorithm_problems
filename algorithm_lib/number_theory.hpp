#pragma once
#include <vector>

namespace algorithm_lib
{
	class number_theory_t
	{
	public:
		template<typename T>
		static T gcd(const T& a, const T& b)
		{
			T va = a;
			T vb = b;
			while (vb)
			{
				auto r = va % vb;
				va = vb;
				vb = r;
			}
			return va;
		}

		template<typename T, typename... U>
		static T gcd(const T& a, const T& b, const U&... others)
		{
			return gcd(gcd(a, b), others...);
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

		template<typename T>
		static std::vector<T> factors_within(T n)
		{
			std::vector<T> r((size_t)n + 1);
			for (T k = 1; k <= n; ++k)
				r[(size_t)k] = k;

			for (T i = 2; i <= n; ++i)
			{
				if (r[(size_t)i] != i)
					continue;
				for (T j = 2 * i; j <= n; j += i)
				{
					r[(size_t)j] = i;
				}
			}
			return r;
		}

		template<typename T>
		static std::vector<T> factors_of(T n)
		{
			auto factors = factors_within((T)sqrt(n));
			std::vector<T> r;
			for (size_t k = 1; k < factors.size(); ++k)
			{
				if ((T)k != factors[k])
					continue;
				if (n % (T)k == 0)
				{
					r.push_back((T)k);
					auto k2 = n / (T)k;
					if (k2 != (T)k)
						r.push_back(k2);
				}
			}
			return r;
		}

		template<typename T>
		static std::vector<T> prime_factors(T n)
		{
			auto factors = factors_within(n);
			std::vector<T> r;
			while (n != 1)
			{
				auto factor = factors[n];
				r.push_back(factor);
				n /= factor;
			}
			return r;
		}
	};
}
