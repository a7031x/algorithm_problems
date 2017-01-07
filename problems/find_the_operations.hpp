#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <memory.h>

namespace find_the_operations
{
	const int max_grid_size = 20;

	struct grid_t
	{
		bool data[max_grid_size][max_grid_size];
		bool& operator()(int x, int y) { return data[y][x]; }
		bool operator()(int x, int y)const { return data[y][x]; }
		grid_t()
		{
			memset(data, 0, sizeof(data));
		}
	};

	struct point_t
	{
		int x;
		int y;
		bool operator < (const point_t& other)const
		{
			return y * max_grid_size + x < other.y * max_grid_size + other.x;
		}
	};

	template<typename T>
	void flip(std::set<T>& s, const T& t)
	{
		if (s.count(t))
			s.erase(t);
		else
			s.insert(t);
	}

	void flip(grid_t& grid, const point_t& point, int n, int d)
	{
		for (int y = std::max(point.y - d, 0); y <= std::min(point.y + d, n - 1); ++y)
		{
			int r = d - std::abs(point.y - y);
			for (int x = std::max(point.x - r, 0); x <= std::min(point.x + r, n - 1); ++x)
			{
				grid(x, y) ^= 1;
			}
		}
	}

	std::map<point_t, grid_t> map_expressions(int n, int d)
	{
		std::map<point_t, grid_t> r;

		for (int x = 0; x < n; ++x)
			for (int y = 0; y < n; ++y)
			{
				grid_t g;
				flip(g, point_t{ x, y }, n, d);
				r[point_t{ x, y }] = g;
			}
		return r;
	}

	struct equation_t
	{
		std::set<point_t> left;
		bool right;
	};

	std::vector<equation_t> create_equations(const std::map<point_t, grid_t>& expressions, const grid_t& grid, int n, int d)
	{
		std::vector<equation_t> equations;
		for (int y = 0; y < n; ++y)
			for (int x = 0; x < n; ++x)
			{
				equation_t e;
				for (auto& kv : expressions)
				{
					if (kv.second(x, y))
						e.left.insert(kv.first);
				}
				e.right = grid(x, y);
				equations.push_back(e);
			}
		return equations;
	}

	void xadd(const equation_t& e0, equation_t& e1)
	{
		for (auto& p : e0.left)
		{
			flip(e1.left, p);
		}
		e1.right ^= e0.right;
	}

	std::set<point_t> solve_equations(std::vector<equation_t>& equations)
	{
		for (int i = 0; i < (int)equations.size(); ++i)
		{
			std::sort(equations.begin() + i, equations.end(), [](const equation_t& x, const equation_t& y)
			{
				return x.left < y.left;
			});

			auto& e0 = equations[i];
			if (0 == e0.left.size())
				continue;
			auto& first = *e0.left.begin();
			for (int j = i + 1; j < (int)equations.size(); ++j)
			{
				if (equations[j].left.count(first))
					xadd(e0, equations[j]);
			}
		}

		for (size_t k = 0; k < equations.size(); ++k)
		{
			if (0 == equations[k].left.size())
			{
				if (false == equations[k].right)
				{
					equations.erase(equations.begin() + k--);
					continue;
				}
				else
					throw std::exception();
			}
		}

		std::set<point_t> r;
		for (auto it = equations.rbegin(); it != equations.rend(); ++it)
		{
			auto& e = *it;
			for (auto& p : r)
			{
				if (e.left.count(p))
					e.right = !e.right;
			}
			if (e.right)
				r.insert(*e.left.begin());
		}
		return r;
	}

	std::set<point_t> solve(grid_t& g, int n, int d)
	{
		auto expressions = map_expressions(n, d);
		auto equations = create_equations(expressions, g, n, d);
		return solve_equations(equations);
	}

	void solve()
	{
		int n, d;
		std::cin >> n >> d;
		grid_t g;
		for (int y = 0; y < n; ++y)
			for (int x = 0; x < n; ++x)
				std::cin >> g(x, y);
		try {
			auto r = solve(g, n, d);
			std::cout << "Possible" << std::endl;
			std::cout << r.size() << std::endl;
			for (auto s : r)
				std::cout << s.y << " " << s.x << std::endl;
		}
		catch (const std::exception&)
		{
			std::cout << "Impossible" << std::endl;
		}
	}
}
