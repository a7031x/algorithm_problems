#include <input_output.hpp>
#include <algorithm>

namespace beautiful_binary_string
{
	inline void solve()
	{
		auto input = console::ns();
		std::string pattern = "010";
		int r = 0;
		for (auto it = input.cbegin(); input.cend() != it;)
		{
			it = std::search(it, input.cend(), pattern.cbegin(), pattern.cend());
			if (input.cend() == it)
				break;
			++r;
			std::advance(it, pattern.size());
		}
		std::cout << r << std::endl;
	}
}