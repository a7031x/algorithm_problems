//https://www.hackerrank.com/contests/master/challenges/pseudo-isomorphic-substrings
#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

namespace pseudo_isomorphic_substrings
{
	class suffix_tree_t
	{
	private:
		class node_t
		{
		public:
			size_t start;
			std::shared_ptr<size_t> end;
			std::unordered_map<int, std::shared_ptr<node_t>> children;
			node_t(size_t start, std::shared_ptr<size_t> end)
				: start(start), end(end)
			{}
		};

		typedef std::shared_ptr<node_t> nodeptr;

	private:
		nodeptr _root;
		std::vector<int> _text;
		int64_t _result;
		int64_t _incremental;

	public:
		suffix_tree_t(const std::vector<int>& text)
			: _text(text)
			, _root(std::make_shared<node_t>(-1, nullptr))
			, _result(0)
			, _incremental(0)
		{
			size_t suffix_count = 0;
			auto end = std::make_shared<size_t>(text.size());
			size_t active_depth = 0;
			nodeptr active_node = _root;
			for (size_t k = 0; k < text.size(); ++k)
			{
				++suffix_count;
				do {
					if (false == create_internal_node(active_node, active_depth, suffix_count - 1, k, end))
						break;
					active_depth = 0;
					active_node = _root;
				} while (--suffix_count);
				_result += _incremental;
				std::cout << _result << std::endl;
			}
		}

		int64_t result()const { return _result; }
	private:
		nodeptr create_internal_node(const nodeptr& parent, const nodeptr& child, size_t depth, size_t saved_depth, size_t pos, const nodeptr& leaf)
		{
			auto internal_node = std::make_shared<node_t>(
				child->start,
				std::make_shared<size_t>(child->start + depth));
			parent->children[at(pos - depth, saved_depth - depth)] = internal_node;
			child->start = *internal_node->end;
			internal_node->children[at(*internal_node->end, saved_depth)] = child;
			internal_node->children[at(pos, saved_depth)] = leaf;
			return internal_node;
		}

		bool equals(int c0, int c1, size_t depth)
		{
			if (c0 < 0 && -c0 >(int)depth)
				c0 = 'i';
			if (c1 < 0 && -c1 >(int)depth)
				c1 = 'i';
			return c0 == c1;
		}

		int at(size_t pos, size_t depth)
		{
			int c = _text[pos];
			if (c < 0 && -c > (int)depth)
				return 'i';
			else
				return c;
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

			while (child = parent->children[at(pos - depth, saved_depth - depth)])
			{
				auto edge_length = *child->end - child->start;
				if (edge_length > depth)
				{
					if (equals(_text[child->start + depth], _text[pos], saved_depth))
					{
						active_node = parent;
						active_depth = saved_depth - depth;
						return false;
					}
					auto internal_node = create_internal_node(parent, child, depth, saved_depth, pos, new_leaf);
					++_incremental;
					return true;
				}
				depth -= edge_length;
				parent = child;
			}
			parent->children[at(pos - depth, saved_depth - depth)] = new_leaf;
			++_incremental;
			return true;
		}
	};

	inline int64_t solve(const std::string& text)
	{
		std::vector<int> wtext;
		int buf[256] = { 0 };
		int k = 0;
		for (auto& c : text)
		{
			++k;
			if (buf[c])
			{
				wtext.push_back(buf[c] - k);
			}
			else
			{
				wtext.push_back('i');
			}
			buf[c] = k;
		}
		suffix_tree_t tree(wtext);
		return tree.result();
	}
}