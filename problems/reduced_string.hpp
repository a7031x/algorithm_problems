//https://www.hackerrank.com/challenges/reduced-string
#include <string>
#include <iostream>

namespace reduced_string
{
	inline void solve()
	{
		std::string s;
		std::cin >> s;
		std::string r;
		for (auto c : s)
		{
			if (!r.empty() && r.back() == c)
				r.pop_back();
			else
				r.push_back(c);
		}
		if (r.empty())
			r = "Empty String";
		std::cout << r << std::endl;
	}
}

/*
int main()
{
	reduced_string::solve();
	return 0;
}
*/