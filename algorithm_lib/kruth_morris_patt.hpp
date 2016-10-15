#pragma once
#include <string>
#include <vector>

namespace algorithm_lib
{
	class kruth_morris_patt_t
	{
	private:
		std::vector<int> _jump_table;
		std::string _pattern;

	public:
		kruth_morris_patt_t(const std::string& pattern) : _pattern(pattern), _jump_table(pattern.size())
		{
			_jump_table[0] = -1;
			for (size_t i = 1; i < pattern.size(); i++)
			{
				auto k = _jump_table[i - 1];
				while (0 <= k)
				{
					if (pattern[k] == pattern[i - 1])
						break;
					else
						k = _jump_table[k];
				}
				_jump_table[i] = k + 1;
			}
		}

		template<typename FwIt>
		FwIt search(FwIt first, FwIt last)
		{
			int k = 0;
			while (first != last)
			{
				if (-1 == k)
				{
					if (++first == last)
						break;
					k = 0;
				}
				if (*first == _pattern[k])
				{
					if (++k == _pattern.size())
					{
						std::advance(first, 1 - k);
						return first;
					}
					++first;
				}
				else
				{
					k = _jump_table[k];
				}
			}
			return last;
		}
	};
}