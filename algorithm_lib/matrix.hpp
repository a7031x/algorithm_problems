#pragma once
#include <vector>

namespace algorithm_lib
{
	template<typename T>
	class matrix_t
	{
		std::vector<T> _buf;
		size_t _rows;
		size_t _columns;

	public:
		matrix_t(size_t rows, size_t columns)
			: _rows(rows)
			, _columns(columns)
			, _buf(rows * columns)
		{
		}

		matrix_t(matrix_t&& other)
		{
			_buf = std::move(other._buf);
			_rows = other._rows;
			_columns = other._columns;
			other._rows = other._columns = 0;
		}

		size_t rows()const { return _rows; }
		size_t columns()const { return _columns; }
		T* operator[] (size_t row) { return _buf.data() + _columns * row; }
		const T* operator[] (size_t row)const { return _buf.data() + _columns * row; }
	};
}