#pragma once
#include <algorithm>
#include <array>

namespace algorithm_lib
{
	template<typename Type, int MaxValue>
	class binary_indexed_tree_t
	{
	private:
		std::array<Type, MaxValue> _buffer;

	public:
		binary_indexed_tree_t(const Type& defValue = Type())
		{
			std::for_each(_buffer.begin(), _buffer.end(), [defValue](Type& v) { v = defValue; });
		}

		Type read(int index) const
		{
			Type sum = Type();
			while (index > 0)
			{
				sum += _buffer[index - 1];
				index -= index & -index;
			}
			return sum;
		}

		void update(int index, const Type& value)
		{
			while (index <= MaxValue)
			{
				_buffer[index - 1] += value;
				index += index & -index;
			}
		}
	};
}
