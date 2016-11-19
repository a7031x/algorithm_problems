#pragma once
#include <vector>
#include <algorithm>

namespace algorithm_lib
{
	//little-endian
	template<typename T>
	inline void multiply(std::vector<T>& n, int base, const T& m, int64_t carry)
	{
		for (auto& v : n)
		{
			auto t = int64_t(v) * m + carry;
			carry = t / base;
			v = T(t - carry * base);
		}

		while (carry)
		{
			n.push_back(carry % base);
			carry /= base;
		}
	}

	//big-endian
	template<typename Iterator>
	inline std::vector<typename Iterator::value_type> change_system(Iterator first, Iterator last, int from, int to)
	{
		std::vector<typename Iterator::value_type> r;
		for (; first != last; ++first)
		{
			multiply(r, to, from, *first);
		}
		std::reverse(r.begin(), r.end());
		return r;
	}

	//big-endian
	template<typename T>
	inline std::vector<T> add(const std::vector<T>& a, const std::vector<T>& b, int base, T carry = 0)
	{
		std::vector<T> r;
		auto it0 = a.crbegin();
		auto it1 = b.crbegin();
		while (it0 != a.crend())
		{
			auto v = *it0++ + (it1 != b.crend() ? *it1++ : 0) + carry;
			if (v >= base)
			{
				v -= base;
				carry = 1;
			}
			else
				carry = 0;
			r.push_back(v);
		}
		std::reverse(r.begin(), r.end());
		return r;
	}

	//big-endian
	template<typename T>
	inline std::vector<T> substract(const std::vector<T>& a, const std::vector<T>& b, int base, T& carry)
	{
		std::vector<T> r;
		auto it0 = a.crbegin();
		auto it1 = b.crbegin();
		while (it0 != a.crend())
		{
			auto v = *it0++ - (it1 != b.crend() ? *it1++ : 0) - carry;
			if (v < 0)
			{
				v += base;
				carry = 1;
			}
			else
				carry = 0;
			r.push_back(v);
		}

		if (0 == r.back() && r.size() > 1)
			r.pop_back();

		std::reverse(r.begin(), r.end());
		return r;
	}

	//big-endian
	template<typename T>
	inline std::vector<T> substract(const std::vector<T>& a, const std::vector<T>& b, int base)
	{
		T carry = 0;
		return substract(a, b, base, carry);
	}
}
