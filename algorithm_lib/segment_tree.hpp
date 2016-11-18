#pragma once
#include <vector>
#include <limits>

namespace algorithm_lib
{
	template<typename T, T Mod = std::numeric_limits<T>::max()>
	struct add_op
	{
		static const T Unit = 0;
		T operator()(const T& x, const T& y)const
		{
			return (x + y) % Mod;
		}
	};

	template<typename T, T Mod = std::numeric_limits<T>::max()>
	struct mult_op
	{
		static const T Unit = 1;
		T operator()(const T& x, const T& y)const
		{
			return x * y % Mod;
		}
	};

	template<typename T, typename Op = add_op<T>>
	class segment_tree_t
	{
		std::vector<T> _data;
		size_t _range;
	public:
		segment_tree_t(size_t range) : _range(range), _data(min_size(range), Op::Unit)
		{
		}

		void update(size_t position, const T& value)
		{
			update(position, value, 0, 0, _range);
		}

		T aggregate(size_t first, size_t last)const
		{
			return aggregate(first, last, 0, 0, _range);
		}

	private:
		T aggregate(size_t first, size_t last, size_t index, size_t left, size_t right)const
		{
			if (left >= first && right <= last)
				return _data[index];

			if (right <= first || last <= left)
				return Op::Unit;

			size_t mid = (left + right) / 2;
			return Op()(aggregate(first, last, 2 * index + 1, left, mid), aggregate(first, last, 2 * index + 2, mid, right));
		}

		void update(size_t position, const T& value, size_t index, size_t left, size_t right)
		{
			if (position == left && position + 1 == right)
			{
				_data[index] = value;
				return;
			}

			if (right <= position || left > position)
				return;

			size_t mid = (left + right) / 2;
			if (position < mid)
			{
				update(position, value, index * 2 + 1, left, mid);
			}
			else
			{
				update(position, value, index * 2 + 2, mid, right);
			}
			_data[index] = Op()(_data[index], value);
		}

		size_t min_size(size_t range)const
		{
			size_t r = range;
			while (r < range * 2)
				r += r & (~r + 1);
			return r;
		}
	};
}
