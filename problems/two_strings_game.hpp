#include <memory>
#include <map>
#include <functional>
#include <stack>
#include <string>
#include <limits>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

namespace two_strings_game2
{
	class suffix_automation_t
	{
		struct state_t
		{
			size_t length = 0;
			size_t link = -1;
			int grundy_value = -1;
			int64_t grundy_sum[27] = { 0 };
			std::map<char, size_t> next;
		};

		std::vector<state_t> _states;
		std::vector<state_t*> _sorted_states;
		size_t _last;

	public:
		suffix_automation_t()
		{
			state_t start;
			_states.push_back(start);
			_last = 0;
		}

		suffix_automation_t(const std::string& s) : suffix_automation_t()
		{
			extend(s);
			_sorted_states.resize(_states.size());
			for (size_t k = 0; k < _states.size(); ++k)
			{
				_sorted_states[k] = &_states[k];
			}

			std::sort(_sorted_states.begin(), _sorted_states.end(), [](const state_t* s0, const state_t* s1)
			{
				return s0->length > s1->length;
			});

			update_grundy_values();
		}

		void extend(const std::string& s)
		{
			for (auto c : s)
				extend_one(c);
		}

		void extend_one(char c)
		{
			size_t current = _states.size();
			_states.push_back(state_t());
			_states[current].length = _states[_last].length + 1;
			size_t parent = _last;
			for (; -1 != parent && 0 == _states[parent].next.count(c); parent = _states[parent].link)
				_states[parent].next[c] = current;

			if (-1 == parent)
				_states[current].link = 0;
			else
			{
				size_t next = _states[parent].next[c];
				if (_states[next].length - 1 == _states[parent].length)
					_states[current].link = next;
				else
				{
					size_t clone = _states.size();
					state_t state = _states[next];
					state.length = _states[parent].length + 1;
					_states.push_back(state);
					for (; -1 != parent && _states[parent].next[c] == next; parent = _states[parent].link)
						_states[parent].next[c] = clone;
					_states[next].link = _states[current].link = clone;
				}
			}
			_last = current;
		}

		std::string kth_substring(int64_t k)const
		{
			auto cache = all_substrings();
			std::string s;
			kth_substring(cache, 0, k, s);
			return s;
		}

		void update_grundy_values()
		{
			for (auto state : _sorted_states)
			{
				bool mark[27] = { false };
				for (auto& kv : state->next)
				{
					auto& next = _states[kv.second];
					mark[next.grundy_value] = true;
					for (size_t k = 0; k < 27; ++k)
						state->grundy_sum[k] += next.grundy_sum[k];
				}
				for (size_t grundy_value = 0; grundy_value < sizeof(mark); ++grundy_value)
					if (!mark[grundy_value])
					{
						state->grundy_value = grundy_value;
						break;
					}
				++state->grundy_sum[state->grundy_value];
			}
		}

		static std::pair<std::string, std::string> solve(const std::string& a, const std::string& b, int64_t k)
		{
			suffix_automation_t sa(a);
			suffix_automation_t sb(b);

			std::vector<int64_t> counts = sb.all_substrings();
			std::string ra, rb;
			size_t left_index = 0;
			while (true)
			{
				const state_t& root_b = sb._states[0];
				int64_t strings = counts[0] - root_b.grundy_sum[sa._states[left_index].grundy_value];
				if (strings >= k)
					break;
				k -= strings;
				if (left_index + 1 == sa._states.size())
					return std::make_pair("no", "solution");
				for (auto& kv : sa._states[left_index].next)
				{
					const state_t& state = sa._states[kv.second];
					int64_t total = 0;
					left_index = kv.second;
					for (size_t gv = 0; gv < 27; ++gv)
					{
						int64_t count0 = state.grundy_sum[gv];
						int64_t count1 = counts[0] - root_b.grundy_sum[gv];
						int64_t strings = count0 * count1;
						if (0 != count0 && strings / count0 != count1)
							total = k + 1;
						else
							total += strings;
						if (total >= k)
							break;
					}
					if (total >= k)
					{
						ra += kv.first;
						break;
					}
					k -= total;
					left_index = sa._states.size();
				}
				if (sa._states.size() <= left_index)
					return std::make_pair("no", "solution");
			}

			size_t right_index = 0;
			int grundy_value = sa._states[left_index].grundy_value;
			if (counts[0] - sb._states[0].grundy_sum[grundy_value] < k)
				return std::make_pair("no", "solution");
			while (k)
			{
				const state_t& state = sb._states[right_index];
				if (state.grundy_value != grundy_value)
					--k;
				else if (sb._states.size() == right_index + 1)
					return std::make_pair("no", "solution");
				if (0 == k)
					break;
				for (auto& kv : state.next)
				{
					const state_t& child = sb._states[kv.second];
					int64_t strings = counts[kv.second] - child.grundy_sum[grundy_value];
					if (strings >= k)
					{
						right_index = kv.second;
						rb.push_back(kv.first);
						break;
					}
					k -= strings;
				}
			}
		//	std::cout << sizeof(state_t) << std::endl;
			return std::make_pair(ra, rb);
		}

