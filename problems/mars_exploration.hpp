#include <input_output.hpp>

namespace mars_exploration
{
	inline void solve()
	{
		auto input = console::s();
		std::string pattern = "SOS";
		int r = 0;
		for (size_t k = 0; k < input.size(); ++k)
		{
			r += input[k] != pattern[k % pattern.size()];
		}
		std::cout << r << std::endl;
	}
}