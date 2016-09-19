#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <stack>
#include <algorithm>

namespace build_a_string
{
	class build_a_string_t
	{
	private:
		class node_t
		{
		public:
			size_t start;
			std::shared_ptr<size_t> end;
			std::unordered_map<char, std::shared_ptr<node_t>> children;
			node_t(size_t start, std::shared_ptr<size_t> end)
				: start(start), end(end)
			{}
		};

		typedef std::shared_ptr<node_t> nodeptr;

	private:
		nodeptr _root;
		std::string _text;
		std::vector<int64_t> _cost;
		int64_t _cost_a;
		int64_t _cost_b;
	public:
		build_a_string_t(int64_t a, int64_t b, const std::string& text)
			: _text(text)
			, _root(std::make_shared<node_t>(-1, nullptr))
			, _cost_a(a)
			, _cost_b(b)
			, _cost(text.size() + 1, INT64_MAX)
		{
			size_t suffix_count = 0;
			auto end = std::make_shared<size_t>(text.size());
			size_t active_depth = 0;
			nodeptr active_node = _root;
			_cost[0] = 0;
			for (size_t k = 0; k < text.size(); ++k)
			{
				++suffix_count;
				do {
					if (false == create_internal_node(active_node, active_depth, suffix_count - 1, k, end))
						break;
					active_depth = 0;
					active_node = _root;
				} while (--suffix_count);
				_cost[k + 1] = std::min(_cost[k + 1], _cost[k] + _cost_a);
			}
		}

		int64_t cost(size_t index)const
		{
			return _cost[index];
		}

	private:
		nodeptr create_internal_node(nodeptr parent, nodeptr child, size_t depth, size_t pos, nodeptr leaf)
		{
			auto internal_node = std::make_shared<node_t>(
				child->start,
				std::make_shared<size_t>(child->start + depth));
			parent->children[_text[pos - depth]] = internal_node;
			child->start = *internal_node->end;
			internal_node->children[_text[*internal_node->end]] = child;
			internal_node->children[_text[pos]] = leaf;
			return internal_node;
		}

		bool create_internal_node(nodeptr& active_node, size_t& active_depth, size_t depth, size_t pos, const std::shared_ptr<size_t>& end)
		{
			nodeptr child;
			nodeptr parent = _root;
			auto new_leaf = std::make_shared<node_t>(pos, end);

			auto saved_depth = depth;
			if (active_depth < depth)
			{
				parent = active_node;
				depth -= active_depth;
			}

			while (child = parent->children[_text[pos - depth]])
			{
				auto edge_length = *child->end - child->start;
				if (edge_length > depth)
				{
					if (_text[child->start + depth] == _text[pos])
					{
						active_node = parent;
						active_depth = saved_depth - depth;
						auto total_length = pos - child->start - depth + saved_depth + 1;
						int64_t length;
						if (total_length >= (saved_depth + 1) * 2)
							length = saved_depth + 1;
						else
						{
							auto pattern = pos - child->start - depth;
							auto mid = (total_length + 1) / 2;
							mid = std::min(mid, total_length + depth - saved_depth);
							auto right_start = (pattern - 1 + mid) / pattern * pattern;
							length = std::max(total_length - right_start, pattern);
						}
						_cost[pos + 1] = std::min(
						{
							_cost[pos + 1],
							_cost[pos + 1 - (size_t)length] + length * _cost_a,
							_cost[pos + 1 - (size_t)length] + _cost_b
						});
						return false;
					}
					auto internal_node = create_internal_node(parent, child, depth, pos, new_leaf);
					return true;
				}
				depth -= edge_length;
				parent = child;

			}
			parent->children[_text[pos - depth]] = new_leaf;
			return true;
		}
	};

	inline int64_t solve(int64_t a, int64_t b, const std::string& text)
	{
		return build_a_string_t(a, b, text).cost(text.size());
	}

	inline void solve()
	{
		int t;
		std::cin >> t;
		while (t--)
		{
			int a, b, c;
			std::string s;
			std::cin >> c >> a >> b >> s;
			std::cout << solve(a, b, s) << std::endl;
		}
	}
}