	private:
		std::vector<int64_t> all_substrings()const
		{
			std::vector<int64_t> cache(_states.size(), 0);
			for (auto state : _sorted_states)
			{
				auto& c = cache[state - _states.data()];
				c = 1;
				for (auto& kv : state->next)
					c += cache[kv.second];
			}
			return cache;
		}

		void kth_substring(const std::vector<int64_t>& counts, size_t root, int64_t k, std::string& s)const
		{
			if (0 == k)
				return;
			for (auto& kv : _states[root].next)
			{
				if (counts[kv.second] >= k)
				{
					s.push_back(kv.first);
					kth_substring(counts, kv.second, k - 1, s);
					return;
				}
				k -= counts[kv.second];
			}
		}
	};

	static std::pair<std::string, std::string> solve(const std::string& a, const std::string& b, int64_t k)
	{
		return suffix_automation_t::solve(a, b, k);
	}

	inline void solve()
	{
		int a, b;
		int64_t k;
		std::cin >> a >> b >> k;
		std::string sa, sb;
		std::cin >> sa >> sb;
		auto r = suffix_automation_t::solve(sa, sb, k);
		if ("no" == r.first && "solution" == r.second)
			std::cout << r.first << " " << r.second << std::endl;
		else
			std::cout << r.first << std::endl << r.second << std::endl;
	}
}

namespace two_strings_game
{
	class suffix_tree_t
	{
	public:
		enum state_t {none, win, lose, both};
		class node_t
		{
		public:
			size_t start;
			std::shared_ptr<size_t> end;
			std::map<char, std::shared_ptr<node_t>> children;
			int64_t win_counter;
			int64_t lose_counter;
			int64_t none_counter;
			int64_t both_counter;
			state_t state;
			size_t length()const { return end ? *end - start : 0; }
			node_t(size_t start, std::shared_ptr<size_t> end)
				: start(start), end(end), win_counter(0), lose_counter(0), none_counter(0), both_counter(0), state(none)
			{}
		};

		typedef std::shared_ptr<node_t> nodeptr;

	private:
		nodeptr _root;
		std::string _text;
		int64_t _win_counter;
		int64_t _lose_counter;
		int64_t _none_counter;
		int64_t _both_counter;
	public:
		suffix_tree_t(const std::string& text)
			: _text(text)
			, _root(std::make_shared<node_t>(-1, nullptr))
			, _win_counter(0)
			, _lose_counter(0)
			, _none_counter(0)
			, _both_counter(0)
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
			create_state(_root);
			nodeptr last_node;
			create_counters(last_node, _root);
			_win_counter = last_node->win_counter;
			_lose_counter = last_node->lose_counter;
			_none_counter = last_node->none_counter;
			_both_counter = last_node->both_counter;
			switch (last_node->state)
			{
			case win: ++_win_counter; break;
			case lose: ++_lose_counter; break;
			case none: ++_none_counter; break;
			case both: ++_both_counter; break;
			}
		}

		state_t root_state()const { return _root->state; }
		const std::string& text()const { return _text; }

		std::string full_path(const std::string& path)const
		{
			auto node = _root;
			std::string r;
			for (auto c : path)
			{
				auto child = node->children[c];
				r += _text.substr(child->start, *child->end - child->start);
				node = child;
			}
			return r;
		}

		void create_state(const nodeptr& node)
		{
			for (auto& kv : node->children)
			{
				auto& child = kv.second;
				create_state(child);
				auto state = determine_state(child->state, child->length());
				node->state = shift_state(node->state, state);
			}
		}

