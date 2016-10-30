#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace console
{
	template<typename Type>
	inline void n(const std::function<void(const std::vector<Type>&)>& callback)
	{
		int n;
		std::cin >> n;
		std::vector<Type> values(n);
		for (auto& v : values)
			std::cin >> v;
		callback(values);
	}

	template<typename Type>
	inline void tn(const std::function<void(const std::vector<Type>&)>& callback)
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			n(callback);
		}
	}

	inline std::string s()
	{
		std::string s;
		std::cin >> s;
		return s;
	}

	inline std::string ns()
	{
		int n;
		std::cin >> n;
		return s();
	}

	inline void redirect(const std::string& file)
	{
		static std::ifstream ins;
		if (ins.is_open())
			ins.close();
		ins.open(file);
		std::cin.rdbuf(ins.rdbuf());
	}

	inline void redirect_string(const std::string& text)
	{
		static std::stringstream ss;
		ss.clear();
		ss << text;
		std::cin.rdbuf(ss.rdbuf());
	}

	template<typename T = int64_t, int MOD = 1000000007>
	inline T mod(T v)
	{
		return v % MOD;
	}
}