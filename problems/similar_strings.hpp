//https://www.hackerrank.com/challenges/similar-strings
#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

namespace similar_strings
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
			int64_t leaves;
			size_t length()const { return end ? *end - start : 0; }
			node_t(size_t start, std::shared_ptr<size_t> end)
				: start(start), end(end), leaves(0)
			{}
		};

		typedef std::shared_ptr<node_t> nodeptr;

	private:
		nodeptr _root;
		std::vector<int> _text;

	public:
		suffix_tree_t(const std::vector<int>& text)
			: _text(text)
			, _root(std::make_shared<node_t>(-1, nullptr))
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
			}
			update_leaves(_root);
		}

		int64_t query(std::vector<int>::const_iterator first, std::vector<int>::const_iterator last)
		{
			nodeptr node = _root;
			size_t depth = 0;
			while (first != last)
			{
				if ((size_t)std::distance(first, last) <= node->length())
					return node->leaves;
				depth += node->length();
				std::advance(first, node->length());
				for (auto& kv : node->children)
				{
					if (equals(*first, kv.first, depth))
					{
						node = kv.second;
						break;
					}
				}
			}
			return node->leaves;
		}
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
			if (c < 0 && -c >(int)depth)
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
					return true;
				}
				depth -= edge_length;
				parent = child;
			}
			parent->children[at(pos - depth, saved_depth - depth)] = new_leaf;
			return true;
		}

		void update_leaves(const nodeptr& root)
		{
			if (0 == root->children.size())
				root->leaves = 1;
			else
			{
				for (auto& kv : root->children)
				{
					update_leaves(kv.second);
					root->leaves += kv.second->leaves;
				}
			}
		}
	};

	inline std::vector<int> isomorphic_array(const std::string& text)
	{
		std::vector<int> r;
		int buf[256] = { 0 };
		int k = 0;
		for (auto& c : text)
		{
			++k;
			if (buf[c])
			{
				r.push_back(buf[c] - k);
			}
			else
			{
				r.push_back('i');
			}
			buf[c] = k;
		}
		return r;
	}

	inline std::vector<int64_t> solve(const std::string& text, const std::vector<std::pair<size_t, size_t>>& queries)
	{
		auto v = isomorphic_array(text);
		v.push_back('$');
		suffix_tree_t st(v);
		std::vector<int64_t> r;
		for (auto& query : queries)
		{
			r.push_back(st.query(v.begin() + query.first, v.begin() + query.second));
		}
		return r;
	}

	inline void solve()
	{
		int m, q;
		std::string s;
		std::cin >> m >> q >> s;
		std::vector<std::pair<size_t, size_t>> queries;

		while (q--)
		{
			int a, b;
			std::cin >> a >> b;
			--a;
			queries.push_back(std::make_pair(a, b));
		}
		for (auto r : solve(s, queries))
			std::cout << r << std::endl;
	}

	inline std::vector<int64_t> naive(const std::string& text, const std::vector<std::pair<size_t, size_t>>& queries)
	{
		std::vector<int64_t> r;
		for (auto& query : queries)
		{
			int64_t sum = 0;
			auto ia0 = isomorphic_array(text.substr(query.first, query.second - query.first));
			for (size_t k = 0; k <= text.size() - query.second + query.first; ++k)
			{
				auto ia1 = isomorphic_array(text.substr(k, query.second - query.first));
				sum += ia0 == ia1;
			}
			r.push_back(sum);
		}
		return r;
	}

	inline void verify(const std::string& text, const std::vector<std::pair<size_t, size_t>>& queries)
	{
		auto r0 = naive(text, queries);
		auto r1 = solve(text, queries);
		for (size_t k = 0; k < r0.size(); ++k)
		{
			if (r0[k] != r1[k])
			{
				std::vector<std::pair<size_t, size_t>> qs;
				qs.push_back(queries[k]);
				std::cout << "query:" << " (" << qs[0].first << ", " << qs[0].second << ")" << std::endl;
				std::cout << "expect: " << r0[k] << std::endl;
				std::cout << "actual: " << r1[k] << std::endl;
			}
		}
	}

	void random_test()
	{
		std::string s;
		const size_t length = 12;
		const size_t alphabet_set = std::min(length / 5, (size_t)26);
		for (size_t k = 0; k < length; ++k)
			s += rand() % alphabet_set + 'a';
		std::vector<std::pair<size_t, size_t>> queries;
		queries.push_back(std::make_pair(0, 1));
		for (size_t k = 0; k < length; ++k)
		{
			size_t k0 = rand() % length + 1;
			size_t k1 = rand() % length + 1;
			if (k0 == k1)
				continue;
			if (k0 > k1)
				std::swap(k0, k1);
			queries.push_back(std::make_pair(k0, k1));
		}
		verify(s, queries);
	}
}