		void create_counters(nodeptr& prev_node, const nodeptr& node)
		{
			if (prev_node)
			{
				node->none_counter = prev_node->none_counter + (prev_node->state == none);
				node->win_counter = prev_node->win_counter + (prev_node->state == win);
				node->lose_counter = prev_node->lose_counter + (prev_node->state == lose);
				node->both_counter = prev_node->both_counter + (prev_node->state == both);
			}

			if (node->end)
			{
				size_t length = node->length() - 1;
				size_t counters[] =
				{
					length / 2,
					length - length / 2
				};
				switch (node->state)
				{
				case none:
				case both:
					node->none_counter += counters[node->state == both];
					node->both_counter += counters[node->state == none];
					break;
				case win:
				case lose:
					node->win_counter += counters[node->state == lose];
					node->lose_counter += counters[node->state == win];
					break;
				}
			}
			prev_node = node;
			for (auto& kv : node->children)
			{
				auto& child = kv.second;
				create_counters(prev_node, child);
			}
		}

		void traverse(const std::function<bool(
			const nodeptr& node,
			const std::string& path,
			size_t path_length)>& callback)const
		{
			struct item_t
			{
				std::string path;
				size_t length;
				nodeptr node;
				item_t(const std::string& path, size_t length, nodeptr node)
					: path(path), length(length), node(node)
				{}
				item_t(item_t&& other)
				{
					path = std::move(other.path);
					length = other.length;
					node = std::move(other.node);
				}
			};

			std::stack<item_t> queue;
			queue.push(item_t("", 0, _root));

			while (!queue.empty())
			{
				auto parent = std::move(queue.top());
				queue.pop();

				for (auto child : parent.node->children)
				{
					queue.push(
						item_t(
							parent.path + child.first,
							parent.length + child.second->length(),
							child.second));
				}
				if (false == callback(parent.node, parent.path, parent.length))
					return;
			}
		}

		static state_t reverse_state(state_t state)
		{
			switch (state)
			{
			case none: return both;
			case both: return none;
			case win: return lose;
			case lose: return win;
			default: throw;
			}
		}

		int64_t none_counter()const { return _none_counter; }
		int64_t win_counter()const { return _win_counter; }
		int64_t lose_counter()const { return _lose_counter; }
		int64_t both_counter()const { return _both_counter; }

		static state_t determine_state(state_t state, int dist)
		{
			switch (state)
			{
			case none:
				return dist % 2 ? both : none;
			case win:
				return dist % 2 ? lose : win;
			case lose:
				return dist % 2 ? win : lose;
			case both:
				return dist % 2 ? none : both;
			default:
				throw;
			}
		}

		static bool win_state(state_t state0, state_t state1)
		{
			switch (state0)
			{
			case none:
			case lose:
				return both == state1 || win == state1;
			case win:
				return none == state1 || lose == state1 || both == state1;
			case both:
				return none == state1 || win == state1 || lose == state1;
			default:
				return false;
			}
		}

		nodeptr proceed(const std::string& path, size_t& pos)const
		{
			nodeptr node = _root;
			pos = 0;

			for (auto c : path)
			{
				if (node->length() == pos)
				{
					node = node->children[c];
					pos = 0;
				}
				else
					++pos;
			}
			return node;
		}

	private:
		state_t shift_state(state_t current, state_t parallel)const
		{
			return (state_t)(current | parallel);
		}

		nodeptr create_internal_node(nodeptr parent, nodeptr child, size_t depth, size_t pos, const nodeptr& leaf)
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

