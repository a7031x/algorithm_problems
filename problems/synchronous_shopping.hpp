#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <algorithm>

namespace synchronous_shopping
{
	struct shop_t
	{
		std::vector<size_t> mask_cost;
		shop_t(size_t mask_size) : mask_cost(mask_size, std::numeric_limits<size_t>::max()) {}
		shop_t(shop_t&& other)
		{
			mask_cost = std::move(other.mask_cost);
		}
		shop_t(const shop_t& other)
		{
			mask_cost = other.mask_cost;
		}
	};

	struct item_t
	{
		size_t shop;
		size_t cost;
		uint16_t mask;
		bool operator < (const item_t& other)const
		{
			if (cost < other.cost)
				return true;
			else if (cost > other.cost)
				return false;
			else if (shop > other.shop)
				return true;
			else if (shop < other.shop)
				return false;
			else if (mask > other.mask)
				return true;
			else if (mask < other.mask)
				return false;
			return false;
		}
	};

	inline size_t solve(const std::vector<std::map<size_t, size_t>>& graph, const std::vector<uint16_t>& shop, uint16_t terminal_mask)
	{
		std::vector<shop_t> blocker(shop.size(), shop_t(terminal_mask + 1));
		std::set<item_t> queue;

		item_t src = { 0, 0, shop[0] };
		blocker[0].mask_cost[shop[0]] = 0;
		queue.insert(src);

		while (queue.size())
		{
			auto item = queue.begin();
			for (auto& neighbor : graph[item->shop])
			{
				item_t next;
				next.shop = neighbor.first;
				next.mask = shop[neighbor.first] | item->mask;
				next.cost = neighbor.second + item->cost;
				
				auto& s = blocker[next.shop];
				if (s.mask_cost[next.mask] > next.cost)
				{
					queue.insert(next);
					s.mask_cost[next.mask] = next.cost;
				}
			}
			queue.erase(item);
		}

		auto& terminal_shop = *blocker.rbegin();
		size_t min_cost = std::numeric_limits<size_t>::max();
		for (uint16_t i = 0; i <= terminal_mask; ++i)
		{
			for (uint16_t j = 0; j <= terminal_mask; ++j)
			{
				if ((i | j) == terminal_mask)
					min_cost = std::min(min_cost, std::max(terminal_shop.mask_cost[i], terminal_shop.mask_cost[j]));
			}
		}

		return min_cost;
	}

	inline void solve()
	{
		int n, m, k;
		std::cin >> n >> m >> k;
		std::vector<uint16_t> shop(n);
		std::vector<std::map<size_t, size_t>> graph(n);
		uint16_t terminal_mask = (1 << k) - 1;

		for (int i = 0; i < n; ++i)
		{
			int types;
			std::cin >> types;
			for (int j = 0; j < types; ++j)
			{
				int type;
				std::cin >> type;
				--type;
				shop[i] |= (1 << type);
			}
		}

		for (int i = 0; i < m; ++i)
		{
			int x, y, z;
			std::cin >> x >> y >> z;
			--x;
			--y;
			graph[x][y] = z;
			graph[y][x] = z;
		}

		std::cout << solve(graph, shop, terminal_mask) << std::endl;
	}
}