		bool create_internal_node(nodeptr& active_node, size_t& active_depth, size_t depth, size_t pos, std::shared_ptr<size_t> end)
		{
			nodeptr child;
			nodeptr parent = _root;
			auto new_leaf = std::make_shared<node_t>(pos, end);
			new_leaf->state = lose;
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

	class verifier_t
	{
		struct game_state_t
		{
			suffix_tree_t::nodeptr node_a;
			suffix_tree_t::nodeptr node_b;
			size_t pos_a;
			size_t pos_b;
		};

	private:
		const suffix_tree_t* _ta;
		const suffix_tree_t* _tb;

	public:
		verifier_t(const suffix_tree_t& tree_a, const suffix_tree_t& tree_b)
		{
			_ta = &tree_a;
			_tb = &tree_b;
		}

		bool check(const std::string& state_a, const std::string& state_b)const
		{
			game_state_t state;
			state.node_a = _ta->proceed(state_a, state.pos_a);
			state.node_b = _tb->proceed(state_b, state.pos_b);
			return check(state);
		}

		std::set<std::pair<std::string, std::string>> check_all()const
		{
			std::set<std::pair<std::string, std::string>> r;
			game_state_t state;
			state.node_a = _ta->proceed("", state.pos_a);
			state.node_b = _tb->proceed("", state.pos_b);
			struct item_t
			{
				game_state_t state;
				std::string a;
				std::string b;
			};
			std::list<item_t> queue;
			item_t root;
			root.state = state;
			queue.push_back(root);
			while (queue.size())
			{
				auto& item = queue.front();
				if (check(item.state))
				{
					r.insert(std::make_pair(item.a, item.b));
				}
				auto& state = item.state;
				if (state.pos_a + 1 < state.node_a->length())
				{
					auto item1 = item;
					item1.a.push_back(_ta->text()[++item1.state.pos_a + item1.state.node_a->start]);
					queue.push_back(item1);
				}
				else
				{
					for (auto& c : state.node_a->children)
					{
						auto item1 = item;
						item1.state.pos_a = 0;
						item1.state.node_a = c.second;
						item1.a.push_back(_ta->text()[item1.state.node_a->start]);
						queue.push_back(item1);
					}
				}
				if (state.pos_b + 1 < state.node_b->length())
				{
					auto item1 = item;
					item1.b.push_back(_tb->text()[++item1.state.pos_b + item1.state.node_b->start]);
					queue.push_back(item1);
				}
				else
				{
					for (auto& c : state.node_b->children)
					{
						auto item1 = item;
						item1.state.pos_b = 0;
						item1.state.node_b = c.second;
						item1.b.push_back(_tb->text()[item1.state.node_b->start]);
						queue.push_back(item1);
					}
				}
				queue.pop_front();
			}

			return r;
		}
	private:

		bool check(const game_state_t& state)const
		{
			if (state.pos_a + 1 < state.node_a->length())
			{
				auto state1 = state;
				++state1.pos_a;
				if (!check(state1))
					return true;
			}
			else
			{
				for (auto& c : state.node_a->children)
				{
					auto state1 = state;
					state1.pos_a = 0;
					state1.node_a = c.second;
					if (!check(state1))
						return true;
				}
			}

			if (state.pos_b + 1 < state.node_b->length())
			{
				auto state1 = state;
				++state1.pos_b;
				if (!check(state1))
					return true;
			}
			else
			{
				for (auto& c : state.node_b->children)
				{
					auto state1 = state;
					state1.pos_b = 0;
					state1.node_b = c.second;
					if (!check(state1))
						return true;
				}
			}

			return false;
		}

		bool terminal(const suffix_tree_t::nodeptr& node, size_t pos)const
		{
			return node->children.size() == 0 && pos == node->length();
		}
	};

	inline std::string traverse(
		const suffix_tree_t& tree,
		int64_t none_base,
		int64_t win_base,
		int64_t lose_base,
		int64_t both_base,
		int64_t& k,
		suffix_tree_t::state_t& state)
	{
		std::string r;
		state = tree.root_state();
		tree.traverse([&](const suffix_tree_t::nodeptr& node,
			const std::string& path,
			size_t path_length)
		{
			for (size_t dist = node->length() - 1; dist != -1; --dist)
			{
				auto win_counter = node->win_counter;
				auto lose_counter = node->lose_counter;
				auto none_counter = node->none_counter;
				auto both_counter = node->both_counter;
				switch (node->state)
				{
				case suffix_tree_t::none:
				case suffix_tree_t::both:
					none_counter -= (dist + (node->state == suffix_tree_t::both)) / 2;
					both_counter -= (dist + (node->state == suffix_tree_t::none)) / 2;
					break;
				case suffix_tree_t::win:
				case suffix_tree_t::lose:
					win_counter -= (dist + (node->state == suffix_tree_t::lose)) / 2;
					lose_counter -= (dist + (node->state == suffix_tree_t::win)) / 2;
					break;
				}

				const int64_t limit_k = std::numeric_limits<int64_t>::max() / 4;
				if (win_counter && limit_k / win_counter < (lose_base + none_base + both_base) ||
					(lose_counter + none_counter) && limit_k / (lose_counter + none_counter) < (win_base + both_base) ||
					both_counter && limit_k / both_counter < (win_base + lose_base + none_base))
					return true;

				auto lk = win_counter * (lose_base + none_base + both_base)
					+ lose_counter * (win_base + both_base)
					+ none_counter * (win_base + both_base)
					+ both_counter * (win_base + lose_base + none_base);

				auto rk = k - lk;

				if (rk <= 0)
					return true;
				state = suffix_tree_t::determine_state(node->state, dist);
				switch (state)
				{
				case suffix_tree_t::win:
					if (rk > lose_base + none_base + both_base)
						continue;
					break;
				case suffix_tree_t::lose:
					if (rk > win_base + both_base)
						continue;
					break;
				case suffix_tree_t::none:
					if (rk > win_base + both_base)
						continue;
					break;
				case suffix_tree_t::both:
					if (rk > win_base + lose_base + none_base)
						continue;
					break;
				}
				r = tree.full_path(path);
				r.resize(r.size() - dist);
				k = rk;
				return false;
			}
			return true;
		});
		if (k > none_base + win_base + lose_base + both_base)
			return "no solution";
		else
			return r;
	}

	inline std::string traverse(const suffix_tree_t& tree, suffix_tree_t::state_t state, int64_t& k)
	{
		std::string r;
		tree.traverse([&](const suffix_tree_t::nodeptr& node,
			const std::string& path,
			size_t path_length)
		{
			for (size_t dist = (node->length() + (node->length() == 0)) - 1; dist != -1; --dist)
			{
				auto node_state = suffix_tree_t::determine_state(node->state, dist);
				if (false == suffix_tree_t::win_state(node_state, state))
					continue;
				auto win_counter = node->win_counter + (node_state == suffix_tree_t::win);
				auto lose_counter = node->lose_counter + (node_state == suffix_tree_t::lose);
				auto none_counter = node->none_counter + (node_state == suffix_tree_t::none);
				auto both_counter = node->both_counter + (node_state == suffix_tree_t::both);
				switch (node->state)
				{
				case suffix_tree_t::none:
				case suffix_tree_t::both:
					none_counter -= (dist + (node->state == suffix_tree_t::both)) / 2;
					both_counter -= (dist + (node->state == suffix_tree_t::none)) / 2;
					break;
				case suffix_tree_t::win:
				case suffix_tree_t::lose:
					win_counter -= (dist + (node->state == suffix_tree_t::lose)) / 2;
					lose_counter -= (dist + (node->state == suffix_tree_t::win)) / 2;
					break;
				}
				int64_t lk;
				switch (state)
				{
				case suffix_tree_t::none:
					lk = win_counter + both_counter;
					break;
				case suffix_tree_t::both:
					lk = win_counter + lose_counter + none_counter;
					break;
				case suffix_tree_t::win:
					lk = lose_counter + none_counter + both_counter;
					break;
				case suffix_tree_t::lose:
					lk = win_counter + both_counter;
					break;
				}

				if (k < lk)
					return true;

				auto rk = k - lk;
				if (rk != 0)
					continue;

				r = tree.full_path(path);
				r.resize(r.size() - dist);
				k = rk;
				return false;
			}
			return true;
		});
		if (k)
			return "no solution";
		else
			return r;
	}

	inline std::pair<std::string, std::string> solve(const std::string& a, const std::string& b, int64_t k)
	{
		suffix_tree_t ta(a);
		suffix_tree_t tb(b);

		suffix_tree_t::state_t state;
		auto ra = traverse(ta, tb.none_counter(), tb.win_counter(), tb.lose_counter(), tb.both_counter(), k, state);
		if(ra == "no solution")
			return std::pair<std::string, std::string>(ra, "");
		auto rb = traverse(tb, state, k);
		if (rb == "no solution")
			return std::pair<std::string, std::string>(rb, "");
		return std::make_pair(ra, rb);
	}

	inline void solve()
	{
		int a, b;
		int64_t k;
		std::cin >> a >> b >> k;
		std::string sa, sb;
		std::cin >> sa >> sb;
		auto r = solve(sa, sb, k);
		std::cout << r.first << std::endl << r.second << std::endl;
	}
}